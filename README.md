# Particle Simulator in an Environment

🇬🇧 English | [🇫🇷 Français](README.fr.md)

Imperative programming project in **C** (PRIN12 course — ENSIIE, 2nd semester).
The program simulates the movement, bounces and interactions of particles
in a 2D and then 3D environment, with trajectory computation, statistical
analysis of the system and rendering (console output and `.pbm` images).

## Team

Group project (group 22):

- **Omar Dahman**
- Othmane Chaoui
- Rayen Jalouali
- Nell Telechea
- Maxence Raymond

Supervisor: Dimitri Watel.

## Repository structure

The work is split into **work packages** (*lots* A → I), each developed on its own
Git branch. The `main` branch contains the starter code; the code and reports for
each package are on the corresponding branches:

| Branch | Work package |
|--------|--------------|
| `lot_a` | Starter code, random distributions, core data structures |
| `lot_b` | Particle generation and distributions |
| `lot_c` | Bounce physics and probabilistic study of trajectories |
| `lot_d` | Simulation and scientific paper writing |
| `lot_e` | Moving to 3D (vectors) |
| `lot_f` | Extensions and related paper |
| `lot_g` | Extensions |
| `lot_h` | Report |
| `lot_i` | Interaction network using graph theory |

## Getting started

```bash
git clone https://github.com/omar-dahman/REPO-NAME.git
cd REPO-NAME
git checkout lot_i      # or any other work package
make
./simulation
```

## My contribution (Omar Dahman)

Contributions across several work packages:

- **Random distributions** — `distributions.h` / `distributions.c` (packages A and B).
- **Bounce physics** — vector reflection (vertical wall, sloped wall, corner)
  and **probabilistic study of trajectories** (random walk: zero expectation,
  variance `E[|pₙ|²] = n`, diffusive behavior) — package C.
- **Moving to 3D** — `vector.h` / `vector.c` and adaptation of the existing code (package E).
- **Graph theory** — modeling the interaction network between particles
  (average degree, Erdős–Rényi model, critical connectivity radius) — package I.
- **Scientific writing** — papers and reports for packages D, F, H and I.

## Tools & technologies

- **C** language
- **Valgrind** (memory leak detection)
- **CUnit** (unit testing)
- **Doxygen** (documentation)
- **GanttProject** (project management)
- **Git** (team collaboration with branches)
