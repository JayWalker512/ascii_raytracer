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

/***************************/
/*** Vector related code ***/
/***************************/

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

vec3_t normalize(vec3_t v) {
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

float distance(vec3_t u, vec3_t v) {
	return magnitude(difference(u,v));
}

/***************************/
/*** Sphere related code ***/
/***************************/

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

	heading = normalize(heading);

	float a = pow(dot(heading, difference(origin, sphere.center)), 2.f);
	float b = pow(magnitude(difference(origin, sphere.center)), 2.f);
	float underSqrt = sqrt(a - b + pow(sphere.radius, 2));

	if (underSqrt >= 0) {
		//float plus = -(dot(heading, difference(origin, sphere.center))) + underSqrt;
		float minus = -(dot(heading, difference(origin, sphere.center))) - underSqrt;
		//return fmaxf(plus, minus);
		return minus;
	}

	return -1.f;
}

/************************/
/*** Point light code ***/
/************************/

typedef struct light_s {
	vec3_t pos;
	float intensity; //should be in [0,1]
} light_t;

light_t light(vec3_t pos, float intensity) {
	light_t l;
	l.pos.x = pos.x;
	l.pos.y = pos.y;
	l.pos.z = pos.z;
	l.intensity = intensity;
	return l;
}

/************************************/
/*** ASCII pixel framebuffer code ***/
/************************************/

void setPixel(char * pixelArray, unsigned int columns, unsigned int rows, int x, int y, char character) {
	if ( x > columns || x < 0 ) {
		return;
	}

	if ( y > rows || y < 0 ) {
		return;
	}

	pixelArray[(y * columns) + x] = character;
} 

void printPixels(char * pixelArray, unsigned int columns, unsigned int rows) {
	char str[columns + 1];
	str[columns] = 0;
	for (unsigned int y = 0; y < rows; y++) {
		for (unsigned int x = 0; x < columns; x++) {
			str[x] = pixelArray[(y * columns) + x]; 
		}
		puts(str);
	}

	//print a row of spaces to separate 'frames'
	for (unsigned int y = 0; y < columns; y++) {
		str[y] = ' ';
	}
	puts(str);
	puts(str);
}

//intensity is 0-255
char charShade(unsigned char intensity) {
	const int levels = 12;
	char shades[12] = " .,:;+ox%&#@";
	return shades[(255-intensity)*levels/256];
}

void clearPixels(char * pixels, unsigned int columns, unsigned int rows) {
	for (unsigned int i = 0; i < columns * rows; i++) {
		//pixels[i] = ' ';
		pixels[i] = 0;
	}
}

/***********************/
/*** PGM output code ***/
/***********************/

void printPGM(char * pixels, unsigned int columns, unsigned int rows) {
	printf("P2\n");
	printf("%d %d\n", columns, rows);
	printf("256\n");
	for (unsigned int y = 0; y < rows; y++) {
		for (unsigned int x = 0; x < columns; x++) {
			printf("%d ", (unsigned char)pixels[(y * columns) + x]); 
		}
		printf("\n");
	}
}


/************************************************/
/*** Ray Tracing abstractions in this section ***/
/************************************************/

