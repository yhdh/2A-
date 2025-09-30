#!/usr/bin/env python3
# courbe.py  — tracer la perf de malloc en fonction de la taille mémoire
# Exemples :
#   python3 courbe.py result_malloc.csv
#   python3 courbe.py result_malloc.csv --no-show
#   python3 courbe.py --xcol size --ycol time --sep ';'

import argparse
import sys
from pathlib import Path
import re

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

# ---------- Units & regex ----------

_UNITS_BYTES = {
    "B": 1,
    # IEC/anglo
    "K": 1024, "KB": 1024, "KIB": 1024,
    "M": 1024**2, "MB": 1024**2, "MIB": 1024**2,
    "G": 1024**3, "GB": 1024**3, "GIB": 1024**3,
    # FR
    "KO": 1024, "MO": 1024**2, "GO": 1024**3,
}

_TIME_UNITS = {
    "S": 1.0,
    "MS": 1e-3,
    "US": 1e-6,
    "µS": 1e-6,
}

SIZE_RE = re.compile(
    r"^\s*(?P<num>[0-9]+(?:[.,][0-9]+)?)\s*(?P<unit>b|kb|kib|k|ko|mb|mib|m|mo|gb|gib|g|go)?\s*$",
    re.IGNORECASE,
)
TIME_RE = re.compile(
    r"^\s*(?P<num>[0-9]+(?:[.,][0-9]+)?)\s*(?P<unit>s|ms|us|µs)?\s*$",
    re.IGNORECASE,
)

# ---------- Helpers ----------

def _to_float(s):
    if s is None:
        return None
    s = str(s).strip()
    if not s:
        return None
    for cand in (s.replace(" ", ""), s.replace(",", "."), s.replace(",", "")):
        try:
            return float(cand)
        except Exception:
            pass
    return None

def parse_size_to_bytes_series(series: pd.Series) -> pd.Series:
    def parse_one(x):
        if x is None:
            return np.nan
        s = str(x).strip()
        if not s:
            return np.nan
        m = SIZE_RE.match(s.replace(" ", ""))
        if not m:
            # fallback : extraire juste les chiffres
            digits = "".join(ch for ch in s if ch.isdigit() or ch in ".,")
            v = _to_float(digits)
            return np.nan if v is None else v
        num = _to_float(m.group("num"))
        if num is None:
            return np.nan
        unit = (m.group("unit") or "B").upper()
        unit = {"": "B", "K": "K", "M": "M", "G": "G"}.get(unit, unit)
        mult = _UNITS_BYTES.get(unit, 1)
        return float(num) * mult
    out = series.map(parse_one)
    return pd.to_numeric(out, errors="coerce")

def parse_time_to_seconds_series(series: pd.Series, default_unit: str = None) -> pd.Series:
    """Vers secondes. Gère s/ms/us et valeurs sans unité (heuristique ou --time-unit)."""
    def parse_one(x):
        if x is None:
            return np.nan
        s = str(x).strip()
        if not s:
            return np.nan
        m = TIME_RE.match(s.replace(" ", ""))
        if not m:
            val = _to_float(s)
            return np.nan if val is None else val
        num = _to_float(m.group("num"))
        if num is None:
            return np.nan
        unit = m.group("unit")
        if unit:
            mult = _TIME_UNITS.get(unit.upper(), 1.0)
            return float(num) * mult
        return float(num)
    vals = series.map(parse_one)
    if default_unit:
        mult = _TIME_UNITS.get(default_unit.upper(), 1.0)
        return vals * mult
    med = np.nanmedian(vals)
    if med > 1e6:
        return vals * _TIME_UNITS["US"]
    if med > 1000:
        return vals * _TIME_UNITS["MS"]
    return vals  # secondes

def guess_columns(df: pd.DataFrame):
    cols = [c.strip() for c in df.columns]
    def has_any(name, keys):
        n = name.lower()
        return any(k in n for k in keys)
    xcol = next((c for c in cols if has_any(c, ["size", "bytes", "malloc_size", "alloc", "taille", "octets"])), None)
    ycol = next((c for c in cols if has_any(c, ["time", "elapsed", "latency", "duration", "temps", "ms", "sec"])), None)
    return xcol, ycol

def human_bytes(x, pos):
    if x is None or np.isnan(x):
        return ""
    for unit, div in (("Gio", 1024**3), ("Mio", 1024**2), ("Kio", 1024)):
        if x >= div:
            return f"{x/div:.0f} {unit}"
    return f"{int(x)} o"

# ---------- I/O ----------

def read_any_csv(path: Path, sep: str = None) -> pd.DataFrame:
    """Lit en ignorant les lignes de commentaires (#) et en gérant les espaces multiples."""
    kwargs = {"comment": "#"}
    if sep:
        # supporte espaces multiples si sep=r"\s+"
        return pd.read_csv(path, sep=sep, engine="python", **kwargs)
    # Essai 1 : colonnes séparées par espaces/tab sans header
    try:
        return pd.read_csv(path, engine="python", delim_whitespace=True, **kwargs)
    except Exception:
        pass
    # Essai 2 : auto (virgule)
    try:
        return pd.read_csv(path, engine="python", **kwargs)
    except Exception:
        pass
    # Essais restants
    for s in (";", "\t", "|", r"\s+"):
        try:
            return pd.read_csv(path, sep=s, engine="python", **kwargs)
        except Exception:
            continue
    raise ValueError("Impossible de lire le fichier comme CSV/TSV/whitespace.")

