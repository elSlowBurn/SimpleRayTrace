#pragma once
#ifndef __vec3d_h__
#define __vec3d_h__

struct vec3d
{
    float x, y, z;

    vec3d(){
    }

    vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

    vec3d(float value) : x(value), y(value), z(value) {};

    vec3d operator+ (vec3d const& other) { return vec3d(other.x + x, other.y + y, other.z + z); }
    vec3d operator- (vec3d const& other) { return vec3d(other.x - x, other.y - y, other.z - z); }
    vec3d operator* (vec3d const& other) { return vec3d(other.x * x, other.y * y, other.z * z); }
    vec3d operator/ (vec3d const& other) { return vec3d(x / other.x, y / other.y, z / other.z); }
    vec3d operator- () { return vec3d(-x, -y, -z); }
};


#endif