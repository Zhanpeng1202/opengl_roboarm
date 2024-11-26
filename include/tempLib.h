#ifndef _MYLIB_H_
#define _MYLIB_H_
#include <OpenGL/OpenGL.h>

// defined 
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

typedef struct 
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}vec3;


typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

// vec4 functions
void print_vec(vec4 v);
vec4 vec_mul_scalar(GLfloat s, vec4 v);
vec4 vec_add(vec4 v1, vec4 v2);
vec4 vec_sub(vec4 v1, vec4 v2);
GLfloat vec_norm2(vec4 v);
vec4 vec_standardlize(vec4 v);
GLfloat vec_dot(vec4 v1, vec4 v2);
vec4 vec_cross(vec4 v1, vec4 v2);

// mat4 functions
void print_mat(mat4 m);

mat4 mat_mul_scalar(GLfloat s, mat4 m);
mat4 mat_add(mat4 m1, mat4 m2);
mat4 mat_sub(mat4 m1, mat4 m2);
mat4 mat_mul_mat(mat4 m1, mat4 m2);
mat4 mat_transpose(mat4 m);
vec4 mat_mul_vec(mat4 m, vec4 v);
mat4 mat_inverse(mat4 v);

mat4 mat_minor(mat4 m);
mat4 mat_cofactor(mat4 m);
GLfloat mat4_det(mat4 m);
GLfloat get_det_minor(mat4 a, mat4 m);
vec4 transformation(mat4 R, mat4 S, vec4 T, vec4 V);
mat4 create_scale_mat(float x, float y, float z);
mat4 create_identity_mat();
mat4 get_transfom_mat(float scale, vec4 translation, mat4 orignal_mat);
mat4 create_rotate_mat(float yaw, float pitch, float roll);



#endif
