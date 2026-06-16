#include <linmath.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_SIZE 100
#define MAX_STEPS 50
#define MAX_DIST 50
#define SURF_DIST 0.01

#define MAJ_RADIUS 5
#define MIN_RADIUS 3

#define PI 3.141


typedef struct {
	int width;
	int height;
	char *buffer;
} FrameBuffer;

char *getPixel(FrameBuffer *fb, int x, int y) {
	return &fb->buffer[y * fb->width + x];
}

void rotateX(vec3 out, vec3 in, float a) {
    float c = cosf(a), s = sinf(a);
    out[0] = in[0];
    out[1] = c * in[1] - s * in[2];
    out[2] = s * in[1] + c * in[2];
} 
void rotateY(vec3 out, vec3 in, float a) {
    float c = cosf(a), s = sinf(a);
    out[1] = in[0];
    out[0] = c * in[1] - s * in[2];
    out[2] = s * in[1] + c * in[2];
}
void rotateZ(vec3 out, vec3 in, float a) {
    float c = cosf(a), s = sinf(a);
    out[2] = in[0];
    out[0] = c * in[1] - s * in[2];
    out[1] = s * in[1] + c * in[2];
}

float sdTorus(vec3 p, float angle) {
    vec3 rp;
    rotateX(rp, p, 1.0f); 
    // rotateY(rp, rp, angle); 
    // rotateZ(rp, rp, angle); 
    vec2 q = { vec2_len((vec2){ rp[0], rp[2] }) - MAJ_RADIUS, rp[1] };
    return vec2_len(q) - MIN_RADIUS;
}


float rayMarch(vec3 ro, vec3 rd, float angle) {
    float d = 0.0;

    for (int i = 0; i < MAX_STEPS; i++) {

        vec3 p;
        vec3 tmp;
        vec3_scale(tmp, rd, d);
        vec3_add(p, ro, tmp);

        float ds = sdTorus(p, angle);

        if (ds < SURF_DIST)
            return d;

        d += ds;

        if (d > MAX_DIST)
            return MAX_DIST;
    }

    return MAX_DIST;
}

void outputBuffer(FrameBuffer fb) {
	for (int y = 0; y < fb.height; y ++) {
		for (int x = 0; x<fb.width; x ++) {
			putchar(*getPixel(&fb, x, y));
		}
		putchar('\n');
	}
}

void draw(FrameBuffer fb, float angle) {

	for (int y = 0; y < fb.height; y++) {

		for (int x = 0; x < fb.width; x++) {
			
			vec3 ro;
			ro[0] = 0;
			ro[1] = 0;
			ro[2] = -30;
			vec3 rd;
			rd[0] = (x - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
			rd[1] = (y - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
			rd[2] = 1;

			float distance = rayMarch(ro, rd, angle);

			*getPixel(&fb, x, y) =  (distance < MAX_DIST) ? '#' : ' ';
		}
	}
}

int main() {
	
	FrameBuffer fb;
	fb.width = SCREEN_SIZE;
	fb.height = SCREEN_SIZE;

	fb.buffer = malloc(sizeof(char) * SCREEN_SIZE * SCREEN_SIZE);
	int count = 0;	
	// for (;;count++) {
	float angle = count * PI / 180;
	draw(fb, angle);
	outputBuffer(fb);
		// system("clear");
	// }

	free(fb.buffer);
	return 0;
}
