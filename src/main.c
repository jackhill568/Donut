#include <linmath.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_SIZE 60
#define MAX_STEPS 79
#define MAX_DIST 100
#define SURF_DIST 0.01

#define MAJ_RADIUS 7
#define MIN_RADIUS 3

#define PI 3.141

vec3 LIGHT_DIR = {0, 0, -10};

char SHADE[] = {'#', '*', '-', '.'};

typedef struct {
  int width;
  int height;
  char *buffer;
} FrameBuffer;

void vec3_set(float *M, float *T) {
  M[0] = T[0];
  M[1] = T[1];
  M[2] = T[2];
}

float dotProduct(vec3 a, vec3 b) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

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
  out[0] = c * in[1] + s * in[2];
  out[1] = in[0];
  out[2] = -s * in[1] + c * in[2];
}
void rotateZ(vec3 out, vec3 in, float a) {
  float c = cosf(a), s = sinf(a);
  out[0] = c * in[1] - s * in[2];
  out[1] = s * in[1] + c * in[2];
  out[2] = in[0];
}

float sdTorus(vec3 p, float angle) {
  vec3 rp;
  rotateX(rp, p, 1.0);
  vec3 temp;
  vec3_dup(temp, rp);
  rotateY(temp, rp, angle);
  vec3_dup(rp, temp);
  rotateZ(rp, temp, angle);

  vec2 q = {vec2_len((vec2){rp[0], rp[2]}) - MAJ_RADIUS, rp[1]};
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

void getNormal(vec3 normals, vec3 p, float angle) {

  vec3 x, y, z;

  vec3_set(x, (vec3){0.01, 0, 0});
  vec3_set(y, (vec3){0, 0.01, 0});
  vec3_set(z, (vec3){0, 0, 0.01});

  vec3 i, j;
  vec3_add(i, p, x);
  vec3_sub(j, p, x);
  normals[0] = sdTorus(i, angle) - sdTorus(j, angle);

  vec3_add(i, p, y);
  vec3_sub(j, p, y);
  normals[1] = sdTorus(i, angle) - sdTorus(j, angle);

  vec3_add(i, p, z);
  vec3_sub(j, p, z);
  normals[2] = sdTorus(i, angle) - sdTorus(j, angle);

  vec3_norm(normals, normals);
}

void outputBuffer(FrameBuffer fb) {
  for (int y = 0; y < fb.height; y++) {
    for (int x = 0; x < fb.width; x++) {
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
      ro[2] = -50;
      vec3 rd;
      rd[0] = (x - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
      rd[1] = (y - 0.5 * SCREEN_SIZE) / SCREEN_SIZE;
      rd[2] = 1;

      float distance = rayMarch(ro, rd, angle);

      vec3 p;
      vec3_scale(p, rd, distance);
      vec3_add(p, p, ro);

      vec3 normal;
      getNormal(normal, p, angle);

      float angle = acosf(dotProduct(normal, LIGHT_DIR) /
                          (vec3_len(normal) * vec3_len(LIGHT_DIR)));
      char c;
      if (distance >= MAX_DIST || angle > PI / 2 + PI / 8) {
        c = ' ';
      } else if (angle >= (PI / 2 - PI / 8)) {
        c = SHADE[3];
      } else if (angle >= (PI / 2 - PI / 4)) {
        c = SHADE[2];
      } else if (angle >= (PI / 2 - 3 * PI / 8)) {
        c = SHADE[1];
      } else if (angle >= 0.0) {
        c = SHADE[0];
      } else {
        c = '@';
      }
      *getPixel(&fb, x, y) = c;
    }
  }
}

int main() {

  FrameBuffer fb;
  fb.width = SCREEN_SIZE;
  fb.height = SCREEN_SIZE;

  fb.buffer = malloc(sizeof(char) * SCREEN_SIZE * SCREEN_SIZE);
  int count = 0;
  for (;; count++) {
    float angle = count * PI / 180;
    draw(fb, angle);
    outputBuffer(fb);
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 10000 * 100; 
	nanosleep(&ts, NULL);
  }
  free(fb.buffer);
  return 0;
}
