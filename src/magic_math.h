#ifndef MAGIC_MATH_H_
#define MAGIC_MATH_H_

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RADIANS(d) ((float)(d) * ((float)M_PI / 180.0f))
#define DEGREES(r) ((float)(r) * (180.0f / (float)M_PI))

#define EPSILON 0.001f
#define EQUAL_FLOAT(a, b) (fabs((a) - (b)) < EPSILON)

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

float
Clamp(float x, float min, float max)
{
    float result = x;
    if(x <= min) result = min;
    else if(x >= max) result = max;
    return result;
}

typedef union
{
    struct
    {
        float x, y, z;
    };

    float v[3];
} V3;

static V3
MakeV3(float x, float y, float z)
{
    V3 result = (V3){ x, y, z };
    return result;
}

static bool
IsEqualV3(V3 a, V3 b)
{
    return (EQUAL_FLOAT(a.x, b.x) &&
            EQUAL_FLOAT(a.y, b.y) &&
            EQUAL_FLOAT(a.z, b.z));
}

static V3
MakeNormalizedV3(float x, float y, float z)
{
    V3 result = (V3){ x, y, z };
    float length2 = x * x + y * y + z * z;
    if(length2 > 0)
    {
        float length = sqrt(length2);
        result.x = x / length;
        result.y = y / length;
        result.z = z / length;
    }

    return result;
}

static V3
NormalizeV3(V3 v)
{
    V3 result = v;
    float length2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if(length2 > 0)
    {
        float length = sqrt(length2);
        result.x = v.x / length;
        result.y = v.y / length;
        result.z = v.z / length;
    }

    return result;
}

static float
MagnitudeV3(V3 v)
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

