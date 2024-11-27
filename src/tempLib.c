#include "tempLib.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0f)

void print_vec(vec4 vec) {
    printf("[%7.4f, \t %7.4f, \t %7.4f, \t %7.4f]\n", vec.x, vec.y, vec.z, vec.w);
}

mat4 create_identity_mat(){
    vec4 x = {1.0,0.0,0.0,0.0};
    vec4 y = {0.0, 1.0,0.0,0.0};
    vec4 z = {0.0,0.0,1.0,0.0};
    vec4 w = {0.0,0.0,0.0,1.0};
    mat4 result = {x,y,z,w};
    return result;
}

vec4 transformation(mat4 R, mat4 S, vec4 T, vec4 V){
    mat4 upper = mat_mul_mat(R,S);
    upper.w = T;
    upper.w.w = 1.0;
    return mat_mul_vec(upper, V);
}

mat4 create_rotate_mat(float yawRad, float pitchRad, float rollRad) {
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);
    float cosPitch = cosf(pitchRad);
    float sinPitch = sinf(pitchRad);
    float cosRoll = cosf(rollRad);
    float sinRoll = sinf(rollRad);

    mat4 rotX = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, cosPitch, -sinPitch, 0.0f},
        {0.0f, sinPitch, cosPitch, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    mat4 rotY = {
        {cosYaw, 0.0f, sinYaw, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {-sinYaw, 0.0f, cosYaw, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    mat4 rotZ = {
        {cosRoll, sinRoll, 0.0f, 0.0f},
        {-sinRoll, cosRoll, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    mat4 result = mat_mul_mat(rotZ, mat_mul_mat(rotY, rotX));

    return result;
}

mat4 get_transfom_mat(float scale, vec4 translation, mat4 orignal_mat){
    mat4 translation_mat = create_identity_mat();
    translation_mat.w = translation;

    mat4 scale_mat = create_scale_mat(scale,scale,scale);
    mat4 result = mat_mul_mat(orignal_mat,scale_mat);

    return mat_mul_mat(result,translation_mat);

}


mat4 create_scale_mat(float x, float y, float z){
    vec4 a = {1.0*x,0.0,0.0,0.0};
    vec4 b = {0.0,1.0*y,0.0,0.0};
    vec4 c = {0.0,0.0,1.0*z,0.0};
    vec4 d = {0.0,0.0,0.0,1.0};

    mat4 result = {a,b,c,d};
    return result;
}

vec4 vec_mul_scalar(GLfloat scale, vec4 vec) {

    vec4 result = {vec.x * scale,
                   vec.y * scale,
                   vec.z * scale,
                   vec.w};
    return result;
}

vec4 vec_add(vec4 v1, vec4 v2) {
    vec4 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    assert(result.w == 0.0);
    return result;
}

vec4 vec_sub(vec4 v1, vec4 v2) {
    vec4 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    assert(result.w == 0.0);
    return result;
}

GLfloat vec_norm2(vec4 v) {

    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z );
}

vec4 vec_standardlize(vec4 v) {
    GLfloat m = vec_norm2(v);
    vec4 result = v;
    if (m == 0.0) m = 1.0;
    result = vec_mul_scalar(1/m,result);    
    return result;
}

GLfloat vec_dot(vec4 v1, vec4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4 vec_cross(vec4 v1, vec4 v2) {
    vec4 result;
    result.x = (v1.y * v2.z) - (v1.z * v2.y);
    result.y = (v1.z * v2.x) - (v1.x * v2.z);
    result.z = (v1.x * v2.y) - (v1.y * v2.x);
    result.w = 0.0;
    return result;
}


void print_mat(mat4 m) {
    // print a 4*4 matrix
    // with 4 point precision
    printf(
        "[%7.4f\t %7.4f\t %7.4f\t %7.4f]\n[%7.4f\t %7.4f\t %7.4f\t %7.4f]\n[%7.4f\t %7.4f\t %7.4f\t %7.4f]\n[%7.4f\t %7.4f\t %7.4f\t %7.4f]\n",
        m.x.x, m.y.x, m.z.x, m.w.x,
        m.x.y, m.y.y, m.z.y, m.w.y,
        m.x.z, m.y.z, m.z.z, m.w.z,
        m.x.w, m.y.w, m.z.w, m.w.w
    );
}


mat4 mat_mul_scalar(GLfloat s, mat4 m) {
    mat4 result = m;
    vec4 v1 = vec_mul_scalar(s,result.x);
    vec4 v2 = vec_mul_scalar(s,result.y);
    vec4 v3 = vec_mul_scalar(s,result.z);
    vec4 v4 = vec_mul_scalar(s,result.w);
    mat4 m2 = {v1,v2,v3,v4};
    return m2;
}

mat4 mat_add(mat4 m1, mat4 m2) {
    mat4 result;
    
    result.x.x = m1.x.x + m2.x.x;
    result.x.y = m1.x.y + m2.x.y;
    result.x.z = m1.x.z + m2.x.z;
    result.x.w = m1.x.w + m2.x.w;

    result.y.x = m1.y.x + m2.y.x;
    result.y.y = m1.y.y + m2.y.y;
    result.y.z = m1.y.z + m2.y.z;
    result.y.w = m1.y.w + m2.y.w;

    result.z.x = m1.z.x + m2.z.x;
    result.z.y = m1.z.y + m2.z.y;
    result.z.z = m1.z.z + m2.z.z;
    result.z.w = m1.z.w + m2.z.w;

    result.w.x = m1.w.x + m2.w.x;
    result.w.y = m1.w.y + m2.w.y;
    result.w.z = m1.w.z + m2.w.z;
    result.w.w = m1.w.w + m2.w.w;

    return result;
}

mat4 mat_sub(mat4 m1, mat4 m2) {
    mat4 result;
    
    result.x.x = m1.x.x - m2.x.x;
    result.x.y = m1.x.y - m2.x.y;
    result.x.z = m1.x.z - m2.x.z;
    result.x.w = m1.x.w - m2.x.w;

    result.y.x = m1.y.x - m2.y.x;
    result.y.y = m1.y.y - m2.y.y;
    result.y.z = m1.y.z - m2.y.z;
    result.y.w = m1.y.w - m2.y.w;

    result.z.x = m1.z.x - m2.z.x;
    result.z.y = m1.z.y - m2.z.y;
    result.z.z = m1.z.z - m2.z.z;
    result.z.w = m1.z.w - m2.z.w;

    result.w.x = m1.w.x - m2.w.x;
    result.w.y = m1.w.y - m2.w.y;
    result.w.z = m1.w.z - m2.w.z;
    result.w.w = m1.w.w - m2.w.w;

    return result;
}

mat4 mat_mul_mat(mat4 m1, mat4 m2) {

    mat4 result;


    result.x.x = m1.x.x * m2.x.x + m1.y.x * m2.x.y + m1.z.x * m2.x.z + m1.w.x * m2.x.w;
    result.x.y = m1.x.y * m2.x.x + m1.y.y * m2.x.y + m1.z.y * m2.x.z + m1.w.y * m2.x.w;
    result.x.z = m1.x.z * m2.x.x + m1.y.z * m2.x.y + m1.z.z * m2.x.z + m1.w.z * m2.x.w;
    result.x.w = m1.x.w * m2.x.x + m1.y.w * m2.x.y + m1.z.w * m2.x.z + m1.w.w * m2.x.w;

    result.y.x = m1.x.x * m2.y.x + m1.y.x * m2.y.y + m1.z.x * m2.y.z + m1.w.x * m2.y.w;
    result.y.y = m1.x.y * m2.y.x + m1.y.y * m2.y.y + m1.z.y * m2.y.z + m1.w.y * m2.y.w;
    result.y.z = m1.x.z * m2.y.x + m1.y.z * m2.y.y + m1.z.z * m2.y.z + m1.w.z * m2.y.w;
    result.y.w = m1.x.w * m2.y.x + m1.y.w * m2.y.y + m1.z.w * m2.y.z + m1.w.w * m2.y.w;

    result.z.x = m1.x.x * m2.z.x + m1.y.x * m2.z.y + m1.z.x * m2.z.z + m1.w.x * m2.z.w;
    result.z.y = m1.x.y * m2.z.x + m1.y.y * m2.z.y + m1.z.y * m2.z.z + m1.w.y * m2.z.w;
    result.z.z = m1.x.z * m2.z.x + m1.y.z * m2.z.y + m1.z.z * m2.z.z + m1.w.z * m2.z.w;
    result.z.w = m1.x.w * m2.z.x + m1.y.w * m2.z.y + m1.z.w * m2.z.z + m1.w.w * m2.z.w;

    result.w.x = m1.x.x * m2.w.x + m1.y.x * m2.w.y + m1.z.x * m2.w.z + m1.w.x * m2.w.w;
    result.w.y = m1.x.y * m2.w.x + m1.y.y * m2.w.y + m1.z.y * m2.w.z + m1.w.y * m2.w.w;
    result.w.z = m1.x.z * m2.w.x + m1.y.z * m2.w.y + m1.z.z * m2.w.z + m1.w.z * m2.w.w;
    result.w.w = m1.x.w * m2.w.x + m1.y.w * m2.w.y + m1.z.w * m2.w.z + m1.w.w * m2.w.w;

    return result;
}

mat4 mat_transpose(mat4 m) {
    mat4 result;
    // straightfoward 

    result.x.x = m.x.x;
    result.x.y = m.y.x;
    result.x.z = m.z.x;
    result.x.w = m.w.x;

    result.y.x = m.x.y;
    result.y.y = m.y.y;
    result.y.z = m.z.y;
    result.y.w = m.w.y;

    result.z.x = m.x.z;
    result.z.y = m.y.z;
    result.z.z = m.z.z;
    result.z.w = m.w.z;

    result.w.x = m.x.w;
    result.w.y = m.y.w;
    result.w.z = m.z.w;
    result.w.w = m.w.w;

    return result;
}

vec4 mat_mul_vec(mat4 m, vec4 v) {

    mat4 m_row = mat_transpose(m);
    GLfloat x = vec_dot(m_row.x,v);
    GLfloat y = vec_dot(m_row.y,v);
    GLfloat z = vec_dot(m_row.z,v);
    GLfloat w = vec_dot(m_row.w,v);

    vec4 result = {x,y,z,w};

    return result;

}



GLfloat det_mat3( GLfloat a, GLfloat b, GLfloat c, 
                GLfloat d, GLfloat e, GLfloat f,
                GLfloat g, GLfloat h, GLfloat i) {
    return a*e*i + b*f*g + c*d*h - g*e*c - h*f*a - i*d*b;
}

mat4 mat_minor(mat4 m) {
    mat4 result;

    // this is the best solution I think 
    result.x.x = det_mat3(m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
    result.x.y = det_mat3(m.y.x, m.z.x, m.w.x, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
    result.x.z = det_mat3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.w, m.z.w, m.w.w);
    result.x.w = det_mat3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z);

    result.y.x = det_mat3(m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
    result.y.y = det_mat3(m.x.x, m.z.x, m.w.x, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
    result.y.z = det_mat3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.w, m.z.w, m.w.w);
    result.y.w = det_mat3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z);

    result.z.x = det_mat3(m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
    result.z.y = det_mat3(m.x.x, m.y.x, m.w.x, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
    result.z.z = det_mat3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.w, m.y.w, m.w.w);
    result.z.w = det_mat3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z);

    result.w.x = det_mat3(m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
    result.w.y = det_mat3(m.x.x, m.y.x, m.z.x, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
    result.w.z = det_mat3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.w, m.y.w, m.z.w);
    result.w.w = det_mat3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z);

    return result;
}

mat4 mat_cofactor(mat4 m) {
    mat4 result;


    result.x.x = +m.x.x;
    result.x.y = -m.x.y;
    result.x.z = +m.x.z;
    result.x.w = -m.x.w;

    result.y.x = -m.y.x;
    result.y.y = +m.y.y;
    result.y.z = -m.y.z;
    result.y.w = +m.y.w;

    result.z.x = +m.z.x;
    result.z.y = -m.z.y;
    result.z.z = +m.z.z;
    result.z.w = -m.z.w;

    result.w.x = -m.w.x;
    result.w.y = +m.w.y;
    result.w.z = -m.w.z;
    result.w.w = +m.w.w;

    return result;
}

GLfloat get_det_minor(mat4 a, mat4 m) {
    return a.x.x * m.x.x - a.x.y * m.x.y + a.x.z * m.x.z - a.x.w * m.x.w;
}

GLfloat mat4_det(mat4 m) {

    mat4 minor = mat_minor(m);
    GLfloat determinant = get_det_minor(m, minor);
    return determinant;
}

mat4 mat_inverse(mat4 m) {
    // Use cofactor to find inverse 
    mat4 minor = mat_minor(m);
    mat4 cofactor = mat_cofactor(minor);
    mat4 transpose = mat_transpose(cofactor);

    GLfloat determinant = get_det_minor(m, minor);
    mat4 minv = mat_mul_scalar(1/determinant, transpose);
    return minv;
}

