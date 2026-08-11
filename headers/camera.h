/**
 * \file    camera.h
 * \author  RAYEN JALOUALI
 * \brief   3D camera management
 * \details Defines the Camera type and all functions related to 3D camera
 *          handling, including predefined camera movements.
 *
 * \note    ADDED FOR LOT E (Tâche E.3) : entire file is new.
 *
 * \details The camera is defined by :
 *   - a position p (3D point)
 *   - a unit gaze vector c (where it looks)
 *   - a screen width w and height h (in pixels)
 *
 *   The camera can rotate around the vertical axis Oz (yaw) and around
 *   its horizontal axis orthogonal to c (pitch), but NOT around its gaze
 *   axis c (no roll). It always stays in landscape mode.
 *
 *   The projection screen is a w x h rectangle centered at p + c,
 *   with both axes orthogonal to c.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h" /* 3D vector library from Tâche E.2 */


typedef enum {
    CAMERA_MOVE_NONE = 0,  /**< no movement, static camera                */
    CAMERA_MOVE_TRANSLATE, /**< straight-line translation                  */
    CAMERA_MOVE_ORBIT,     /**< circular orbit around a target point       */
    CAMERA_MOVE_FLY,       /**< fly-through: moves while rotating toward a target */
    CAMERA_MOVE_PENDULUM   /**< oscillating pendulum swing around an axis  */
} CameraMoveType;


typedef struct {
    vec3  direction; /**< normalized displacement direction     */
    float speed;     /**< distance covered per iteration        */
} CameraParamsTranslate;

typedef struct {
    vec3  center;        /**< point to orbit around                 */
    float radius;        /**< orbit radius                          */
    float angular_speed; /**< angular speed (rad/iteration)         */
    float angle;         /**< current angle in rad (internal state) */
    float elevation;     /**< fixed elevation angle (rad)           */
} CameraParamsOrbit;


typedef struct {
    vec3  target;    /**< target point to fly toward                */
    float speed;     /**< distance covered per iteration            */
    float turn_rate; /**< rotation rate toward target (0 to 1)      */
} CameraParamsFly;


typedef struct {
    vec3  pivot;     /**< center of the swing                       */
    float radius;    /**< distance between pivot and camera         */
    float amplitude; /**< maximum swing amplitude (rad)             */
    float frequency; /**< swing frequency (rad/iteration)           */
    float phase;     /**< current phase in rad (internal state)     */
} CameraParamsPendulum;


typedef struct {
    vec3  position;  /**< camera position p in 3D space             */
    vec3  direction; /**< unit gaze vector c (always normalized)    */
    int   width;     /**< screen width w in pixels                  */
    int   height;    /**< screen height h in pixels                 */

    CameraMoveType move_type; /**< active movement type             */

    /** Movement parameters (active field depends on move_type) */
    union {
        CameraParamsTranslate translate;
        CameraParamsOrbit     orbit;
        CameraParamsFly       fly;
        CameraParamsPendulum  pendulum;
    } params;
} Camera;


Camera camera_create(vec3 position, vec3 direction, int width, int height);


void camera_set_move_none(Camera *cam);

/**
 * \brief   Configures a straight-line translation movement.
 * \param   cam       Camera to configure.
 * \param   direction Displacement direction (will be normalized).
 * \param   speed     Distance covered per iteration.
 */
void camera_set_move_translate(Camera *cam, vec3 direction, float speed);

/**
 * \brief   Configures an orbit movement around a point.
 * \details The camera circles around \a center at distance \a radius,
 *          at a fixed elevation angle \a elevation (rad).
 * \param   cam           Camera to configure.
 * \param   center        Point to orbit around.
 * \param   radius        Orbit radius.
 * \param   angular_speed Angular speed (rad/iteration).
 * \param   elevation     Fixed elevation angle (rad).
 */
void camera_set_move_orbit(Camera *cam, vec3 center, float radius,
                           float angular_speed, float elevation);

/**
 * \brief   Configures a fly-through movement toward a target.
 * \param   cam       Camera to configure.
 * \param   target    Target point.
 * \param   speed     Distance covered per iteration.
 * \param   turn_rate Rotation rate toward target (0 = none, 1 = instant).
 */
void camera_set_move_fly(Camera *cam, vec3 target, float speed, float turn_rate);

/**
 * \brief   Configures an oscillating pendulum movement.
 * \details The camera swings around \a pivot with the given amplitude.
 * \param   cam       Camera to configure.
 * \param   pivot     Center of the swing.
 * \param   radius    Distance from pivot to camera.
 * \param   amplitude Maximum swing amplitude (rad).
 * \param   frequency Swing frequency (rad/iteration).
 */
void camera_set_move_pendulum(Camera *cam, vec3 pivot, float radius,
                              float amplitude, float frequency);

/**
 * \brief   Applies one movement step to the camera.
 * \details Should be called at each iteration of the simulation.
 * \param   cam Camera to update.
 
 */
void camera_update(Camera *cam);

/**
 * \brief   Computes the two axes of the projection screen.
 * \details The screen is orthogonal to \a direction.
 *          - \a right : horizontal axis (orthogonal to c and Oz)
 *          - \a up    : vertical axis   (orthogonal to c and right)
 * \param   cam   Camera.
 * \param   right Output horizontal axis.
 * \param   up    Output vertical axis.
 */
void camera_get_screen_axes(const Camera *cam, vec3 *right, vec3 *up);

/**
 * \brief   Determines whether a particle is visible by the camera.
 * \details Tests whether the segment [camera_position, particle_pos]
 *          intersects the projection screen (w x h rectangle centered at p + c).
 *          If visible, returns the pixel coordinates of the intersection.
 * \param   cam          Camera.
 * \param   particle_pos 3D position of the particle.
 * \param   pixel_x      Output pixel column (valid only if return value is 1).
 * \param   pixel_y      Output pixel row    (valid only if return value is 1).
 * \return  1 if the particle is visible, 0 otherwise.
 */
int camera_is_particle_visible(const Camera *cam, vec3 particle_pos,
                               int *pixel_x, int *pixel_y);

/**
 * \brief   Rotates the camera around the vertical axis Oz (yaw).
 * \param   cam   Camera to rotate.
 * \param   angle Rotation angle in rad (positive = left).
 */
void camera_rotate_yaw(Camera *cam, float angle);


void camera_rotate_pitch(Camera *cam, float angle);


void camera_print(const Camera *cam);

#endif /* CAMERA_H */
