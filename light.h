#pragma once

struct light
{
    int type; /*type 0 - Ambient (only intensity)
                type 1 - Point (intensity and position)
                type 2 - Directional (intensity and position)*/
    float intensity;
    vec3d position;
    light() = default;
    light(int _type, float _intensity) : type(_type), intensity(_intensity){};

    light(int _type, float _intensity, float x, float y, float z){
        type = _type;
        intensity = _intensity;
        position.x = x;
        position.y = y;
        position.z = z;
     }
     
    light(float _intensity, float x, float y, float z){
        intensity = _intensity;
        position.x = x;
        position.y = y;
        position.z = z;
     }
};
