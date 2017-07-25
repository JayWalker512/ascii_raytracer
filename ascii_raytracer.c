/*
Copyright 2017 Brandon Foltz

Permission is hereby granted, free of charge, 
to any person obtaining a copy of this software 
and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, 
copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions 
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF 
ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>

typedef enum { false, true } bool;

typedef struct vec3_s {
	float x;
	float y;
	float z;
} vec3_t;

void printVec(vec3_t v, char * name) {
	printf("%s: (%f, %f, %f)\n", name, v.x, v.y, v.z);
}

vec3_t vecLerpBetween(vec3_t src, vec3_t tgt, float lerp) {
	vec3_t v;
	v.x = src.x + ((tgt.x - src.x) * lerp);
	v.y = src.y + ((tgt.y - src.y) * lerp);
	v.z = src.z + ((tgt.z - src.z) * lerp);
	return v;
}

vec3_t vec3Lerp(vec3_t v, float lerp) {
	vec3_t u;
	u.x = v.x * lerp;
	u.y = v.y * lerp;
	u.z = v.z * lerp;
	return u;
}

vec3_t vector3FromTo(vec3_t src, vec3_t tgt) {
	vec3_t v;
	v.x = (tgt.x - src.x);
	v.y = (tgt.y - src.y);
	v.z = (tgt.z - src.z);
	return v;
}

vec3_t vector3(float x, float y, float z) {
	vec3_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vec3_t norm(vec3_t v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x = v.x / length;
	v.y = v.y / length;
	v.z = v.z / length;
	return v;
}

float dot(vec3_t u, vec3_t v) {
	return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

float magnitude(vec3_t v) {
	return sqrt(dot(v,v));
}

vec3_t sum(vec3_t u, vec3_t v) {
	vec3_t w;
	w.x = u.x + v.x;
	w.y = u.y + v.y;
	w.z = u.z + v.z;
	return w;
}

vec3_t difference(vec3_t u, vec3_t v) {
	return sum(u, vector3(-v.x, -v.y, -v.z));
}

typedef struct sphere_s {
	float radius;
	vec3_t center;
} sphere_t;

sphere_t sphere(float x, float y, float z,float r) {
	sphere_t s;
	s.radius = r;
	s.center.x = x;
	s.center.y = y;
	s.center.z = z;
	return s;
}

/* Determines if there is an intersection between a given ray and sphere.
 * If no intersection is found, returns -1.f, otherwise returns distance along
 * NORMALIZED vector 'heading' to the point of intersection.
 */
float raySphereIntersection(vec3_t origin, vec3_t heading, sphere_t sphere) {
	//working out sqrt( (heading . (origin - sphere.center))^2 - mag(origin - sphere.center)^2 + sphere.radius^2 )
	//where a = (heading . (origin - sphere.center))^2
	//where b = mag(origin - sphere.center)^2

	heading = norm(heading);

	float a = pow(dot(heading, difference(origin, sphere.center)), 2.f);
	float b = pow(magnitude(difference(origin, sphere.center)), 2.f);
	float underSqrt = sqrt(a - b + pow(sphere.radius, 2));

	if (underSqrt >= 0) {
		return -(dot(heading, difference(origin, sphere.center))) - underSqrt;
	}

	return -1.f;
}

void setPixel(char * pixelArray, char columns, char rows, int x, int y, char character) {
	if ( x > columns || x < 0 ) {
		return;
	}

	if ( y > rows || y < 0 ) {
		return;
	}

	pixelArray[(y * columns) + x] = character;
} 

void printPixels(char * pixelArray, char columns, char rows) {
	char str[columns + 1];
	str[columns] = 0;
	for (char y = 0; y < rows; y++) {
		for (char x = 0; x < columns; x++) {
			str[x] = pixelArray[(y * columns) + x]; 
		}
		//memcpy(str, pixelArray[y * columns], columns);
		puts(str);
	}
	usleep(50000);

	//print a row of spaces to separate 'frames'
	for (char y = 0; y < columns; y++) {
		str[y] = ' ';
	}
	puts(str);
	puts(str);
	puts(str);
	puts(str);
}

//intensity is 0-255
char charShade(unsigned char intensity) {
	const int levels = 10;
	char shades[10] = " .,:;ox%#@";
	return shades[(255-intensity)*levels/256];
}

void clearPixels(char * pixels, char columns, char rows) {
	for (int i = 0; i < columns * rows; i++) {
		pixels[i] = ' ';
	}
}

#define COLUMNS 40
#define ROWS 20

