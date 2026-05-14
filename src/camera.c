/**
 * \file    camera.c
 * \author  RAYEN JALOUALI
 * \brief   3D camera management implementation
 * \details Implements all functions declared in camera.h.
 *
 * \note    ADDED FOR LOT E (Tâche E.3) : entire file is new.
 */

#include <math.h>
#include <stdio.h>
#include "camera.h"
#include "vector.h"


/* Global up vector: vertical axis is Oz */
static const vec3 WORLD_UP = {0.0f, 0.0f, 1.0f};

/* Threshold to avoid division by zero */
#define CAM_EPS 1e-6f


static void normalize_dir(Camera *cam) {
    float n = vec3_norm(cam->direction);
    if (n > CAM_EPS)
        cam->direction = vec3_scale(cam->direction, 1.0f / n);
}


static vec3 right_axis(const Camera *cam) {
    vec3  right = vec3_cross(cam->direction, WORLD_UP);
    float n     = vec3_norm(right);

    /* Degenerate case: direction parallel to Oz, use Ox as fallback */
    if (n < CAM_EPS)
        return vec3_make(1.0f, 0.0f, 0.0f);

    return vec3_scale(right, 1.0f / n);
}


static vec3 up_axis(vec3 right, vec3 direction) {
    return vec3_cross(right, direction);
}

Camera camera_create(vec3 position, vec3 direction, int width, int height) {
    Camera cam;

    cam.position  = position;
    cam.direction = direction;
    cam.width     = width;
    cam.height    = height;
    cam.move_type = CAMERA_MOVE_NONE;

    /* Ensure the gaze vector is unit length */
    normalize_dir(&cam);

    return cam;
}

void camera_set_move_none(Camera *cam) {
    cam->move_type = CAMERA_MOVE_NONE;
}

void camera_set_move_translate(Camera *cam, vec3 direction, float speed) {
    float n;

    cam->move_type = CAMERA_MOVE_TRANSLATE;

    /* Normalize the displacement direction */
    n = vec3_norm(direction);
    if (n > CAM_EPS)
        cam->params.translate.direction = vec3_scale(direction, 1.0f / n);
    else
        cam->params.translate.direction = vec3_make(0.0f, 0.0f, 0.0f);

    cam->params.translate.speed = speed;
}

void camera_set_move_orbit(Camera *cam, vec3 center, float radius,
                           float angular_speed, float elevation) {
    float cos_el, sin_el, n;
    vec3  offset, to_center;

    cam->move_type = CAMERA_MOVE_ORBIT;

    cam->params.orbit.center        = center;
    cam->params.orbit.radius        = radius;
    cam->params.orbit.angular_speed = angular_speed;
    cam->params.orbit.elevation     = elevation;
    cam->params.orbit.angle         = 0.0f;

    /* Place camera on the orbit immediately */
    cos_el = cosf(elevation);
    sin_el = sinf(elevation);
    offset = vec3_make(radius * cos_el, 0.0f, radius * sin_el);
    cam->position = vec3_add(center, offset);

    /* Camera looks toward the center */
    to_center = vec3_sub(center, cam->position);
    n = vec3_norm(to_center);
    if (n > CAM_EPS)
        cam->direction = vec3_scale(to_center, 1.0f / n);
}

void camera_set_move_fly(Camera *cam, vec3 target, float speed, float turn_rate) {
    cam->move_type            = CAMERA_MOVE_FLY;
    cam->params.fly.target    = target;
    cam->params.fly.speed     = speed;
    cam->params.fly.turn_rate = turn_rate;
}

void camera_set_move_pendulum(Camera *cam, vec3 pivot, float radius,
                              float amplitude, float frequency) {
    cam->move_type                 = CAMERA_MOVE_PENDULUM;
    cam->params.pendulum.pivot     = pivot;
    cam->params.pendulum.radius    = radius;
    cam->params.pendulum.amplitude = amplitude;
    cam->params.pendulum.frequency = frequency;
    cam->params.pendulum.phase     = 0.0f;
}

