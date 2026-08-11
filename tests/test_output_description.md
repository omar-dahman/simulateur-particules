```c
/**
 * \file    test_output_description.c
 * \author  Othmane CHAOUI
 * \date    2026-04-23
 *
 * \brief   Description of output results
 * \details This file contains the description of output
 *          results
 */
```

# Tests descriptifs - Interface graphique

---

## Tests Tâche E.3 - Mouvement de caméra

### Test 1 : Translation de la caméra

**Protocole :** 
  - Particules : n = 1
  - Environnement : w = 100, h = 100, d = 100
  - r = 1, dt = 0.1, itérations = 10
  - Caméra position initiale : (0, 0, 0)
  - Caméra direction : (1, 0, 0)
  - Type mouvement : translation (1)
  - Direction translation : (1, 0, 0)
  - Vitesse translation : 2.0
  - Mode : console (export_img = 0)

**Résultat attendu :** 
  - `camera_print()` affiche position : (0,0,0) → (2,0,0) → (4,0,0) → ... → (18,0,0)
  - Direction reste (1, 0, 0) constante
  - Norme du vecteur direction = 1

**Résultat obtenu :** 
  - *passed* : Position correcte à chaque itération
  - *passed* : Direction inchangée
  - *passed* : Norme = 1

---

### Test 2 : Mouvement orbital

**Protocole :** 
  - Particules : n = 1
  - Environnement : w = 100, h = 100, d = 100
  - r = 1, dt = 0.1, itérations = 50
  - Caméra position initiale : (50, 0, 100)
  - Caméra direction initiale : (0, 0, -1)
  - Type mouvement : orbit (2)
  - Centre orbite : (50, 50, 50)
  - Rayon orbite : 60
  - Vitesse angulaire : 0.05 rad/itération
  - Élévation : 0.3 rad
  - Mode : console

**Résultat attendu :** 
  - La caméra tourne autour du centre
  - La distance au centre reste constante (≈60)
  - La direction pointe toujours vers le centre
  - L'angle augmente progressivement

**Résultat obtenu :** 
  - *passed* : Position circulaire
  - *passed* : Distance au centre ≈60
  - *passed* : Direction vers le centre

---

### Test 3 : Mouvement Fly (vol vers une cible)

**Protocole :** 
  - Particules : n = 1
  - Environnement : w = 100, h = 100, d = 100
  - r = 1, dt = 0.1, itérations = 20
  - Caméra position initiale : (0, 0, 0)
  - Caméra direction initiale : (1, 0, 0)
  - Type mouvement : fly (3)
  - Cible : (100, 0, 0)
  - Vitesse de vol : 5.0
  - Taux de rotation : 0.1
  - Mode : console

**Résultat attendu :** 
  - La caméra se rapproche de la cible (x augmente)
  - La direction s'aligne progressivement sur (1,0,0)
  - Position finale proche de (100,0,0)

**Résultat obtenu :** 
  - *passed* : Position (0,0,0) → (≈95,0,0)
  - *passed* : Direction tend vers (1,0,0)

---

### Test 4 : Mouvement Pendulum (balancement)

**Protocole :** 
  - Particules : n = 1
  - Environnement : w = 100, h = 100, d = 100
  - r = 1, dt = 0.1, itérations = 100
  - Caméra position initiale : (10, 0, 0)
  - Caméra direction initiale : (-1, 0, 0)
  - Type mouvement : pendulum (4)
  - Pivot : (0, 0, 0)
  - Rayon : 10
  - Amplitude : π/4 = 0.785 rad
  - Fréquence : 0.1 rad/itération
  - Mode : console

**Résultat attendu :** 
  - La caméra oscille autour du pivot
  - La direction pointe toujours vers le pivot
  - Position x oscille entre ≈ -7 et ≈ 7

**Résultat obtenu :** 
  - *passed* : Oscillation visible
  - *passed* : Direction centrée sur pivot

---

### Test 5 : Caméra statique (move_type = 0)

**Protocole :** 
  - Particules : n = 1
  - Environnement : w = 100, h = 100, d = 100
  - r = 1, dt = 0.1, itérations = 10
  - Caméra position : (50, 50, 100)
  - Caméra direction : (0, 0, -1)
  - Type mouvement : static (0)
  - Mode : console

**Résultat attendu :** 
  - Position reste (50, 50, 100) à toutes les itérations
  - Direction reste (0, 0, -1)

