# Notes de cours — Programmation scientifique

Ce module comporte deux projets:
- calcul des solutions de l’oscillateur harmonique quantique (OHQ) 1D et étude de certaines de leurs propriétés,
- calcul et représentation de la densité locale d’un système nucléaire.

Les codes seront écrits en C++11 et respecteront les normes de codage scientifique.

---

## Exigences du projet (rappels)
- Un fichier `AUTHORS` doit contenir votre/vos nom(s)
- Le code source doit être en C++11 et documenté avec Doxygen
- La chaîne de compilation utilise GNU Make
- La présentation est livrée à l’emplacement `pres/index.html`
- La présentation doit utiliser remark.js
- L’usage de branches est autorisé, mais seule la branche `master` sera évaluée

---

## Rappel très bref de la théorie quantique
- Interférences (fentes de Young, 1803) → nature ondulatoire de la lumière
- Problème du rayonnement du corps noir (~1890) → quantification (Planck, 1900), énergie d’un oscillateur: $E = n h f$, $n\in\mathbb{N}^*$
- Photon (Einstein, 1905): $E = h f$
- Modèle de Bohr (1913), couches fermées (1922), spin (Stern–Gerlach, 1922)
- Ondes stationnaires (de Broglie, 1924), principe d’exclusion de Pauli (1925)
- Naissance de la MQ moderne (Heisenberg, Schrödinger, Born ~1925)

---

## Oscillateur harmonique quantique (1D)

### Équation de Schrödinger stationnaire
$$
\hat H_{(z)}\,\psi_n(z) = E_n\,\psi_n(z)
$$
avec
$$
\hat H_{(z)} \equiv \frac{\hat p_{(z)}^2}{2m} + \tfrac{1}{2} m\,\omega^2 z^2,\qquad
\hat p_{(z)} \equiv -\,i\,\hbar\, \frac{\partial}{\partial z}.
$$
Les niveaux d’énergie sont
$$
E_n = \hbar\,\omega\left(n + \tfrac{1}{2}\right),\qquad n=0,1,2,\dots
$$

![Niveaux d’énergie et fonctions propres de l’OHQ 1D](<énergie et solution de léquation de Schrondinger.png>)

# Polynômes d’Hermite (version « physiciens »)

**Définition**

$$
\forall n \ge 0,\quad
H_n(z) \equiv (-1)^n\, e^{z^2}\,\frac{\mathrm d^n}{\mathrm d z^n}\!\left(e^{-z^2}\right).
$$

**Relation de récurrence**

$$
H_0(z)=1,\qquad H_1(z)=2z,\qquad
H_{n+1}(z)=2z\,H_n(z)-2n\,H_{n-1}(z),\quad (n\ge 1).
$$

---

## Unités et constantes (mode « physique »)

### Constantes SI 2019
- $h = 6.62607015\times10^{-34}$ J·s
- $c = 2.99792458\times10^{8}$ m·s $^{-1}$
- $e = 1.602176634\times10^{-19}$ C

### Exemples de conversions
```
1 [J]  = 1 [C] * 1 [V]
1 [eV] = 1.602176634e-19 [J]
1      = 1.0e+06 [eV/MeV]
1      = 1.0e+15 [fs/s]
```

### Valeurs utiles de \hbar selon les unités de travail
- Temps en s, énergie en J:  $\hbar = h/(2\pi) \approx 1.054571817\times10^{-34}$ J·s
- Temps en s, énergie en eV: $\hbar \approx 6.582119569\times10^{-16}$ eV·s
- Temps en fs, énergie en MeV: $\hbar \approx 6.582119569\times10^{-7}$ MeV·fs

### Masses (en MeV/ $c^2$)
- Neutron:  $m_n = 939.56542052$
- Électron: $m_e = 0.510998950$

Bonnes pratiques
1. Choisir le système étudié (neutron dans un potentiel nucléaire, électron dans un potentiel atomique, …)
2. Choisir les unités de travail (ex.: temps en fs, distance en fm, énergie en MeV)
3. Utiliser ces unités de façon cohérente dans toutes les sorties (présentations, figures, tableaux, …)

---

## Travail à réaliser
- Implémenter le calcul des solutions de l’OHQ 1D le long de l’axe $z$ (via la récurrence des $H_n$)
- Évaluer et tracer les premières solutions (xmgrace, matplotlib, …)
- Vérifier numériquement les énergies $E_n$ (par exemple via $\hat H\psi = E\psi$ et/ou valeurs moyennes)
- Vérifier numériquement l’orthonormalité des solutions (quadrature)
- Présenter les résultats en utilisant le modèle remark.js

### Bonus
- Projet et présentation en unités SI
- Avertissements absents à la compilation, au liage, aux tests et à la doc
- Méthodes alternatives également implémentées
- Code clair et bien commenté; commits petits et explicites; tests unitaires passants

---

## Présentation
Format
- Durée: ~10 min
- Questions: ~10 min
- Support: remark.js (`pres/index.html`)

Contenu attendu
- Structure du projet (choix de classes, hiérarchie des fichiers, …)
- Chaîne de compilation (contenu du Makefile)
- Documentation générée (Doxygen)
- Extraits de code et quelques tests unitaires
- Historique Git (messages de commit) et résultats numériques

---

## Pour dépôt Git
- S’assurer que `AUTHORS`, `README.md`, `LICENSE`, `Makefile` et la doc Doxygen sont présents
- Exécuter les tests et la génération de la doc avant commit
- Utiliser des messages de commit explicites (impératif court)