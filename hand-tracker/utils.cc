#include <math.h>
#include "utils.h"

float scale_and_expand_limits(float value, TLimits* limits, float starting_width)
{
	if(!limits->init) {
		limits->min = value - (starting_width / 2.0);
		limits->max = value + (starting_width / 2.0);
		limits->init = true;
		return 0.0;		// no reasonable value to return until we know a range
	}

	if(value < limits->min)
		limits->min = value;

	if(value > limits->max)
		limits->max = value;

	float range = (limits->max - limits->min);

	if(range == 0.0)
		return 0.0;

	return (float)(value - limits->min) / (float)range;
}

unsigned int get_closest_power_of_two(unsigned int n)
{
	unsigned int m = 2;
	while(m < n) m <<= 1;
	return m;
}

void draw_string(void *font, char *str)
{
	char c, *p = str;
	while((c = *str++) != '\0') {
		glutBitmapCharacter(font, c);
	}
}

void draw_circle(float x, float y, float radius)
{
	static const int circle_points = 100;
	static const float angle_per_point = (2.0f * 3.1416f / circle_points);

	glBegin(GL_POLYGON);
		for(int i=0 ; i<circle_points+1 ; i++) {
			glVertex2d(x + radius * cos(angle_per_point * i), y + radius * sin(angle_per_point * i));
		}
	glEnd();
}

GLuint init_texture(void** buf, int& width, int& height)
{
	GLuint texID = 0;
	glGenTextures(1, &texID);

	width = get_closest_power_of_two(width);
	height = get_closest_power_of_two(height);
	*buf = new unsigned char[width*height*4];
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texID;
}
