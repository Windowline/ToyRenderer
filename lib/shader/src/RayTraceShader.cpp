#include "RayTraceShader.hpp"

const char* vertexRayTrace = R(
        layout (location = 0) in vec2 a_position;
        layout (location = 1) in vec2 a_texCoord;
        out vec2 v_uv;

        void main() {
            v_uv = a_texCoord;
            gl_Position = vec4(a_position, 0.0, 1.0);
        }
);

const char* fragmentRayTrace = R(
        struct Ray {
            vec3 org;
            vec3 dir;
        };

        struct Material {
            vec4 color;
        };

        struct Hit {
            bool didHit;
            float dst;
            vec3 pos;
            vec3 normal;
            Material mat;
        };

        struct Sphere {
            vec3 pos;
            float r;
            Material mat;
        };

        Hit raySphere(Ray ray, vec3 sphereCenter, float sphereRadius) {
            Hit hitInfo;
            hitInfo.didHit = false;

            vec3 oc = ray.org - sphereCenter;

            float a = dot(ray.dir, ray.dir);
            float b = 2.0 * dot(oc, ray.dir);
            float c = dot(oc, oc) - sphereRadius * sphereRadius;
            float discriminant = b * b - 4.0 * a * c;

            if (discriminant >= 0.0) {
                float dst = (-b - sqrt(discriminant)) / (2.0 * a);

                if (dst >= 0.0) {
                    hitInfo.didHit = true;
                    hitInfo.dst = dst;
                    hitInfo.pos = ray.org + ray.dir * dst;
                    hitInfo.normal = normalize(hitInfo.pos - sphereCenter);
                }
            }

            return hitInfo;
        }

        Hit rayCollision(Ray ray) {
            int numSphere = 2;
            Sphere spheres[2];

            spheres[0].pos = vec3(0.0, 0.0, -40.0);
            spheres[0].r = 25.0;
            spheres[0].mat.color = vec4(1.0, 1.0, 1.0, 1.0);

            spheres[1].pos = vec3(0.0, 0.0, 0.0);
            spheres[1].r = 5.0;
            spheres[1].mat.color = vec4(1.0, 0.0, 0.0, 1.0);

            Hit closestHit;
            closestHit.didHit = false;
            closestHit.dst = 100.0;
            closestHit.mat.color = vec4(0.0, 0.0, 0.0, 1.0);

            for (int i = 0; i < numSphere; ++i) {
                Sphere sphere = spheres[i];
                Hit hit = raySphere(ray, sphere.pos, sphere.r);

                if (hit.didHit && hit.dst < closestHit.dst) {
                    closestHit = hit;
                    closestHit.mat = sphere.mat;
                }
            }

            return closestHit;
        }

        uniform vec2 u_resolution;
        uniform mat4 u_cameraLocalToWorldMat;
        uniform vec3 u_worldCameraPos;
        layout (location = 0) out vec4 fragColor;
        in vec2 v_uv;

        void main() {
            mat4 tmp2 = u_cameraLocalToWorldMat;

            vec2 uv = v_uv * 2.0 - 1.0;
            float aspect = u_resolution.x / u_resolution.y;
            uv.x *= aspect;

            Ray ray;
            ray.org = u_worldCameraPos;
            ray.dir = normalize(vec3(uv, -1.0));

            Hit closestHit = rayCollision(ray);
            fragColor = closestHit.mat.color;
        }
);

RayTraceShader::RayTraceShader() {
    this->load();
    basicUniformLoc();

    _cameraPosUniformLoc = glGetUniformLocation(_programID, "u_worldCameraPos");
    _cameraLocalToWorldMatUniformLoc = glGetUniformLocation(_programID, "u_cameraLocalToWorldMat");
    _resolutionUnifromLoc = glGetUniformLocation(_programID, "u_resolution");
}

bool RayTraceShader::load() {
    string vShader = string("#version 330 core \n") + string(vertexRayTrace);
    string fShader = string("#version 330 core \n") + string(fragmentRayTrace);

    _programID = loadProgram(reinterpret_cast<const char *>(vShader.c_str()),
                             reinterpret_cast<const char *>(fShader.c_str()));

    assert(_programID != 0);
    return true;
}

void RayTraceShader::useProgram() {
    glUseProgram(_programID);
}