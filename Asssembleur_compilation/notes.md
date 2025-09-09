# Notes de Cours : Assembleur et Compilation

**Auteur :** ABDEL WEDOUD Yahdhih  
**Date :** 9 septembre 2025  
**Institution :** ENSIIE  
**Cours :** Assembleur et Compilation  

---

## Table des Matières

1. [Introduction](#1-introduction)
2. [Langage Machine](#2-langage-machine)
   2.1 [Jeux d’Instructions](#21-jeux-dinstructions)
   2.2 [Classification des Familles de Processeurs](#22-classification-des-familles-de-processeurs)
      2.2.1 [CISC vs. RISC](#221-cisc-vs-risc)
      2.2.2 [Accès aux Données](#222-accès-aux-données)
3. [Langage Assembleur](#3-langage-assembleur)
   3.1 [Différence entre Langage Assembleur et Langage Machine](#31-différence-entre-langage-assembleur-et-langage-machine)
   3.2 [Programme Assembleur](#32-programme-assembleur)
4. [Le Langage Assembleur RISC-V](#4-le-langage-assembleur-risc-v)
   4.1 [Instructions RV32IM](#41-instructions-rv32im)
   4.2 [Exemple de Code](#42-exemple-de-code)

---

## 1. Introduction

Un ordinateur, plus particulièrement son processeur, comprend les informations sous forme de signal électrique discret. En général, on note par 1 la présence d’une tension et par 0 son absence, ce qui représente un bit d’information. Le signal électrique fournit donc une séquence de bits. Pour fonctionner, il faut que ce signal électrique respecte le format attendu par le processeur. Pour chaque (famille de) processeur, il existe une spécification des instructions à exécuter en fonction des séquences de bits reçues en entrée : on parle de code machine ou de langage machine.

À l’aube de l’informatique, ces séquences de bits étaient explicitement écrites par le programmeur : instructions et données à traiter étaient codées en binaire pour être exécutées par la machine. Pour cela, on pouvait par exemple utiliser des cartes perforées : un trou représente un bit de valeur 1, l’absence de trou un bit de valeur 0.

Il s’est vite avéré que le travail d’écrire du code machine directement était fastidieux et prompt à causer des erreurs. En effet, pour un cerveau humain, il est assez difficile de retenir des codes binaires qui représentent les opérations à effectuer, alors qu’il est plus facile de manipuler des noms. Il est ainsi plus facile de savoir que SUB correspond à l’instruction pour effectuer une soustraction plutôt que le code binaire 010110. On utilise alors des mnémoniques, c’est-à-dire des noms, qui permettent de facilement se rappeler et manipuler les codes des instructions, les registres, les adresses, etc.

**Définition 1.1 (Langage assembleur).** Un langage assembleur est une représentation lisible du langage machine où les instructions, les adresses et les données peuvent être abstraites à l’aide de noms.

Le processeur n’est par contre pas capable d’exécuter un programme en langage assembleur directement : il ne peut comprendre que le code machine. Il faut donc traduire le programme en langage assembleur en code machine. Dès 1949, on a vu apparaître l’utilisation de noms pour désigner de façon facilement compréhensible les instructions. Néanmoins, la traduction vers le code machine se faisait alors à la main, ce qui, outre le fait que cette étape était longue et fastidieuse, pouvait entraîner des erreurs de traduction. Assez rapidement, cette tâche de traduction a donc été dévolue à un programme informatique : le programme assembleur.

**Définition 1.2 (Programme assembleur).** Un programme assembleur est un exécutable qui convertit un programme écrit en langage assembleur en du code machine.

Le premier programme assembleur a été écrit en 1954 pour l’IBM 701.

En général, on désigne par « assembleur » seul aussi bien le langage assembleur que le programme assembleur, en fonction du contexte. On remarquera que même écrit en langage assembleur, un programme a besoin d’être codé en binaire pour pouvoir être stocké, lu par la machine, etc. Les cartes perforées ont été largement utilisées jusque dans les années 1970 qui ont vu la démocratisation des supports de stockage magnétiques.

Contrairement à d’autres langages de programmation de plus haut niveau, le langage assembleur reste proche du langage machine. On peut même parfois lire qu’il existe une bijection entre les deux, ce qui n’est pas tout à fait exact. En effet, il est possible de renommer des variables, ou d’utiliser différentes façons pour indiquer des adresses, sans que le code machine final ne soit différent. À partir d’un code machine, on peut obtenir un programme en langage assembleur lui correspondant : on parle de désassemblage. Néanmoins, cette opération de désassemblage n’est pas unique : on peut faire correspondre plusieurs programmes en langage assembleur au même code machine.

Une des limites principales des langages assembleurs est qu’ils sont spécifiques à une machine (ou à une famille de machines) donnée. Dès les années 50 est apparu le besoin de langages de plus haut niveau que l’assembleur, de façon à abstraire certaines particularités propres à la machine (registres en nombre finis, instructions de branchement basiques, etc.) pour mieux se concentrer sur les aspects algorithmiques et pouvoir passer à l’échelle sur des projets plus complexes. Un des objectifs était également de pouvoir écrire un seul programme pour des architectures différentes. Pour pouvoir utiliser ces langages, il faut alors soit disposer d’un interpréteur, c’est-à-dire un exécutable qui va lire le programme et évaluer son contenu ; soit traduire le programme en code machine exécutable : on parle alors de compilation.

Ces langages se sont heurtés au scepticisme des programmeurs de l’époque qui avaient l’habitude de produire des codes assembleurs très efficaces et qui pensaient qu’il ne serait pas possible de produire automatiquement des exécutables aussi efficaces à partir de langages de haut niveau. Un bon compilateur ne peut donc se contenter de traduire naïvement le langage de haut niveau, il doit l’optimiser. Ces optimisations, associées au gain d’échelle offert par l’abstraction, ont permis la généralisation de l’utilisation de langages de haut niveau.

Un compilateur ne traduit pas forcément un langage en code machine, il peut produire du code dans un langage intermédiaire qui sera ensuite lui-même compilé (par exemple, le langage intermédiaire peut être du C) ou interprété (par exemple, du bytecode).

**Définition 1.3.** Un compilateur est un exécutable qui traduit un langage de haut niveau vers un langage de plus bas niveau.

La qualification de haut ou bas niveau pour un langage est subjective. Ainsi, C est un langage de haut niveau si on le compare à de l’assembleur, mais les compilateurs pour certains langages produisent du C (l’avantage étant qu’il existe ensuite des compilateurs de C vers de nombreuses architectures, ce qui évite de devoir écrire un compilateur pour chacune d’elle). Par la suite, on se contentera de parler de langage source et de langage cible.

**Exemple 1.1 :** Le premier compilateur optimisant, écrit en 1957, traduisait du Fortran en code machine pour l’IBM 704. Les compilateurs de Fortran sont toujours parmi les meilleurs à l’heure actuelle en termes d’optimisation. Ceci s’explique par la relative simplicité du langage, mais aussi par l’utilisation de Fortran pour le calcul scientifique qui a engendré le besoin d’obtenir du code très efficace.

Les compilateurs pour C produisent en général du code machine (exemple : gcc). Le compilateur Java de Sun produit du bytecode qui est ensuite interprété par une machine virtuelle, la JVM. OCaml dispose de deux compilateurs, ocamlc et ocamlopt, produisant respectivement du bytecode et du code machine.

Un exécutable qui génère du PDF à partir d’un autre langage (exemple : LaTeX, SVG, PostScript, etc.) est aussi un compilateur.

Un préprocesseur peut également être vu comme un compilateur du langage avec macros vers le langage pur.

Lex et Yacc sont aussi des compilateurs : ils traduisent des expressions régulières et des grammaires hors-contexte vers du code C. (Cf. l’acronyme de Yacc : Yet Another Compiler Compiler).

Dans la plupart des exemples de ce cours, on considérera comme langage source un sous-ensemble du langage Pascal qu’on appellera Pseudo-Pascal. Sa syntaxe est fournie en annexe A.4. Le langage cible sera quant à lui le langage assembleur RISC-V dont on donne en annexe A.3 les instructions les plus courantes.

En général, un compilateur ne se contente pas de traduire un langage dans un autre, il est capable de signaler des erreurs de syntaxe, de sémantique (par exemple via une vérification de type) si possible de façon compréhensible par l’utilisateur, il fait des optimisations qui peuvent viser plusieurs objectifs parfois contradictoires : vitesse d’exécution, taille du code, utilisation de la mémoire (notamment pour les applications embarquées), etc.

## 2. Langage Machine

Le langage assembleur est une abstraction du langage binaire directement compréhensible par le processeur, le langage machine. Hors exception, les processeurs traitent successivement des instructions qui leur sont spécifiques.

### 2.1 Jeux d’Instructions

**Définition 2.1 (Instruction).** Une instruction machine est une opération élémentaire (atomique) qu’un processeur peut effectuer.

Un processeur peut stocker des valeurs dans un nombre fixé de registres. S’il a besoin de plus de place, il doit utiliser la mémoire vive, mais l’accès à celle-ci a un coût non négligeable.

**Exemple 2.1 :** En général, un processeur possède des instructions pour faire :
- Des opérations arithmétiques ; celles-ci peuvent agir sur des entiers, ou sur des nombres à virgule flottante, il y a bien des instructions distinctes suivant le type de nombres (autrement dit, l’instruction d’addition sur les entiers est différente de l’instruction d’addition sur les nombres à virgule flottante) ;
- Des opérations logiques (“et”, “ou” mais aussi rotation, décalage, etc.)
- Des instructions de transfert, qui permettent de copier des données entre la mémoire vive de l’ordinateur et les registres ou la pile du processeur ;
- Des branchements, qui permettent de se déplacer dans le programme, et qui peuvent être conditionnels ou non.

**Définition 2.2 (Jeu d’instructions).** Un jeu d’instructions est l’ensemble des instructions qu’un processeur peut réaliser.

À l’origine, chaque nouvelle machine définissait son propre jeu d’instruction. Néanmoins, assez rapidement, on a cherché à pouvoir réutiliser les jeux d’instructions des anciens modèles, de façon à améliorer la réutilisabilité du code existant, de pouvoir le porter plus facilement entre différentes machines, et d’avoir des processeurs qui soient rétrocompatibles avec les générations précédentes. Par conséquent, certains jeux d’instructions sont communs à plusieurs processeurs, on parle alors de famille de processeurs.

**Exemple 2.2 :** Parmi les familles les plus courantes de processeurs, on peut citer :
- **x86** : Cette famille de processeurs est basée sur le jeu d’instruction du processeur Intel 8086. Ce jeu d’instruction a été petit à petit étendu pour ajouter de nouvelles fonctionnalités en dur dans le processeur ; par exemple, les processeurs actuels de cette famille possèdent en général des extensions SSE (qui permettent de travailler sur plusieurs données à la fois, ce qui est utile dans des applications de traitement du signal ou graphiques), ou encore l’extension x86-64 qui permet de travailler sur des données de 64 bits.
- **PowerPC** : Ce jeu d’instruction est surtout connu pour avoir été utilisé dans les processeurs équipant les ordinateurs de la marque Apple jusqu’en 2006 (quand Apple est passé à la famille x86). Il est aujourd’hui encore utilisé dans des systèmes embarqués (par exemple sur le rover Curiosity actuellement en activité sur Mars), ou dans des consoles de jeu (Wii, PlayStation 3, ...).
- **ARM** : Ce jeu d’instruction est particulièrement adapté pour les systèmes embarqués et mobiles, par exemple les smartphones. C’est par conséquent le jeu d’instructions le plus utilisé et celui dont la plus grande quantité de processeurs est produite.
- **MIPS** : Bien que conçu pour être généraliste, ce jeu d’instruction a surtout été utilisé dans des systèmes embarqués et des consoles de jeu (PlayStation 1 et 2, Nintendo 64).
- **RISC-V** : Ce jeu d’instruction a été développé par l’université de Californie à Berkeley en 2010 dans le but de fournir un standard ouvert servant de base à la conception de processeurs. Il n’y a en particulier pas de frais à payer pour son implémentation. Bien que développé initialement dans le secteur académique, avec une volonté de fédérer les différentes avancées dans un standard unique pour faire progresser plus rapidement la recherche sur les jeux d’instructions, il y a toujours eu l’idée de faire passer les bons choix de conception résultants dans l’industrie. Ce standard est de plus en plus adopté par cette dernière : outre de petites entreprises qui se sont construites autour de ce standard, comme SiFive, des grands groupes comme NVIDIA, Apple ou Huawei commencent à s’y intéresser de près. (Notamment, concernant Huawei, suite à l’interdiction américaine de l’utilisation d’Intel ou d’ARM.) Du fait de sa simplicité et de son potentiel, c’est principalement ce jeu que nous étudierons par la suite.

### 2.2 Classification des Familles de Processeurs

#### 2.2.1 CISC vs. RISC

Lors de la conception d’un processeur, on peut raisonnablement se poser la question suivante : vaut-il mieux avoir beaucoup d’instructions très spécifiques pour pouvoir répondre au besoin du code ? ou vaut-il mieux un petit ensemble d’instructions de base ?

On peut alors distinguer deux classes de familles de processeurs :
- **CISC (Complex Instruction Set Computer)** : Il s’agit de jeux d’instructions dans lesquels une instruction atomique peut réaliser plusieurs opérations à la fois, par exemple un chargement d’une donnée depuis la mémoire et une opération arithmétique sur cette donnée, ou bien un calcul sur plus de deux données à la fois. Un exemple typique de jeu d’instruction CISC est le x86.

Parmi les avantages du CISC, on trouve :
- Le code obtenu peut être plus compact puisqu’on peut utiliser une seule instruction pour effectuer plusieurs opérations (on obtient typiquement un facteur 1/2) ;
- Il est possible de fortement optimiser un code, en choisissant les instructions les plus adaptées aux calculs demandés.

Parmi ses inconvénients, on peut citer :
- Certaines des instructions complexes durent en fait plus d’un cycle d’horloge ; par exemple, l’instruction pour la division dans le Motorola 68000 a besoin de 160 cycles d’horloges ! On peut donc obtenir du code plus compact en taille, mais pas forcément plus rapide ;
- Les compilateurs ne produisent pas forcément les instructions les plus optimales pour un calcul donné ; c’est un problème assez complexe.

- **RISC (Reduced Instruction Set Computer)** : Il s’agit de jeux d’instructions relativement petits (en nombre d’instructions) avec des instructions simples et relativement générales. Parmi les jeux d’instructions RISC on trouve ARM, MIPS et PowerPC, et bien entendu RISC-V.

Le RISC a comme avantage d’être plus facile à concevoir, avec des processeurs contenant moins de transistors, ce qui fait qu’ils coûtent moins chers, consomment moins de courant et produisent par conséquent moins de chaleur. Cela explique leur quasi-omniprésence dans le monde embarqué et mobile. De plus, on peut compter sur les compilateurs pour essayer d’optimiser le code et avoir un nombre d’instructions finales assez réduit.

Au niveau matériel, la distinction est à l’heure actuelle moins claire : les processeurs correspondant à des jeux d’instruction CISC sont implémentés à l’aide d’instructions de base plutôt semblables à du RISC, et les instructions plus complexes sont définies en dur à l’aide de ces instructions.

#### 2.2.2 Accès aux Données

On peut aussi classifier les jeux d’instructions en fonction de la façon dont ils accèdent aux données, comment ils les chargent et les stockent. Nous allons illustrer ceci sur la traduction d’une addition C := A + B.

- **“0 adresse”** : Ce type d’instructions utilise une pile dans laquelle sont stockées toutes les données. L’addition est alors traduite par :
  ```
  PUSH A
  PUSH B
  ADD
  POP C
  ```

- **“à accumulateur”** : Ce type d’instruction utilise un seul registre fixe dans lequel sont stockés tous les calculs. Cela donne :
  ```
  LOAD A
  ADD B
  STORE C
  ```

- **“à registre-registre”** : Ce type d’instruction utilise plusieurs registres à donner en paramètre. Une valeur peut donc être chargée puis réutilisée plusieurs fois.
  ```
  LOAD R0, A
  LOAD R1, B
  ADD R2, R0, R1
  STORE R2, C
  ```

- **“à registre-mémoire”** : Ce type d’instruction utilise à la fois des registres et des adresses mémoire.
  ```
  LOAD R0, A
  ADD R1, R0, B
  STORE R1, C
  ```

Cela permet d’utiliser moins de registres, mais cela va à l’encontre de l’approche RISC.

Un jeu d’instruction peut en général avoir des instructions de différents types d’accès aux données.

**À noter :** x86 est à la fois accumulateur et à registre : la plupart des instructions mettent leur résultat dans le registre AX.

## 3. Langage Assembleur

### 3.1 Différence entre Langage Assembleur et Langage Machine

- Abstraction des opérateurs
- Abstraction des noms de registres
- Étiquettes symboliques pour les adresses
- Directives : ne sont pas traduites directement en code machine mais indiquent comment traduire la suite
  - `.asciiz "this is string"`
- Commentaires : tout ce qui suit un point-virgule est du commentaire
- Pseudo-instructions : des instructions qui n'existent pas en code machine. Elles sont traduites en une ou plusieurs vraies instructions machines
  - Exemple : on n'a pas besoin de toutes les comparaisons en langage machine, mais c'est utile de les avoir en langage assembleur.
  - `ret` : est une pseudo-instruction qui correspond au retour d'une fonction
    - `jalr zero, 0(ra)`
    - `jal ra, ma-fonction`
  - `mv rd, rs` : copie le contenu de rs dans rd ; cette pseudo-instruction est codée par `addi rd, rs, 0`
- Macros
  - Exemple : `.macro maFonction(arg)`
    ```
    add a0, arg, arg
    addi a0, a0, 1
    ```

### 3.2 Programme Assembleur

- Remplacer les macros par leur contenu
- Remplacer les pseudo-instructions par leurs traductions
- Remplacer les noms de registres par leurs numéros
- Remplacer les adresses symboliques par les véritables adresses (adresse absolue ou décalage suivant l'instruction)
- Remplacer les instructions assembleur par les codes machines correspondants

On produit alors un fichier objet (.o) qui contient le code machine, les données statiques, des informations sur l'utilisation du code, des informations sur l'utilisation de code, en particulier les fonctions exportées par le code.

## 4. Le Langage Assembleur RISC-V

Une version de RISC-V est identifiée par un sigle de la forme : RVnBACD avec B : nom de base et ACD : extension.

Par exemple : RV32IM c'est-à-dire 32 bits avec base sur les entiers et multiplications.

- **I** : Base sur les entiers, opérations courantes sur les entiers (+, -, décalage bit à bit, comparaisons ...) mais pas multiplication ni division
- Instructions de saut, branchement
- Lecture/écriture en mémoire
- Appels système
- **E** : Base pour les systèmes embarqués, essentiellement même instruction que I mais avec uniquement 16 registres
- **M** : Extension pour la multiplication, division et reste sur les entiers
- **C** : Extension avec instructions compressées, instructions qui tiennent sur 16 bits uniquement, permet d'avoir du code deux fois plus compact, en général utilisé avec E
- **F** : Extension pour les nombres à virgule flottante simple précision (32 bits) selon la norme IEEE 754-2008 (float en C), ajoute des registres pour les nombres à virgule flottante
- **D** : Nombres à virgule flottante double précision (64 bits, double en C) nécessite l'extension F
- **G** : Raccourci pour IMFDAZicsr_Zifencei ; minimum nécessaire pour pouvoir faire tourner un système d'exploitation généraliste

Dans ce cours, on se contentera de RV32IM.

### 4.1 Instructions RV32IM

**Opérations sur des entiers**
- `add rd, rs, rt` : ajoute le contenu de rt et rs et le place dans rd
- `addi rd, rs, i` : ajoute la constante i au contenu de rs et le place dans rd
- `sub rd, rs, rt` : soustraction (addi rd, rs, -i)
- `mul rd, rs, rt` : multiplication
- `div rd, rs, rt` : division
- Attention, pas d'instruction pour la multiplication par une constante
- `slli rd, rs, i` : on décale le contenu de rs de i bits à gauche, place le résultat dans rd
- `and rd, rs, rt` : et logique entre le contenu de rs et rt et place le résultat dans rd
- `li rd, i` : load immediate, charge la constante i dans le registre rd
- `mv rd, rs` : copie le contenu de rs dans rd

**Comparaisons**
- `slt rd, rs, rt` : "set less than" compare le contenu de rs et celui de rt, met 1 dans rd si rs est plus petit que rt et 0 sinon
- `slti rd, rs, i` : idem avec constante entière i
- `sle rd, rs, rt` : set less or equal
- `sgt rd, rs, rt` : set greater than
- `sge rd, rs, rt` : set greater or equal
- `seq rd, rs, rt` : set equal
- `sne rd, rs, rt` : set not equal

**Branchements et sauts**
- `j i` : saut à l'instruction i
- `jal i` : idem mais stocke l'adresse de la ligne suivante dans le registre ra
- `jr rs` : saute à l'instruction stockée à l'adresse dans rs
- `jalr rd, i(rs)` : plus généralement, saute à l'adresse du contenu de rs plus i, stocke dans rd l'adresse de l'instruction qui suit jalr
- `call f` : pseudo-instruction pour jal f
- `ret` : pseudo-instruction pour le retour de la fonction = jr ra
- `bgez rs, label` : continue à l'instruction étiquetée par label si le contenu de rs est plus grand ou égal à 0, sinon à l'instruction suivante
- `beq rs, rt, label` : branche si les contenus de rs et rt sont égaux
- `bgtz rs, label`
- `blez rs, label`
- `bltz rs, label`

**Gestion de la mémoire**
- `lw rd, i(rt)` : récupère le contenu de la mémoire à l'adresse contenue dans rt plus i, et le place dans rd
- `sw rs, i(rt)` : stocke le contenu de rs à l'adresse en mémoire contenue dans rt plus i (w : word -> 32 bits)
- `lb rd, i(rt)` : byte -> 8 bits
- `sb rs, i(rt)`

**Appels système**
- `ecall` : environment call, appel système

### 4.2 Exemple de Code

```risc-v
f11:
    addi sp, sp, -8
    sw ra, 4(sp)
    sw s0, 0(sp)
    mv s0, a0
    blez s0, f4
    addi a0, s0, -1
    call f11
    mul a0, s0, a0
    j f16

f16:
    lw ra, 4(sp)
    lw s0, 0(sp)
    addi sp, sp, 8
    ret

f4:
    li a0, 1
    j f16
```

---

*Ces notes sont destinées à un usage éducatif. Pour toute question ou correction, veuillez contacter l'auteur.*









