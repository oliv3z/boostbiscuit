
#ifndef VEC3_H
#define VEC3_H

#include <complex>


struct vec3 {
    double x = 0.0f;
    double y = 0.0f;
    double z = 0.0f;

    vec3 operator + (const vec3& val) const {
        return vec3 (x + val.x, y + val.y, z + val.z);
    }

    vec3 operator - (const vec3& val) const {
        return vec3 (x - val.x, y - val.y, z - val.z);
    }

    vec3 operator * (const float& val) const {
        return vec3 (x * val, y * val, z * val);
    }

    [[nodiscard]] double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] double distance(const vec3& val) const {
        return (*this - val).length();
    }
};


#endif //VEC3_H