static float
MagnitudeSquaredV3(V3 v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

static V3
NegateV3(V3 v)
{
    V3 result = (V3){ -v.x, -v.y, -v.z };
    return result;
}

static V3
AddV3(V3 a, V3 b)
{
    V3 result = (V3){ a.x + b.x, a.y + b.y, a.z + b.z };
    return result;
}

static V3
SubV3(V3 a, V3 b)
{
    V3 result = (V3){ a.x - b.x, a.y - b.y, a.z - b.z };
    return result;
}

static V3
SumV3(V3 *v, size_t count)
{
    V3 result = (V3){ 0, 0, 0 };
    for(size_t i=0; i<count; ++i)
    {
        result.x += v[i].x;
        result.y += v[i].y;
        result.z += v[i].z;
    }

    return result;
}

static V3
ScaleV3(V3 v, float scale)
{
    V3 result = (V3){ v.x * scale, v.y * scale, v.z * scale };
    return result;
}

static V3
CrossV3(V3 a, V3 b)
{
    V3 result = (V3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };

    return result;
}

static float
DotV3(V3 a, V3 b)
{
    float result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

/*
 * Matrices are as follows:
 * Row-by-row in memory.
 * We use column vectors.
 */
typedef union
{
    struct
    {
        float f00, f01, f02, f03;
        float f10, f11, f12, f13;
        float f20, f21, f22, f23;
        float f30, f31, f32, f33;
    };

    float v[4*4];
} Mat4;

static bool
IsEqualMat4(Mat4 a, Mat4 b)
{
    for(size_t i=0; i<4*4; ++i)
    {
        if(a.v[i] != b.v[i]) return false;
    }

    return true;
}

static Mat4
MulMat4(Mat4 a, Mat4 b)
{
    Mat4 result;
    result.f00 = a.f00*b.f00 + a.f01*b.f10 + a.f02*b.f20 + a.f03*b.f30;
    result.f01 = a.f00*b.f01 + a.f01*b.f11 + a.f02*b.f21 + a.f03*b.f31;
    result.f02 = a.f00*b.f02 + a.f01*b.f12 + a.f02*b.f22 + a.f03*b.f32;
    result.f03 = a.f00*b.f03 + a.f01*b.f13 + a.f02*b.f23 + a.f03*b.f33;
    result.f10 = a.f10*b.f00 + a.f11*b.f10 + a.f12*b.f20 + a.f13*b.f30;
    result.f11 = a.f10*b.f01 + a.f11*b.f11 + a.f12*b.f21 + a.f13*b.f31;
    result.f12 = a.f10*b.f02 + a.f11*b.f12 + a.f12*b.f22 + a.f13*b.f32;
    result.f13 = a.f10*b.f03 + a.f11*b.f13 + a.f12*b.f23 + a.f13*b.f33;
    result.f20 = a.f20*b.f00 + a.f21*b.f10 + a.f22*b.f20 + a.f23*b.f30;
    result.f21 = a.f20*b.f01 + a.f21*b.f11 + a.f22*b.f21 + a.f23*b.f31;
    result.f22 = a.f20*b.f02 + a.f21*b.f12 + a.f22*b.f22 + a.f23*b.f32;
    result.f23 = a.f20*b.f03 + a.f21*b.f13 + a.f22*b.f23 + a.f23*b.f33;
    result.f30 = a.f30*b.f00 + a.f31*b.f10 + a.f32*b.f20 + a.f33*b.f30;
    result.f31 = a.f30*b.f01 + a.f31*b.f11 + a.f32*b.f21 + a.f33*b.f31;
    result.f32 = a.f30*b.f02 + a.f31*b.f12 + a.f32*b.f22 + a.f33*b.f32;
    result.f33 = a.f30*b.f03 + a.f31*b.f13 + a.f32*b.f23 + a.f33*b.f33;
    return result;
}

static V3
MulMat4Vec3(Mat4 m, V3 v)
{
    // This function assumes a fouth component to v as v.w = 1.0
    V3 result;
    result.x = m.f00*v.x + m.f10*v.y + m.f20*v.z + m.f30;
    result.y = m.f01*v.x + m.f11*v.y + m.f21*v.z + m.f31;
    result.z = m.f02*v.x + m.f12*v.y + m.f22*v.z + m.f32;
    return result;
}

static Mat4
IdentityMat4()
{
    Mat4 result;
    result.f01 = result.f02 = result.f03 = 0.0f;
    result.f10 = result.f12 = result.f13 = 0.0f;
    result.f20 = result.f21 = result.f23 = 0.0f;
    result.f30 = result.f31 = result.f32 = 0.0f;
    result.f00 = result.f11 = result.f22 = result.f33 = 1.0f;
    return result;
}

static Mat4
TransposeMat4(const Mat4 *m)
{
    Mat4 r;
    r.f00 = m->f00; r.f01 = m->f10; r.f02 = m->f20; r.f03 = m->f30;
    r.f10 = m->f01; r.f11 = m->f11; r.f12 = m->f21; r.f13 = m->f31;
    r.f20 = m->f02; r.f21 = m->f12; r.f22 = m->f22; r.f23 = m->f32;
    r.f30 = m->f03; r.f31 = m->f13; r.f32 = m->f23; r.f33 = m->f33;
    return r;
}

static Mat4
InvertMat4(const Mat4 *m)
{
    Mat4 inv;

    inv.v[0] = m->v[5]  * m->v[10] * m->v[15] -
               m->v[5]  * m->v[11] * m->v[14] -
               m->v[9]  * m->v[6]  * m->v[15] +
               m->v[9]  * m->v[7]  * m->v[14] +
               m->v[13] * m->v[6]  * m->v[11] -
               m->v[13] * m->v[7]  * m->v[10];

    inv.v[4] = -m->v[4]  * m->v[10] * m->v[15] +
                m->v[4]  * m->v[11] * m->v[14] +
                m->v[8]  * m->v[6]  * m->v[15] -
                m->v[8]  * m->v[7]  * m->v[14] -
                m->v[12] * m->v[6]  * m->v[11] +
                m->v[12] * m->v[7]  * m->v[10];

    inv.v[8] = m->v[4]  * m->v[9] *  m->v[15] -
               m->v[4]  * m->v[11] * m->v[13] -
               m->v[8]  * m->v[5] *  m->v[15] +
               m->v[8]  * m->v[7] *  m->v[13] +
               m->v[12] * m->v[5] *  m->v[11] -
               m->v[12] * m->v[7] *  m->v[9];

    inv.v[12] = -m->v[4]  * m->v[9] *  m->v[14] +
                 m->v[4]  * m->v[10] * m->v[13] +
                 m->v[8]  * m->v[5] *  m->v[14] -
                 m->v[8]  * m->v[6] *  m->v[13] -
                 m->v[12] * m->v[5] *  m->v[10] +
                 m->v[12] * m->v[6] *  m->v[9];

    inv.v[1] = -m->v[1]  * m->v[10] * m->v[15] +
                m->v[1]  * m->v[11] * m->v[14] +
                m->v[9]  * m->v[2] * m->v[15] -
                m->v[9]  * m->v[3] * m->v[14] -
                m->v[13] * m->v[2] * m->v[11] +
                m->v[13] * m->v[3] * m->v[10];

    inv.v[5] = m->v[0]  * m->v[10] * m->v[15] -
               m->v[0]  * m->v[11] * m->v[14] -
               m->v[8]  * m->v[2] * m->v[15] +
               m->v[8]  * m->v[3] * m->v[14] +
               m->v[12] * m->v[2] * m->v[11] -
               m->v[12] * m->v[3] * m->v[10];

    inv.v[9] = -m->v[0]  * m->v[9] * m->v[15] +
                m->v[0]  * m->v[11] * m->v[13] +
                m->v[8]  * m->v[1] * m->v[15] -
                m->v[8]  * m->v[3] * m->v[13] -
                m->v[12] * m->v[1] * m->v[11] +
                m->v[12] * m->v[3] * m->v[9];

    inv.v[13] = m->v[0]  * m->v[9] * m->v[14] -
                m->v[0]  * m->v[10] * m->v[13] -
                m->v[8]  * m->v[1] * m->v[14] +
                m->v[8]  * m->v[2] * m->v[13] +
                m->v[12] * m->v[1] * m->v[10] -
                m->v[12] * m->v[2] * m->v[9];

    inv.v[2] = m->v[1]  * m->v[6] * m->v[15] -
               m->v[1]  * m->v[7] * m->v[14] -
               m->v[5]  * m->v[2] * m->v[15] +
               m->v[5]  * m->v[3] * m->v[14] +
               m->v[13] * m->v[2] * m->v[7] -
               m->v[13] * m->v[3] * m->v[6];

    inv.v[6] = -m->v[0]  * m->v[6] * m->v[15] +
                m->v[0]  * m->v[7] * m->v[14] +
                m->v[4]  * m->v[2] * m->v[15] -
                m->v[4]  * m->v[3] * m->v[14] -
                m->v[12] * m->v[2] * m->v[7] +
                m->v[12] * m->v[3] * m->v[6];

    inv.v[10] = m->v[0]  * m->v[5] * m->v[15] -
                m->v[0]  * m->v[7] * m->v[13] -
                m->v[4]  * m->v[1] * m->v[15] +
                m->v[4]  * m->v[3] * m->v[13] +
                m->v[12] * m->v[1] * m->v[7] -
                m->v[12] * m->v[3] * m->v[5];

    inv.v[14] = -m->v[0]  * m->v[5] * m->v[14] +
                 m->v[0]  * m->v[6] * m->v[13] +
                 m->v[4]  * m->v[1] * m->v[14] -
                 m->v[4]  * m->v[2] * m->v[13] -
                 m->v[12] * m->v[1] * m->v[6] +
                 m->v[12] * m->v[2] * m->v[5];

    inv.v[3] = -m->v[1] * m->v[6] * m->v[11] +
                m->v[1] * m->v[7] * m->v[10] +
                m->v[5] * m->v[2] * m->v[11] -
                m->v[5] * m->v[3] * m->v[10] -
                m->v[9] * m->v[2] * m->v[7] +
                m->v[9] * m->v[3] * m->v[6];

    inv.v[7] = m->v[0] * m->v[6] * m->v[11] -
               m->v[0] * m->v[7] * m->v[10] -
               m->v[4] * m->v[2] * m->v[11] +
               m->v[4] * m->v[3] * m->v[10] +
               m->v[8] * m->v[2] * m->v[7] -
               m->v[8] * m->v[3] * m->v[6];

    inv.v[11] = -m->v[0] * m->v[5] * m->v[11] +
                 m->v[0] * m->v[7] * m->v[9] +
                 m->v[4] * m->v[1] * m->v[11] -
                 m->v[4] * m->v[3] * m->v[9] -
                 m->v[8] * m->v[1] * m->v[7] +
                 m->v[8] * m->v[3] * m->v[5];

    inv.v[15] = m->v[0] * m->v[5] * m->v[10] -
                m->v[0] * m->v[6] * m->v[9] -
                m->v[4] * m->v[1] * m->v[10] +
                m->v[4] * m->v[2] * m->v[9] +
                m->v[8] * m->v[1] * m->v[6] -
                m->v[8] * m->v[2] * m->v[5];

    double det = m->v[0] * inv.v[0] + m->v[1] * inv.v[4] + m->v[2] * inv.v[8] + m->v[3] * inv.v[12];

    Mat4 result = IdentityMat4();

    if(det != 0)
    {
        det = 1.0 / det;

        for(int i = 0; i < 16; ++i)
        {
            result.v[i] = inv.v[i] * det;
        }
    }

    return result;
}

static Mat4
PerspectiveMat4(float aspect, float fovy, float near, float far)
{
    float const tan_half_fov_y = tan(RADIANS(fovy) / 2.0f);

    Mat4 result = IdentityMat4();

    result.f01 = result.f02 = result.f03 = 0.0f;
    result.f10 = result.f12 = result.f13 = 0.0f;
    result.f20 = result.f21 = 0.0f;
    result.f30 = result.f31 = 0.0f;

    result.f00 = 1.0f / (aspect * tan_half_fov_y);
    result.f11 = 1.0f / tan_half_fov_y;
    result.f22 = far / (near - far);
    result.f23 = -1.0f;
    result.f32 = -(far * near) / (far - near);
    result.f33 = 1.0f;

    return result;
}

static Mat4
OrthographicMat4(float left, float right, float bottom, float top, float near, float far)
{
    Mat4 result;
    result.f01 = result.f02 = result.f03 = 0.0f;
    result.f10 = result.f12 = result.f12 = 0.0f;
    result.f20 = result.f21 = result.f23 = 0.0f;
    result.f00 =  2.0f / (right - left);
    result.f11 =  2.0f / (top - bottom);
    result.f22 = -2.0f / (far - near);
    result.f30 = -(right + left) / (right - left);
    result.f31 = -(top + bottom) / (top - bottom);
    result.f32 = -(far + near) / (far - near);
    result.f33 = 1.0f;
    return result;
}

static Mat4
LookAtMat4(V3 eye, V3 target, V3 up)
{
    Mat4 result;

    V3 forward = NormalizeV3(SubV3(target, eye));
    V3 right = NormalizeV3(CrossV3(forward, up));
    V3 actual_up = NormalizeV3(CrossV3(right, forward));

    result = IdentityMat4();
    result.f00 = right.x;
    result.f10 = right.y;
    result.f20 = right.z;
    result.f01 = actual_up.x;
    result.f11 = actual_up.y;
    result.f21 = actual_up.z;
    result.f02 =-forward.x;
    result.f12 =-forward.y;
    result.f22 =-forward.z;
    result.f30 =-DotV3(right, eye);
    result.f31 =-DotV3(actual_up, eye);
    result.f32 = DotV3(forward, eye);

    return result;
}

static Mat4
TranslationMat4(V3 v)
{
    Mat4 result;
    result.f00 = result.f11 = result.f22 = result.f33 = 1.0f;
    result.f01 = result.f02 = result.f03 = 0.0f;
    result.f10 = result.f12 = result.f13 = 0.0f;
    result.f20 = result.f21 = result.f23 = 0.0f;
    result.f30 = result.f31 = result.f32 = 0.0f;
    result.f30 = v.x;
    result.f31 = v.y;
    result.f32 = v.z;
    return result;
}

static Mat4
ScaleMat4(V3 v)
{
    Mat4 result;
    result.f00 = v.x;
    result.f11 = v.y;
    result.f22 = v.z;
    result.f33 = 1.0f;
    result.f01 = result.f02 = result.f03 = 0.0f;
    result.f10 = result.f12 = result.f13 = 0.0f;
    result.f20 = result.f21 = result.f23 = 0.0f;
    result.f30 = result.f31 = result.f32 = 0.0f;
    return result;
}

static Mat4
RotateMat4(float pitch, float yaw, float roll)
{

    /*
     * 2D rotation:
     * c -s
     * s  c
     *
     * x-rotation:
     *   0   0   0
     *   0  cx -sx
     *   0  sx  cx
     *
     * y-rotation
     *   cy  0  sy
     *    0  0   0
     *  -sy  0  cy
     *
     * z-rotation
     *  cz -sz   0
     *  sz  cz   0
     *   0   0   0
     */

    float cx = cos(pitch);
    float sx = sin(pitch);
    float cy = cos(yaw);
    float sy = sin(yaw);
    float cz = cos(roll);
    float sz = sin(roll);

    // TODO(istarnion): Inline this!
    Mat4 x = {.v = {
          1,   0,   0, 0,
          0,  cx, -sx, 0,
          0,  sx,  cx, 0,
          0,   0,   0, 1
    }};

    Mat4 y = {.v = {
         cy,   0,  sy, 0,
          0,   1,   0, 0,
        -sy,   0,  cy, 0,
          0,   0,   0, 1
    }};

    Mat4 z = {.v = {
         cz, -sz,   0, 0,
         sz,  cz,   0, 0,
          0,   0,   1, 0,
          0,   0,   0, 1
    }};

    Mat4 result = MulMat4(MulMat4(y, x), z);
    return result;
}

static Mat4
TransformMat4(V3 pos, V3 scale, V3 euler)
{
    Mat4 t = TranslationMat4(pos);
    Mat4 s = ScaleMat4(scale);
    Mat4 r = RotateMat4(euler.x, euler.y, euler.z);

    // order: Scale, rotation, translation
    // TODO(istarnion): Inline this!
    Mat4 result = MulMat4(MulMat4(s, r), t);
    return result;
}

static void
DecomposeMat4(const Mat4 &m, V3 *pos, V3 *rot, V3 *scale)
{
    if(pos)
    {
        pos->x = m.f30;
        pos->y = m.f31;
        pos->z = m.f32;
    }

    if(rot)
    {
        // TODO(istarnion): Implement!
    }

    if(scale)
    {
        scale->x = m.f00;
        scale->y = m.f11;
        scale->z = m.f22;
    }
}

#ifdef __cplusplus
}
#endif
#endif /* end of include guard: MAGIC_MATH_H_ */