void main() {
	char pixels[COLUMNS * ROWS];

	/*for (int i = 0; i < COLUMNS * ROWS; i++) {
		pixels[i] = charShade(shades, 10, ((i % COLUMNS) / (float)COLUMNS) * 255);
	}*/

	/*for (char y = 0; y < ROWS; y++) {
		for (char x = 0; x < COLUMNS; x++) {
			setPixel(pixels, COLUMNS, ROWS, x, y, charShade(shades, 10, (x % 10) * (256 / 10.0))); 
		}
	}*/

	/*for (int i = 0; i < COLUMNS * ROWS; i++) {
		pixels[i] = ' ';
	}

	for (int x = 0; x < COLUMNS; x++) {
		//draw an exponential curve
		setPixel(pixels, COLUMNS, ROWS, x, (int)pow((double)(x - 20), 2.f), '#');
		
		//draw a line
		//setPixel(pixels, COLUMNS, ROWS, x, x, '.');
		
		//draw a big circle section
		setPixel(pixels, COLUMNS, ROWS, x, ROWS * sqrt(1.0 - pow((x / (float)COLUMNS), 2)), '@');
		
		//draw a smaller circle section
		setPixel(pixels, COLUMNS, ROWS, x, (ROWS / 2.0) * sqrt(1.0 - pow((x / ((float)COLUMNS / 2.0)), 2)), '@');
	}

	printPixels(pixels, COLUMNS, ROWS);*/

	/*clearPixels(pixels, COLUMNS, ROWS);

	float t = 0.0;
	while ( 1 ) {
		clearPixels(pixels, COLUMNS, ROWS);

		//move a pixel around in a circle
		//(x,y) = (r*cos(t),r*sin(t))
		setPixel(pixels, COLUMNS, ROWS, (COLUMNS / 2.0) + (int)10.0*cos(t), (ROWS / 2.0) + (int)10.0*sin(t), '@');
		setPixel(pixels, COLUMNS, ROWS, (COLUMNS / 2.0) + (int)5.0*cos(t), (ROWS / 2.0) + (int)5.0*sin(t), '.');
		//setPixel(pixels, COLUMNS, ROWS, (COLUMNS / 2.0) + (int)10.0*cos(t - 1.5), (ROWS / 2.0) + (int)10.0*sin(t - 1.5), ' ');
		//setPixel(pixels, COLUMNS, ROWS, (COLUMNS / 2.0) + (int)5.0*cos(t - 1.5), (ROWS / 2.0) + (int)5.0*sin(t - 1.5), ' ');		
		t += 0.1; //(6.28 / 90.0);

		printPixels(pixels, COLUMNS, ROWS);
	}*/

	//silly vector test
	/*vec3_t vec;
	vec.x = 0.f;
	vec.y = 0.f;
	vec.z = 1.f;
	vec = norm(vec);
	printVec(vec, "vector");

	vec3_t vec2;
	vec2.x = 0.f;
	vec2.y = 1.f;
	vec2.z = 1.f;

	vec3_t up = vector3FromTo(vec, vec2);
	printVec(up, "up");

	vec2 = norm(vec2);

	vec3_t p = vecLerpBetween(vec, vec2, 0.5f);
	printVec(p, "lerp");

	printf("angle: %f\n", asin(dot(vec, vec2)) * (180.0 / 3.14));
	*/

	//checking intersection detection code
	sphere_t s = sphere(0.f, 0.f, 1.f, 1.f);
	vec3_t v = vector3(0.f, 0.f, 1.f);
	float f = raySphereIntersection(vector3(0.f, 0.f, 0.f), v, s);
	//printf("f: %f\n", f);

	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			//calculate the origin of the ray for orthographic projection
			float xCoord = 2.f * ((float)x / (float)(COLUMNS - 1)) - 1.f;
			float yCoord = 2.f * ((float)y / (float)(ROWS - 1)) - 1.f;
			//printf("x: %f, y: %f\n", xCoord, yCoord);
			vec3_t origin = vector3(xCoord, yCoord, 0.f);
			vec3_t heading = vector3(0.f, 0.f, 1.f);
			f = raySphereIntersection(origin, heading, s);
			char c = ' ';
			if (f >= 0) {
				//dist = magnitude((heading * f) - origin);
				float dist = magnitude(difference(vec3Lerp(heading, f), origin));
				if (dist <= 1.f ) {
					c = charShade((unsigned char)(dist * 255));
				} 
				//printf("dist: %f\n", dist);
			}
			setPixel(pixels, COLUMNS, ROWS, x, y, c);
		}
	}

	printPixels(pixels, COLUMNS, ROWS);

	return;
}