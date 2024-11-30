#include "tempLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// mat4 look_at(vec4 eye, vec4 center, vec4 up)
// {
//     vec4 f = vec_standardlize(vec_sub(center, eye));
//     vec4 s = vec_standardlize(vec_cross(f, up));
//     vec4 u = vec_cross(s, f);
//     mat4 look_at_mat = {
//         { s.x, s.y, s.z,    -vec_dot(s, eye) },
//         { u.x, u.y, u.z,   -vec_dot(u, eye) },
//         { -f.x, -f.y, -f.z, vec_dot(f, eye) },
//         { 0, 0, 0, 1 }
//     };
    
//     print_mat(look_at_mat);
//     return look_at_mat;
// }

mat4 look_at(vec4 eye, vec4 at, vec4 up){
    vec4  f = vec_standardlize(vec_sub(at, eye));
    vec4  u = vec_standardlize(up);
    vec4  s = vec_standardlize(vec_cross(f, u));
    u = vec_cross(s, f);
    mat4 result = {
        {s.x, u.x, -f.x, 0.0},
        {s.y, u.y, -f.y, 0.0},
        {s.z, u.z, -f.z, 0.0},
        {-vec_dot(s, eye), -vec_dot(u, eye), vec_dot(f, eye), 1.0}
    };

    return result;
}


mat4 proj_matrix() {
    float left = -0.2;
    float right = 0.2;
    float bottom = -0.2;
    float top = 0.2;
    float near = 0.1;
    float far = 50.0;
    mat4 M = {
        {2*near/(right-left), 0.0, 0.0, 0.0},
        {0.0, 2*near/(top-bottom), 0.0, 0.0},
        {(right+left)/(right-left), (top+bottom)/(top-bottom), -(far+near)/(far-near), -1.0},
        {0.0, 0.0, -2*(far*near)/(far-near), 0.0}
    };

    return M;
}