//returns the intensity of the pixel through which the ray was traced
float traceRay(vec3_t origin, vec3_t heading, sphere_t * spheres, int numSpheres, light_t * lights, int numLights) {
	/* Need to check for intersections with all spheres, and track
	 * which intersection is nearest to the origin of the ray. */

	heading = normalize(heading);

	int nearestIndex = -1;
	float nearestParam = 99999.f;
	for (int i = 0; i < numSpheres; i++) {
		float f = raySphereIntersection(origin, heading, spheres[i]);
		if (f > 0.f && f < nearestParam) {
			nearestParam = f;
			nearestIndex = i;
		} 
	}

	if (nearestIndex == -1) {
		return 0.f;
	}

	vec3_t pointOfIntersection = sum(vec3Lerp(heading, nearestParam), origin);
	vec3_t surfaceNormal = normalize(vector3FromTo(spheres[nearestIndex].center, pointOfIntersection));

	float shadowBias = 0.001f;
	pointOfIntersection = sum(pointOfIntersection, vec3Lerp(surfaceNormal, shadowBias));

	//Now we need to trace rays from the intersection to all
	//un-occluded lights and add up their contributed intensities.
	float intensity = 0.f;
	for (int i = 0; i < numLights; i++) {
		vec3_t lightHeading = normalize(vector3FromTo(pointOfIntersection, lights[i].pos));
		bool obscured = false;
		for (int k = 0; k < numSpheres; k++) {
			if (0.f < raySphereIntersection(pointOfIntersection, lightHeading, spheres[k])) {
				//if there IS an intersection, the light is obscured and contributes no intensity
				obscured = true;
				continue;
			}
		}	

		//if the ray from the intersection point to the light is not obscured, 
		//we add it's contribution to the intensity at that pixel.
		if (false == obscured) {
			float reflectionCoefficient = dot(normalize(surfaceNormal), normalize(lightHeading));
			if (reflectionCoefficient > 0.f) {
				//Using equation 1/(1 + k*d^2) where k is just some constant that determines how rapidly the intensity falls off.
				//This also solves the problem with a plain inverse square where intensity saturates when distance < 1.
				intensity += reflectionCoefficient * lights[i].intensity * (1/(1 + 0.3f*pow(distance(pointOfIntersection, lights[i].pos), 2)));
			}
		}
	}

	return intensity; 
}

int main(int argc, char * argv[]) {
	unsigned int columns = 20;
	unsigned int rows = 20;
	if (argc != 1) {
		columns *= 100;
		rows *= 100;
	}

	char pixels[columns * rows];
	for (unsigned int i = 0; i < columns * rows; i++) {
		pixels[i] = 0;
	}

	const int numSpheres = 2;
	sphere_t spheres[numSpheres];
	spheres[0] = sphere(0.f, 0.f, 4.5f, 2.5f);

	const int numLights = 1;
	light_t lights[numLights];
	lights[0] = light(vector3(-1.f, 0.f, 0.f), 1.f);
	//lights[1] = light(vector3(-2.f, 0.f, 0.f), 1.f);
	
	//vec3_t sphereToLight = vector3FromTo(spheres[0].center, lights[0].pos);
	//vec3_t secondSphere = vec3Lerp(sphereToLight, 0.6f);
	//vec3_t secondSphere = vector3(-0.5f, 0.f, 0.5f);
	//spheres[1] = sphere(secondSphere.x, secondSphere.y, secondSphere.z, 0.25f);

	float t = 4.5f;

	do {
		
		if (argc == 1) {
			usleep(100000);
		}

		//animate the sphere
		spheres[1] = sphere(3 * cos(t), 0.f, 4.5f + (3 * sin(t)), 0.25f);
		t += 6.28 / 180.0;

		clearPixels(pixels, columns, rows);

		for (int x = 0; x < columns; x++) {
			for (int y = 0; y < rows; y++) {
				//calculate the origin of the ray for orthographic projection
				float xCoord = 2.f * ((float)x / (float)(columns - 1)) - 1.f;
				float yCoord = 2.f * ((float)y / (float)(rows - 1)) - 1.f;
				//printf("x: %f, y: %f\n", xCoord, yCoord);

				//perspective vectors
				vec3_t origin = vector3(0.f, 0.f, 0.f);
				vec3_t heading = vector3(xCoord, yCoord, 1.f);
				
				char c = 0;				
				if (argc == 1) {
					//this is the ascii path
				
					c = ' ';
					c = charShade(255 - 255 * traceRay(origin, heading, spheres, numSpheres, lights, numLights));
				} else {
					//this is the PGM path

					c = 0;
					c = 255 * traceRay(origin, heading, spheres, numSpheres, lights, numLights);
				}

				setPixel(pixels, columns, rows, x, y, c);
			}
		}

		if (argc == 1) {
			printPixels(pixels, columns, rows);	
		} else {
			printPGM(pixels, columns, rows);
		}

	} while (argc == 1);

	return 0;
}