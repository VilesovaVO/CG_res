#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include <cmath>

class Camera {
private:
    Vec3f eye;      // camera pos
    Vec3f target;   // point to look
    Vec3f up;
    float fov;      // degrees
    float aspect;   // ratio
    float znear;
    float zfar;

public:
    Camera(Vec3f e = Vec3f(0, 0, 3),
        Vec3f t = Vec3f(0, 0, 0),
        Vec3f u = Vec3f(0, 1, 0),
        float f = 45.0f,
        float a = 1.0f,
        float n = 0.1f,
        float ff = 100.0f)
        : eye(e), target(t), up(u), fov(f), aspect(a), znear(n), zfar(ff) {
        up.normalize();
    }

    Matrix getViewMatrix() {
        Vec3f z = (eye - target).normalize();
        Vec3f x = cross(up, z).normalize();
        Vec3f y = cross(z, x).normalize();

        Matrix view = Matrix::identity(4);

        view[0][0] = x.x; view[0][1] = x.y; view[0][2] = x.z;
        view[1][0] = y.x; view[1][1] = y.y; view[1][2] = y.z;
        view[2][0] = z.x; view[2][1] = z.y; view[2][2] = z.z;

        view[0][3] = -dot(x, eye);
        view[1][3] = -dot(y, eye);
        view[2][3] = -dot(z, eye);

        return view;
    }

    Matrix getProjectionMatrix() {
        Matrix proj = Matrix::identity(4);

        float tanHalfFov = tan(fov * 3.14159265f / 360.0f);
        float range = znear - zfar;

        proj[0][0] = 1.0f / (aspect * tanHalfFov);
        proj[1][1] = 1.0f / tanHalfFov;
        proj[2][2] = (-znear - zfar) / range;
        proj[2][3] = 2.0f * zfar * znear / range;
        proj[3][2] = 1.0f;
        proj[3][3] = 0.0f;

        return proj;
    }

    Matrix getViewProjectionMatrix() {
        return getProjectionMatrix() * getViewMatrix();
    }

    Vec3f getEye() const { return eye; }
    Vec3f getTarget() const { return target; }
    Vec3f getUp() const { return up; }
    float getFov() const { return fov; }
    float getAspect() const { return aspect; }
    float getZNear() const { return znear; }
    float getZFar() const { return zfar; }

private:
    float dot(const Vec3f& a, const Vec3f& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3f cross(const Vec3f& a, const Vec3f& b) {
        return Vec3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
};

#endif // CAMERA_H