/* ADDED FOR LOT E (Tâche E.3) : new function */
void camera_update(Camera *cam) {
    float n, dist, step_dist, angle, cos_el, sin_el;
    vec3  step, offset, to_center, to_target, target_dir, new_dir, to_pivot;

    switch (cam->move_type) {

        case CAMERA_MOVE_NONE:
            /* Static camera, nothing to do */
            break;

        case CAMERA_MOVE_TRANSLATE:
            /* Move position along direction at the given speed */
            step = vec3_scale(cam->params.translate.direction,
                              cam->params.translate.speed);
            cam->position = vec3_add(cam->position, step);
            break;

        case CAMERA_MOVE_ORBIT:
            /* Advance the orbit angle */
            cam->params.orbit.angle += cam->params.orbit.angular_speed;
            angle  = cam->params.orbit.angle;
            cos_el = cosf(cam->params.orbit.elevation);
            sin_el = sinf(cam->params.orbit.elevation);

            /* New position on the orbit circle */
            offset = vec3_make(
                cam->params.orbit.radius * cos_el * cosf(angle),
                cam->params.orbit.radius * cos_el * sinf(angle),
                cam->params.orbit.radius * sin_el
            );
            cam->position = vec3_add(cam->params.orbit.center, offset);

            /* Camera always looks toward the center */
            to_center = vec3_sub(cam->params.orbit.center, cam->position);
            n = vec3_norm(to_center);
            if (n > CAM_EPS)
                cam->direction = vec3_scale(to_center, 1.0f / n);
            break;

        case CAMERA_MOVE_FLY:
            to_target = vec3_sub(cam->params.fly.target, cam->position);
            dist      = vec3_norm(to_target);

            if (dist > CAM_EPS) {
                target_dir = vec3_scale(to_target, 1.0f / dist);

                /* Linear interpolation of gaze direction toward the target */
                new_dir = vec3_make(
                    cam->direction.x + cam->params.fly.turn_rate *
                        (target_dir.x - cam->direction.x),
                    cam->direction.y + cam->params.fly.turn_rate *
                        (target_dir.y - cam->direction.y),
                    cam->direction.z + cam->params.fly.turn_rate *
                        (target_dir.z - cam->direction.z)
                );
                n = vec3_norm(new_dir);
                if (n > CAM_EPS)
                    cam->direction = vec3_scale(new_dir, 1.0f / n);

                /* Advance along the current gaze direction */
                step_dist = (dist < cam->params.fly.speed)
                            ? dist
                            : cam->params.fly.speed;
                step = vec3_scale(cam->direction, step_dist);
                cam->position = vec3_add(cam->position, step);
            }
            break;

        case CAMERA_MOVE_PENDULUM:
            /* Advance phase by one frequency step */
            cam->params.pendulum.phase += cam->params.pendulum.frequency;
            angle = cam->params.pendulum.amplitude *
                    sinf(cam->params.pendulum.phase);

            /* Position swings in the XY plane around the pivot */
            cam->position.x = cam->params.pendulum.pivot.x +
                               cam->params.pendulum.radius * sinf(angle);
            cam->position.y = cam->params.pendulum.pivot.y -
                               cam->params.pendulum.radius * cosf(angle);
            cam->position.z = cam->params.pendulum.pivot.z;

            /* Camera looks toward the pivot */
            to_pivot = vec3_sub(cam->params.pendulum.pivot, cam->position);
            n = vec3_norm(to_pivot);
            if (n > CAM_EPS)
                cam->direction = vec3_scale(to_pivot, 1.0f / n);
            break;
    }
}

/* ADDED FOR LOT E (Tâche E.3) : new function */
void camera_get_screen_axes(const Camera *cam, vec3 *right, vec3 *up) {
    *right = right_axis(cam);
    *up    = up_axis(*right, cam->direction);
}


int camera_is_particle_visible(const Camera *cam, vec3 particle_pos,
                               int *pixel_x, int *pixel_y) {
    float t, u, v, half_w, half_h, ray_dot_dir;
    vec3  ray, screen_center, intersection, diff, right, up;

    /* Vector from camera to particle */
    ray = vec3_sub(particle_pos, cam->position);

    /* Projection onto the gaze direction */
    ray_dot_dir = vec3_dot(ray, cam->direction);

    /* Particle is behind or exactly on the camera: not visible */
    if (ray_dot_dir <= CAM_EPS)
        return 0;

    /* Parameter t of the intersection with the screen plane */
    t = 1.0f / ray_dot_dir; /* direction is unit length */

    /* t must be in ]0, 1]: intersection between camera and particle */
    if (t <= 0.0f || t > 1.0f)
        return 0;

    /* Screen center: p + c */
    screen_center = vec3_add(cam->position, cam->direction);

    /* Intersection point with the screen plane */
    intersection = vec3_make(
        cam->position.x + t * ray.x,
        cam->position.y + t * ray.y,
        cam->position.z + t * ray.z
    );

    /* Coordinates in the screen basis centered at screen_center */
    diff = vec3_sub(intersection, screen_center);
    camera_get_screen_axes(cam, &right, &up);

    u = vec3_dot(diff, right); /* horizontal coordinate */
    v = vec3_dot(diff, up);    /* vertical coordinate   */

    /* Check that the intersection lies within the screen rectangle */
    half_w = cam->width  / 2.0f;
    half_h = cam->height / 2.0f;

    if (u < -half_w || u > half_w || v < -half_h || v > half_h)
        return 0;

    /* Convert to pixel coordinates (top-left corner = (0, 0)) */
    *pixel_x = (int)(u + half_w);
    *pixel_y = (int)(half_h - v);

    /* Safety clamp to stay within image bounds */
    if (*pixel_x < 0)            *pixel_x = 0;
    if (*pixel_x >= cam->width)  *pixel_x = cam->width  - 1;
    if (*pixel_y < 0)            *pixel_y = 0;
    if (*pixel_y >= cam->height) *pixel_y = cam->height - 1;

    return 1;
}



