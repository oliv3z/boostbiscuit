
#ifndef VEC4_H
#define VEC4_H


struct vec4 {
    double x = 0.0f;
    double y = 0.0f;
    double z = 0.0f;
    double w = 0.0f;

    vec4 operator + (const vec4& val) const {
        return vec4 (x + val.x, y + val.y, z + val.z, w + val.w);
    }

    vec4 operator - (const vec4& val) const {
        return vec4 (x - val.x, y - val.y, z - val.z, w - val.w);
    }

    vec4 operator * (const float& val) const {
        return vec4 (x * val, y * val, z * val, w * val);
    }
};


#endif //VEC4_H
