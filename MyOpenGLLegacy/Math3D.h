#pragma once
#include <cmath>

struct Vec3 {
    float x = 0, y = 0, z = 0;
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) { return { a.x + b.x,a.y + b.y,a.z + b.z }; }
inline Vec3 operator-(const Vec3& a, const Vec3& b) { return { a.x - b.x,a.y - b.y,a.z - b.z }; }
inline Vec3 operator*(const Vec3& a, float s) { return { a.x * s,a.y * s,a.z * s }; }
inline Vec3 operator/(const Vec3& a, float s) { return { a.x / s,a.y / s,a.z / s }; }

inline float dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float length(const Vec3& v) { return std::sqrt(dot(v, v)); }

inline Vec3 normalize(const Vec3& v) {
    float l = length(v);
    if (l < 1e-6f) return { 0,0,0 };
    return v / l;
}

inline float clampf(float v, float a, float b) {
    return (v < a) ? a : (v > b) ? b : v;
}

inline float deg2rad(float d) { return d * 3.1415926535f / 180.0f; }
