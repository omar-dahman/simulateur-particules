# Simulateur de particules dans un environnement

Projet de programmation impérative en **C** (module PRIN12 — ENSIIE, 2ᵉ semestre).
Le programme simule le déplacement, les rebonds et les interactions de particules
dans un environnement 2D puis 3D, avec calcul des trajectoires, étude statistique
du système et rendu (console et images `.pbm`).

## Équipe

Projet réalisé en groupe (groupe 22) :

- **Omar Dahman**
- Othmane Chaoui
- Rayen Jalouali
- Nell Telechea
- Maxence Raymond

Encadrant : M. Dimitri Watel.

## Organisation du dépôt

Le travail est découpé en **lots** (A → I), chacun développé sur sa propre branche
Git. La branche `main` sert d'amorce ; le code et les rapports de chaque lot se
trouvent dans les branches correspondantes :

| Branche | Lot |
|---------|-----|
| `lot_a` | Amorce, distributions aléatoires, structures de base |
| `lot_b` | Génération et distributions des particules |
| `lot_c` | Physique des rebonds et étude probabiliste des trajectoires |
| `lot_d` | Simulation et rédaction d'article |
| `lot_e` | Passage en 3D (vecteurs) |
| `lot_f` | Extensions et article associé |
| `lot_g` | Extensions |
| `lot_h` | Rapport |
| `lot_i` | Réseau d'interactions par la théorie des graphes |

> Pour explorer un lot : `git checkout lot_c` (par exemple).

## Ma contribution (Omar Dahman)

Contributions réparties sur plusieurs lots :

- **Distributions aléatoires** — `distributions.h` / `distributions.c` (lots A et B).
- **Physique des rebonds** — réflexion vectorielle (mur vertical, mur incliné, coin)
  et **étude probabiliste des trajectoires** (marche aléatoire : espérance nulle,
  variance `E[|pₙ|²] = n`, comportement diffusif) — lot C.
- **Passage en 3D** — `vector.h` / `vector.c` et adaptation du code existant (lot E).
- **Théorie des graphes** — modélisation du réseau d'interactions entre particules
  (degré moyen, modèle d'Erdős–Rényi, rayon critique de connexité) — lot I.
- **Rédaction scientifique** — articles et rapports des lots D, F, H et I.

## Outils & technologies

- Langage **C**
- **Valgrind** (détection de fuites mémoire)
- **CUnit** (tests unitaires)
- **Doxygen** (documentation)
- **GanttProject** (gestion de projet)
- **Git** (travail collaboratif par branches)