void camera_rotate_yaw(Camera *cam, float angle) {
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    float new_x = cos_a * cam->direction.x - sin_a * cam->direction.y;
    float new_y = sin_a * cam->direction.x + cos_a * cam->direction.y;

    cam->direction.x = new_x;
    cam->direction.y = new_y;
    

    normalize_dir(cam);
}


void camera_rotate_pitch(Camera *cam, float angle) {
    vec3  right = right_axis(cam);
    vec3  cross = vec3_cross(right, cam->direction);
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);

    cam->direction.x = cos_a * cam->direction.x + sin_a * cross.x;
    cam->direction.y = cos_a * cam->direction.y + sin_a * cross.y;
    cam->direction.z = cos_a * cam->direction.z + sin_a * cross.z;

    normalize_dir(cam);
}

void camera_print(const Camera *cam)
{
    static const char *move_names[] = {
        "NONE", "TRANSLATE", "ORBIT", "FLY", "PENDULUM"};

    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                         CAMERA STATE                             ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");

    /* Position */
    printf("📍 Position     : (%.3f, %.3f, %.3f)\n",
           cam->position.x, cam->position.y, cam->position.z);

    /* Direction */
    float norm = vec3_norm(cam->direction);
    printf("🎯 Direction   : (%.3f, %.3f, %.3f)  [norme = %.6f]",
           cam->direction.x, cam->direction.y, cam->direction.z, norm);
    if (fabs(norm - 1.0f) > 1e-5f)
    {
        printf(" ⚠️  ERREUR : vecteur non unitaire !");
    }
    printf("\n");

    /* Angle par rapport à l'axe X (pour le debug) */
    float angle_x = acosf(fabs(cam->direction.x)) * 180.0f / M_PI;
    float angle_y = acosf(fabs(cam->direction.y)) * 180.0f / M_PI;
    float angle_z = acosf(fabs(cam->direction.z)) * 180.0f / M_PI;
    printf("📐 Angles      : X=%.1f° , Y=%.1f° , Z=%.1f°\n", angle_x, angle_y, angle_z);

    /* Écran */
    printf("🖥️  Écran       : %d x %d pixels\n", cam->width, cam->height);

    /* Type de mouvement */
    printf("🎬 Mouvement   : %s", move_names[cam->move_type]);

    /* Paramètres spécifiques selon le type de mouvement */
    switch (cam->move_type)
    {
    case CAMERA_MOVE_TRANSLATE:
        printf("\n   └─ Direction : (%.3f, %.3f, %.3f), Vitesse : %.3f",
               cam->params.translate.direction.x,
               cam->params.translate.direction.y,
               cam->params.translate.direction.z,
               cam->params.translate.speed);
        break;

    case CAMERA_MOVE_ORBIT:
        printf("\n   └─ Centre : (%.3f, %.3f, %.3f), Rayon : %.3f",
               cam->params.orbit.center.x,
               cam->params.orbit.center.y,
               cam->params.orbit.center.z,
               cam->params.orbit.radius);
        printf("\n      └─ Vitesse angulaire : %.3f rad/it, Élévation : %.3f rad, Angle courant : %.3f rad",
               cam->params.orbit.angular_speed,
               cam->params.orbit.elevation,
               cam->params.orbit.angle);
        break;

    case CAMERA_MOVE_FLY:
        printf("\n   └─ Cible : (%.3f, %.3f, %.3f), Vitesse : %.3f, Taux rotation : %.3f",
               cam->params.fly.target.x,
               cam->params.fly.target.y,
               cam->params.fly.target.z,
               cam->params.fly.speed,
               cam->params.fly.turn_rate);
        break;

    case CAMERA_MOVE_PENDULUM:
        printf("\n   └─ Pivot : (%.3f, %.3f, %.3f), Rayon : %.3f",
               cam->params.pendulum.pivot.x,
               cam->params.pendulum.pivot.y,
               cam->params.pendulum.pivot.z,
               cam->params.pendulum.radius);
        printf("\n      └─ Amplitude : %.3f rad, Fréquence : %.3f rad/it, Phase : %.3f rad",
               cam->params.pendulum.amplitude,
               cam->params.pendulum.frequency,
               cam->params.pendulum.phase);
        break;

    default:
        break;
    }
    printf("\n");

    /* Calcul de la distance à l'origine (pour debug) */
    float dist_to_origin = vec3_norm(cam->position);
    printf("📏 Distance origine : %.3f\n", dist_to_origin);

    /* Ligne de séparation */
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
}
