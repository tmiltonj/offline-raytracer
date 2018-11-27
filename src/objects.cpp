#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "objects.hpp"
#include "objloader.hpp"


const float AREA_LIGHT_SPREAD { 0.5f };
const float BIAS { 0.1f };


Scene::Scene()
{
    camera = nullptr;
    objects = std::vector<std::shared_ptr<Object>>();
}



Camera::Camera(Vec3 pos, int fov, int f, float a)
{
    if (fov <= 0)
        throw std::invalid_argument("fov must be > 0");

    if (f <= 0)
        throw std::invalid_argument("f must be > 0");

    if (a <= 0.0)
        throw std::invalid_argument("a must be > 0.0");

    this->pos = pos;
    this->fov = fov;
    this->f = f;
    this->a = a;
}



bool valid_light(Vec3 L)
{
    return (L.x >= 0.0 && L.y >= 0.0 && L.z >= 0.0);
}



Light::Light(Vec3 pos, Vec3 amb, Vec3 dif, Vec3 spe)
{
    if (!valid_light(amb))
        throw std::invalid_argument("Values of amb must be >= 0");

    if (!valid_light(dif))
        throw std::invalid_argument("Values of dif must be >= 0");

    if (!valid_light(spe))
        throw std::invalid_argument("Values of spe must be >= 0");

    this->pos = pos;
    this->amb = amb;
    this->dif = dif;
    this->spe = spe;
}



void Light::soften(Vec3 l, Vec3 v, int num_shadows, std::vector<std::shared_ptr<Light>> &area_lights)
{
    //std::vector<std::shared_ptr<Light>> area_lights;

    Vec3 norm { glm::normalize(glm::cross(l, v)) * (AREA_LIGHT_SPREAD / num_shadows) };
    float rotation { 2.0f * glm::pi<float>() / num_shadows };
    for (int i = 0; i < num_shadows; i++)
    {
        area_lights.push_back(
            std::make_shared<Light>(
                pos + norm, amb / (float)num_shadows, dif / (float)num_shadows, spe / (float)num_shadows
            ));

        norm = glm::rotate(norm, rotation, l) + glm::normalize(norm) * (AREA_LIGHT_SPREAD / num_shadows);
    }
}



Object::Object(Vec3 amb, Vec3 dif, Vec3 spe, float shi)
{
    if (!valid_light(amb))
        throw std::invalid_argument("Values of amb must be >= 0");

    if (!valid_light(dif))
        throw std::invalid_argument("Values of dif must be >= 0");

    if (!valid_light(spe))
        throw std::invalid_argument("Values of spe must be >= 0");

    if (shi <= 0)
        throw std::invalid_argument("Value for shi must be >= 0.0");

    this->amb = amb;
    this->dif = dif;
    this->spe = spe;
    this->shi = shi;
}



Plane::Plane(
    Vec3 normal, Vec3 point,
    Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    this->normal = normal;
    this->point = point;
}

Vec3 Plane::get_normal(Vec3 point){ return normal; }

// http://www.geomalgorithms.com/a05-_intersect-1.html
float Plane::check_collision(Vec3 p0, Vec3 d)
{
    // TODO: Replace -1 w/ constant
    // if n.d = 0, the ray is perpendicular to the plane
    if (glm::dot(normal, d) == 0){ return -1; }

    Vec3 p1 { p0 + d };
    return { glm::dot(normal, point - p0) / glm::dot(normal, p1 - p0) };
}



Sphere::Sphere(
    Vec3 pos, float r,
    Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    if (r <= 0.0)
        throw std::invalid_argument("r must be >0.0");

    this->pos = pos;
    this->r = r;
}


Vec3 Sphere::get_normal(Vec3 point)
{
    return (point != pos) ? glm::normalize(point - pos) : Vec3 { 0.0 };
}


float Sphere::check_collision(Vec3 p0, Vec3 d)
{
    Vec3 p_dif { p0 - pos };
    float a, b, c, radicand;
    a = 1.0;
    b = 2.0 * (d.x * p_dif.x + d.y * p_dif.y + d.z * p_dif.z);
    c = pow(p_dif.x, 2.0) + pow(p_dif.y, 2.0) + pow(p_dif.z, 2.0) - pow(r, 2.0);

    float t, t0, t1;
    double sqrt_rad;
    radicand = pow(b, 2.0) - (4.0 * a * c);
    if (radicand < 0.0)
    {
        t = -1000.0; // TODO: Replace with a constant
    }
    else
    {
        sqrt_rad = sqrt(radicand);
        t0 = (-b + sqrt_rad) / 2.0;
        t1 = (-b - sqrt_rad) / 2.0;

        if (t0 + BIAS < 0.0) {
            // t0 is behind the ray
            t = t1;
        }
        else if (t1 + BIAS < 0.0) {
            // t1 is behind the ray
            t = t0;
        }
        else {
            // Both are in front, find the closest
            t = fmin(t0, t1);
        }
    }

    return t;
}



Mesh::Mesh(std::string filename, Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
Object::Object(amb, dif, spe, shi)
{
    last_col_normal = Vec3 { 0.0 };

    std::vector<int> indices;
    if (!loadOBJ(filename, vertices, normals, uvs, indices))
    {
        throw std::invalid_argument("Invalid input file");
    }
}



Mesh::Mesh(std::vector<Vec3> vertices, Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
Object::Object(amb, dif, spe, shi)
{
    last_col_normal = Vec3 { 0.0 };
    if (vertices.size() % 3 == 0)
    {
        this->vertices = std::vector<Vec3>{ vertices };

        Vec3 v[3];
        v[0] = vertices[0];
        v[1] = vertices[1];
        v[2] = vertices[2];

        Vec3 u, w;
        u = v[1] - v[0];
        w = v[2] - v[0];
        last_col_normal = glm::cross(u, w);
    }

}



Vec3 Mesh::get_normal(Vec3 point)
{ 
    return last_col_normal; 
}



float Mesh::check_collision(Vec3 p0, Vec3 d)
{
    float t0 { std::numeric_limits<float>::infinity() };

    Vec3 vertex[3];
    Vec3 u, v, w, normal, p1, p_col;
    float denom, uu, vv, uv, wv, wu;
    float t_plane_col, s_tri_col, t_tri_col;

    for (unsigned int i = 0; i < vertices.size(); i += 3)
    {
        vertex[0] = vertices[i];
        vertex[1] = vertices[i+1];
        vertex[2] = vertices[i+2];

        u = vertex[1] - vertex[0];
        v = vertex[2] - vertex[0];

        normal = glm::cross(u, v);

        p1 = p0 + d;
        t_plane_col = glm::dot(normal, vertex[0] - p0) / glm::dot(normal, p1 - p0);

        if (t_plane_col > 0.0 && t_plane_col < t0)
        {
            p_col = p0 + d * t_plane_col;
            w = p_col - vertex[0];

            uu = glm::dot(u, u);
            vv = glm::dot(v, v);
            uv = glm::dot(u, v);
            wv = glm::dot(w, v);
            wu = glm::dot(w, u);
            denom = (uv * uv) - (uu * vv);

            s_tri_col = (uv * wv - vv * wu) / denom;
            t_tri_col = (uv * wu - uu * wv) / denom;

            if (s_tri_col >= 0 && t_tri_col >= 0 && (s_tri_col + t_tri_col) <= 1)
            {
                t0 = t_plane_col;
                last_col_normal = normal;
            }
        }
    }

    if (t0 < std::numeric_limits<float>::infinity())
        return t0;
    else
        return -1.0;
}