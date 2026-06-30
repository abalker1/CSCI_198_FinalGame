#ifndef _COMMON_H_INCLUDED
#define _COMMON_H_INCLUDED

#define GLEW_STATIC

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <chrono> // Replaces time.h. This is better.
#include <cstdlib>
#include <deque>
#include <fstream>
#include <functional>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <windows.h>

#define PI 3.14159

using namespace std;

struct visualParkedCar {
    float z_position;      // Z position along the road (for looping)
    float y_position;
    float x_offset;        // Left (-X) or right (+X) side of road
    int textureIndex;      // Which texture to use (0 or 1)
    bool isVisible;        // Whether this car should be visible (can sync with parking spots later)
};

typedef struct{
    float x;
    float y;
    } vec2;

typedef struct{
    float x;
    float y;
    float z;
} vec3;

typedef struct{
    float r;
    float g;
    float b;
} col3;

typedef struct{
    float r;
    float g;
    float b;
    float a;
} col4;

// Helper to clamp values
// Returns val if lo < val < hi
// Returns lo if val < lo
// Returns hi if val > hi
auto clamp = [](double val, double lo, double hi) {
    return min(hi, max(lo, val));
};

// Helper for random number generation
// Returns a random float between a and b
auto frand = [](float a, float b) {
    return a + (b - a) * (rand() / static_cast<float>(RAND_MAX));
};

// Helper to return one of two different values based on a cutoff
// cutoff must be between 0 and 1
// Returns `a` if the random value
auto either = [](float a, float b, float cutoff) {
    float r = (rand() / static_cast<float>(RAND_MAX));
    if (r < cutoff) {
        return a;
    }
    else {
        return b;
    }
};

// Helper for getting distance between two vec3
// Returns the distance between two 3D vector positions
auto vec3Distance = [](vec3 p1, vec3 p2) {
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));
};

#endif // _COMMON_H_INCLUDED