# ---------- Main ----------

def main():
    ap = argparse.ArgumentParser(description="Tracer la performance de malloc en fonction de la taille.")
    ap.add_argument("csv", nargs="?", default="result_malloc.csv", help="Fichier d'entrée")
    ap.add_argument("--xcol", help="Nom de la colonne taille")
    ap.add_argument("--ycol", help="Nom de la colonne temps")
    ap.add_argument("--sep", help="Séparateur CSV (ex: ';' ou '\\s+')", default=None)
    ap.add_argument("--out", default=None, help="Chemin du PNG de sortie")
    ap.add_argument("--no-show", action="store_true", help="N'affiche pas la fenêtre matplotlib")
    ap.add_argument("--yscale", choices=["linear", "log"], default="linear", help="Échelle Y")
    ap.add_argument("--title", default="Performance de malloc en fonction de la taille (1 thread)")
    ap.add_argument("--time-unit", choices=["s", "ms", "us"], default=None,
                    help="Force l’unité des temps quand elle n’est pas indiquée")
    args = ap.parse_args()

    csv_path = Path(args.csv)
    if not csv_path.exists():
        sys.exit(f"CSV introuvable : {csv_path}")

    # Lecture initiale
    try:
        df_raw = read_any_csv(csv_path, args.sep)
    except Exception as e:
        sys.exit(f"Erreur de lecture : {e}")

    df_raw.columns = [str(c).strip() for c in df_raw.columns]

    # 1) Tentative via en-têtes/détection de colonnes
    xcol, ycol = args.xcol, args.ycol
    if not xcol or not ycol:
        gx, gy = guess_columns(df_raw)
        xcol = xcol or gx
        ycol = ycol or gy

    df = None
    if xcol and ycol and (xcol in df_raw.columns) and (ycol in df_raw.columns):
        size_bytes = parse_size_to_bytes_series(df_raw[xcol])
        time_s = parse_time_to_seconds_series(df_raw[ycol], default_unit=args.time_unit)
        df = pd.DataFrame({"size_bytes": size_bytes, "time_s": time_s})

    # 2) Fallback spécial : layout sans en-têtes "N ko  nb_double  time  ..."
    if df is None or df.dropna().empty:
        try:
            tmp = pd.read_csv(csv_path, comment="#", delim_whitespace=True,
                              header=None, engine="python")
            # Doit au minimum avoir 4 colonnes : 0=N, 1='ko', 2=nb_double, 3=time
            if tmp.shape[1] >= 4:
                size_bytes = (tmp.iloc[:, 0].astype(float) * 1024.0)
                time_s = tmp.iloc[:, 3].astype(float)
                df = pd.DataFrame({"size_bytes": size_bytes, "time_s": time_s})
        except Exception:
            pass

    if df is None or df.replace([np.inf, -np.inf], np.nan).dropna().empty:
        sys.exit(f"Colonnes non trouvées. Colonnes disponibles : {list(df_raw.columns)}\n"
                 "Tu peux forcer avec --xcol et --ycol, ou laisse le fallback traiter le format 'N ko ...'.")

    # Nettoyage / tri
    df = df.replace([np.inf, -np.inf], np.nan).dropna()
    df = df.sort_values("size_bytes").reset_index(drop=True)

    # Export clean (utile pour audit)
    clean_csv = csv_path.with_name(csv_path.stem + "_clean.csv")
    df.to_csv(clean_csv, index=False)

    # Plot
    fig = plt.figure(figsize=(10, 5))
    ax = plt.gca()
    ax.plot(df["size_bytes"].to_numpy(), df["time_s"].to_numpy(),
            marker="o", linewidth=1, label="Temps d'exécution malloc")

    ax.set_xlabel("Taille mémoire")
    ax.set_ylabel("Temps (secondes)")
    ax.set_title(args.title)
    ax.grid(True, which="both", axis="both")
    ax.set_xscale("linear")
    ax.set_yscale(args.yscale)
    ax.xaxis.set_major_formatter(FuncFormatter(human_bytes))

    y_max = float(df["time_s"].max()) if len(df) else 1.0
    ax.set_ylim(0.0, y_max * 1.08 if y_max > 0 else 1.0)
    ax.legend()
    plt.tight_layout()

    out_png = Path(args.out) if args.out else csv_path.with_name(csv_path.stem + "_performance.png")
    plt.savefig(out_png, dpi=160)
    print(f"[OK] Figure enregistrée -> {out_png}")
    print(f"[OK] Données nettoyées   -> {clean_csv}")

    if not args.no_show:
        plt.show()

if __name__ == "__main__":
    main()
