#pragma once

struct sphere
{
    vec3d center;
    float radius;
    float color_intensity;
    bool overleaped = false;


    sphere() = default;

    sphere(float _x, float _y, float _z, float _radius, float _intensity){
        //X coordinate
        center.x = _x;
        //Y coordinate
        center.y = _y;
        //Z coordinate
        center.z = _z;
        //Radius
        radius = _radius;
        //Intensity [0,1]
        color_intensity = _intensity;
    }
    sphere(float value, float _radius, float _intensity){
        center.x = value;
        center.y = value;
        center.z = value;
        radius = _radius;
        color_intensity = _intensity;
    }
};