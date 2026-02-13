#ifndef PARTICULES_H
#define PARTICULES_H

/**
 * \file particules.h
 * 
 * Ce fichier permet de gérer chaque particule individuellement.
 * 5 fonctions sont définies:
 *  -\b create_particule permet d'allouer de la mémoire et de créer une particule.
 *  -\b free_particule permet de libérer la mémoire relative à une particule.
 *  -\b get_x permet d'obtenir l'abscisse de la particule.
 *  -\b get_y permet d'obtenir l'ordonnée de la particule.
 *  -\b get_vx permet d'obtenir la vitesse horizontale de la particule.
 *  -\b get_vy permet d'obtenir la vitesse verticale de la particule.
 *  -\b set_speed permet de définir la vitesse de la particule.
 *  -\b move permet de déplacer la particule.
 *  -\b distanceve permet d'obtenir la distance euclidienne entre les positions de deux particules.
 */


/**
 * \typedef particule
 * Type représentant une particule.
 */
typedef struct particule_s* particule;

/**
 * \brief Crée puis renvoie une particule positionnée en ( \a x, \a y) et dont la vitesse est un vecteur ( \a vx, \a vy).
 * Si la vitesse n'est pas unitaire, le vecteur est normalisé.
 * \param x position en abscisse de la particule.
 * \param y position en ordonnée de la particule.
 * \param vx vitesse horizontale de la particule.
 * \param vy vitesse verticale de la particule.
 * \return Une nouvelle particule positionnée en ( \a x, \a y ) et de vecteur vitesse ( \a vx, \a vy ).
 */
particule create_particule(float x, float y, float vx, float vy);

/**
 * \brief Libère la mémoire de la particule \a p.
 * \attention La particule p doit avoir été créée avec la fonction \a create_particule 
 * et ne doit pas déjà avoir été détruite avec la fonction \a free_particule.
 * \param p Une particule que l'on souhaite détruire.
 */
void free_particule(particule p);

/**
 * \brief Renvoie l'abscisse de la particule \a p.
 * \param p Une particule.
 * \return L'abscisse de la particule \a p .
 */
float get_x(particule p);

/**
 * \brief Renvoie l'ordonnée de la particule \a p.
 * \param p Une particule.
 * \return L'ordonnée de la particule \a p .
 */
float get_y(particule p);

/**
 * \brief Renvoie la vitesse horizontale de la particule \a p.
 * \param p Une particule.
 * \return La vitesse horizontale de la particule \a p .
 */
float get_vx(particule p);

/**
 * \brief Renvoie la vitesse verticale de la particule \a p.
 * \param p Une particule.
 * \return La vitesse verticale de la particule \a p .
 */
float get_vy(particule p);

/**
 * \brief Remplace la vitesse de la particule \a p par ( \a vx, \a vy ).
 * Si le vecteur n'est pas unitaire, il est normalisé.
 * \param p Une particule.
 * \param vx vitesse horizontale de la particule.
 * \param vy vitesse verticale de la particule.
 */
void set_speed(particule p, float vx, float vy);

/**
 * \brief Modifie la position de la particule 
 * comme si elle se déplaçait en ligne droite pendant \a dt secondes.
 * \param p Une particule.
 * \param dt Le temps du déplacement de la particule.
 */
void move(particule p, float dt);


/**
 * \brief Renvoie la distance euclidienne entre les positions de \a p1 et \a p2.
 * \param p1 Une particule.
 * \param p2 Une autre particule.
 * \return La distance euclidienne entre les positions des particules.
 */
float distanceve(particule p1, particule p2);
#endif