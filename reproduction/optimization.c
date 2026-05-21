#include "optimization.h"

// Tâche G.2 : début des modifications
#include "environnement.h"
#include "particules.h"
#include "math.h"

void set_particule(env e, int i, float x, float y, float vx, float vy) {
    particule p = get_particule(e, i);
    set_position(p, x, y);
    set_speed(p, vx, vy);
}
// Tâche G.2 : fin des modifications

// converts the distance along the losange to the real x,y
// coordinates. returns n € {0,1,2,3} representing in which
// side of the rectangle the particle is in.
int get_pos(float pos, float w, float h, float* x, float* y) {
    // represents 1/4th the size of the inscribed losange
    float space_unit = sqrtf(w*w + h*h) / 2.0f;
    int n = (int)floorf(pos / space_unit);
    float d = fmodf(pos, space_unit);

    // according to thales' theorem, y/x = h/w
    // and we know from the pythagorean theorem : y**2 + x**2 = d**2
    // hence x = w * d / sqrt(h**2 + w**2) and y = h * d / sqrt(h**2 + w**2) 
    // Moreover, 2.0f * space_unit is mathematically identical to sqrtf(w*w + h*h)
    float int_x = (w * d) / (2.0f * space_unit);
    float int_y = (h * d) / (2.0f * space_unit);

    if (n == 0) {
        *x = int_x;
        *y = h/2.0f - int_y;
    } else if (n == 1) {
        *x = w/2.0f + int_x;
        *y = int_y;
    } else if (n == 2) {
        *x = w - int_x;
        *y = h/2.0f + int_y;
    } else { // n == 3
        *x = w/2.0f - int_x;
        *y = h - int_y;
    }

    return n;
}

// unnormalized speed
void get_speed(int n, float w, float h, float* vx, float* vy) {
    if (n == 0) {
        *vx = w;
        *vy = -h;
    } else if (n == 1) {
        *vx = w;
        *vy = h;
    } else if (n == 2) {
        *vx = -w;
        *vy = h;
    } else if (n == 3) {
        *vx = -w;
        *vy = -h;
    }
}

float eval_E_sq(float x, float D, float cos_alpha, int is_obtus) {
    float sign = is_obtus ? 1.0 : -1.0;
    return x*x + (D-x)*(D-x) + sign * 2.0 * x * (D-x) * cos_alpha;
}

float dichotomy(float min, float max, int iter, float D, float w, float h) {
    float cos_alpha = (w*w - h*h) / (w*w + h*h);
    float phi = 0.0;

    for (int i = 0; i < iter; i++) {
        phi = (min + max) / 2.0;
        
        float x_aigu = (D / 2.0) - phi;
        float x_obtus = (D / 4.0) + phi;

        float E_aigu_sq = eval_E_sq(x_aigu, D, cos_alpha, 0);
        float E_obtus_sq = eval_E_sq(x_obtus, D, cos_alpha, 1);

        if (E_aigu_sq < E_obtus_sq) {
            min = phi; 
        } else {
            max = phi; 
        }
    }
    return phi;
}

// == fmodf but positive
float wrap_phase(float raw_phase, float period) {
    float result = fmodf(raw_phase, period);
    if (result < 0.0f) {
        result += period;
    }
    return result;
}

float compute_phase(float w, float h, float D, float T, float dt, int n) {
    int next_frame = (int)(T / dt) + 1;
    float timestamp = next_frame * dt;

    if (n % 4 == 0) {
        return wrap_phase(D / 4. - timestamp, D);
        
    } else if (n % 2 == 0) { 
        if (w >= h) {
            return wrap_phase(D - timestamp, D);
        } else {
            return wrap_phase(3. * D / 2. - timestamp, D);
        }
        
    } else {
        float phi_opt = dichotomy(0., D/4., 30, D, w, h);
        return wrap_phase(D + phi_opt - timestamp, D);
    }
}


void init_env(env environment, float T) {
    float w = get_w(environment);
    float h = get_h(environment);
    int n = get_n(environment);
    float D = 2.0f * sqrtf(h*h + w*w);
    float space_divided = D / (float)n;
    float dt = get_dt(environment);
    float phase = compute_phase(w,h,D,T,dt,n);

    for (int i = 0; i < n; i++) {
        float pos = fmod((float)i * space_divided + phase, D);
        float x;
        float y;
        float vx;
        float vy;
        
        int segment_idx = get_pos(pos, w, h, &x, &y);
        get_speed(segment_idx, w, h, &vx, &vy);
        
        set_particule(environment, i, x, y, vx, vy);
    }
}
