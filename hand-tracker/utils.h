#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/glut.h>

#define PI (3.1415926535897932384)

typedef struct
{
	bool init;
	float min;
	float max;
} TLimits;

typedef struct
{
	TLimits x;
	TLimits y;
	TLimits z;
} TLimits3;

typedef struct
{
	float x;
	float y;
	float z;
} JointVector;

float clamp(float value, float min, float max);
float scale_and_expand_limits(float value, TLimits* limits, float starting_width=0.0);
float scale_and_expand_limits_with_clamp(float value, float min, float max, TLimits* limits, float starting_width=0.0);

unsigned int get_closest_power_of_two(unsigned int n);

void draw_string(void *font, char *str);

void draw_circle(float x, float y, float radius);

GLuint init_texture(void** buf, int& width, int& height);

#endif