**Résultat obtenu :** 
  - *passed* : Position constante
  - *passed* : Direction constante

---

## Tests Tâche E.4 - Vision caméra

---

### Test 1 : Particule visible au centre (render_3d)

**Protocole :** 
  - Environnement : w = 100, h = 100, d = 100
  - Particules : n = 1 
  - r = 1, dt = 0.1, itérations = 1
  - Caméra position : (50, 50, 100)
  - Caméra direction : (0, 0, -1) (regarde vers l'origine)
  - Type mouvement : static (0)
  - Export images : oui (1)
  - Largeur image : 500 px
  - Hauteur image : 500 px

**Résultat attendu :** 
  - Pixel noir au centre de l'image (coordonnées ≈ 250, 250)

**Résultat obtenu :** 
  - *passed* : Pixel noir visible à (250, 250)

---

### Test 2 : Particule décalée (en déplaçant la caméra)

**Protocole :** 
  - Environnement : w = 100, h = 100, d = 100
  - Particules : n = 1 
  - r = 1, dt = 0.1, itérations = 1
  - Caméra position : (70, 50, 100) (décalée sur X pour simuler un décalage relatif)
  - Caméra direction : (0, 0, -1)
  - Type mouvement : static (0)
  - Export images : oui (1)
  - Largeur image : 500 px
  - Hauteur image : 500 px

**Résultat attendu :** 
  - La particule est visible mais décalée sur l'image
  - Effet équivalent à une particule décalée dans l'environnement

**Résultat obtenu :** 
  - *failed :* particule reste au centre

---

### Test 3 : Particule hors champ (en déplaçant la caméra très loin)

**Protocole :** 
  - Environnement : w = 100, h = 100, d = 100
  - Particules : n = 1 
  - r = 1, dt = 0.1, itérations = 1
  - Caméra position : (50, 500, 100) (caméra très haute → hors champ)
  - Caméra direction : (0, -1, 0) (regarde vers le bas)
  - Type mouvement : static (0)
  - Export images : oui (1)
  - Largeur image : 500 px
  - Hauteur image : 500 px

**Résultat attendu :** 
  - La particule n'est pas visible (hors champ vertical)
  - Image entièrement blanche

**Résultat obtenu :** 
- *failed :* particule au milieu

---

### Test 4 : Particule derrière la caméra

**Protocole :** 
  - Environnement : w = 100, h = 100, d = 100
  - Particules : n = 1 
  - r = 1, dt = 0.1, itérations = 1
  - Caméra position : (50, 50, 50)
  - Caméra direction : (1, 0, 0) (regarde vers la droite)
  - Type mouvement : static (0)
  - Export images : oui (1)
  - Largeur image : 500 px
  - Hauteur image : 500 px

**Résultat attendu :** 
  - La particule (au centre) n'est pas sur le trajet de la caméra
  - Elle est derrière ou hors champ → invisible

**Résultat obtenu :** 
  - *passed* : Non visible

---

### Test 5 : Animation avec caméra orbitale

**Protocole :** 
  - Environnement : w = 100, h = 100, d = 100
  - Particules : n = 1 
  - r = 1, dt = 0.1, itérations = 50
  - Caméra position initiale : (50, 0, 100)
  - Caméra direction initiale : (0, 0, -1)
  - Type mouvement : orbit (2)
  - Centre orbite : (50, 50, 50)
  - Rayon orbite : 60
  - Vitesse angulaire : 0.05 rad/itération
  - Élévation : 0.3 rad
  - Export images : oui (1)
  - Largeur image : 500 px
  - Hauteur image : 500 px

**Résultat attendu :** 
  - La particule (au centre) est visible sur toutes les images
  - Sa position sur l'image varie légèrement
  - Fichiers `output/env-0.pbm` à `output/env-49.pbm` générés

**Résultat obtenu :** 
  - *passed* : 50 fichiers générés
  - *passed* : Particule toujours visible
  - *passed* : Variation de position

---

## Récapitulatif des résultats

| Test | Statut |
|------|--------|
| E.3.1 | *passed* |
| E.3.2 | *passed* |
| E.3.3 | *passed* |
| E.3.4 | *passed* |
| E.3.5 | *passed* |
| E.4.1 | *passed* |
| E.4.2 | *failed* |
| E.4.3 | *failed* |
| E.4.4 | *passed* |
| E.4.5 | *passed* |
