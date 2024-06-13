#pragma once

float dot(vec3d a, vec3d b){ return (a.x * b.x + a.y * b.y + a.z * b.z); }

float length(vec3d a) { return sqrt(dot(a, a));}

std::vector<float> intersectionRaySphere(vec3d origin, vec3d direction, sphere sp){
    vec3d C = sp.center;
    float r = sp.radius;
    vec3d OC = C - origin;
    
    float a = dot(direction, direction);
    float b = 2 * dot(OC, direction);
    float c = dot(OC, OC) - r * r;

    float discr = b * b - 4 * a * c;

    if (discr < 0) {
        std::vector<float> solution;
        solution.push_back(0);
        solution.push_back(0);
        return solution;
    }
    else{
        std::vector<float> solution;
        solution.push_back((-b + sqrt(discr)) / (2 * a));
        solution.push_back((-b - sqrt(discr)) / (2 * a));
        return solution;
    }
}

// В будуюем добавить разные виды света
float ComputeLighting(vec3d point, vec3d normal, light* lights){
    // float intensity = (dot((lights.position / length(lights.position)), normal) + 1) / 2;
    float intensity = 0;
    float lenghts_n = length(normal);
    for (int i = 0; i < 3; i++)
    {
        light tmp_light = lights[i];
        if (tmp_light.type == 0) intensity += tmp_light.intensity;
        else
        {
            vec3d tmp_vec;
            if (tmp_light.type == 1) tmp_vec = tmp_light.position - point;
            else tmp_vec = tmp_light.position;

            float n_dot_l = dot(normal, tmp_vec);
            if (n_dot_l > 0) intensity = intensity + tmp_light.intensity * n_dot_l / (lenghts_n * length(tmp_vec));
        }
    }
    return intensity;
}

float traceRayV2(vec3d origin, vec3d direction, float t_min, float t_max, sphere SpS, light* lights)
{
    float closest_t = 9999999.9f;
    bool intersected = false;
    std::vector<float> root = intersectionRaySphere(origin, direction, SpS);
    if (root.at(0) < t_max && root.at(0) > t_min && root.at(0) < closest_t)
    {
        closest_t = root.at(0);
        // closest = SpS.at(i);
        intersected = true;
    }
    if (root.at(1) < t_max && root.at(1) > t_min && root.at(1) < closest_t)
    {
        closest_t = root.at(1);
        // closest = SpS.at(i);
        intersected = true;
    }
    if (!intersected)
        return 0;
    vec3d point = origin + (direction * closest_t);
    vec3d normal = SpS.center - point;
    normal = normal * (-1 / length(normal));

    return ComputeLighting(point, normal, lights) * SpS.color_intensity;
}

float traceRay(vec3d origin, vec3d direction, float t_min, float t_max, std::vector<sphere> SpS, light* lights){
    float closest_t = 9999999.9f;
    bool intersected = false;             
    sphere closest;         
    for (int i = 0; i < SpS.size(); i++){
        std::vector<float> root = intersectionRaySphere(origin, direction, SpS.at(i));
        if (root.at(0) < t_max && root.at(0) > t_min && root.at(0) < closest_t)
        {
            closest_t = root.at(0);
            closest = SpS.at(i);
            intersected = true;
        }
        if (root.at(1) < t_max && root.at(1) > t_min && root.at(1) < closest_t)
        {
            closest_t = root.at(1);
            closest = SpS.at(i);
            intersected = true;
        }
    }
    if (!intersected)
        return 0;

    vec3d point = origin + (direction * closest_t);
    vec3d normal = closest.center - point;
    normal = normal * (-1 / length(normal));

    // float a = ComputeLighting(point, normal, lights);
    return ComputeLighting(point, normal, lights) * closest.color_intensity;
    // return 1;
}
