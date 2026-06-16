#include <linmath.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_SIZE 100
#define MAX_STEPS 50
#define MAX_DIST 50
#define SURF_DIST 1

#define MAJ_RADIUS 10
#define MIN_RADIUS 7


typedef struct {
	int width;
	int height;
	char *buffer;
} FrameBuffer;

char *getPixel(FrameBuffer *fb, int x, int y) {
	return &fb->buffer[y * fb->width + x];
}


float sdTorus( vec3 p) {
	vec2 q;
	q[0] = vec2_len((vec2){p[0], p[2]}) - MAJ_RADIUS;
	q[1] = p[1];
  	return vec3_len(q)-MIN_RADIUS;
}


float rayMarch(vec3 ro, vec3 rd) {
    float d = 0.0;

    for (int i = 0; i < MAX_STEPS; i++) {

        vec3 p;
        vec3 tmp;
        vec3_scale(tmp, rd, d);
        vec3_add(p, ro, tmp);

        float ds = sdTorus(p);

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

void draw(FrameBuffer fb) {

	for (int y = 0; y < fb.height; y++) {

		for (int x = 0; x < fb.width; x++) {
			
			vec3 ro;
			ro[0] = 0;
			ro[1] = 0;
			ro[2] = -3;
			vec3 rd;
			rd[0] = (x - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
			rd[1] = (y - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
			rd[2] = 1;

			float distance = rayMarch(ro, rd);

			*getPixel(&fb, x, y) =  (distance < MAX_DIST) ? '#' : ' ';
		}
	}
}

int main() {
	
	FrameBuffer fb;
	fb.width = SCREEN_SIZE;
	fb.height = SCREEN_SIZE;

	fb.buffer = malloc(sizeof(char) * SCREEN_SIZE * SCREEN_SIZE);
		
	draw(fb);

	outputBuffer(fb);


	free(fb.buffer);
	return 0;
}
