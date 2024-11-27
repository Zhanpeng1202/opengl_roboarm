#include "tempLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// ---------------------------------------
float FLT_MAX = 100000;
float FLT_MIN = -100000;


vec4 map_to_sphere(float x, float y) {
    float z;
    float length = x * x + y * y;

    if (length <= 1.0f) {
        z = sqrt(1.0f - length);  // on the sphere
    } else {
        z = 0.0f;  
        float norm = sqrt(length);
        x /= norm;
        y /= norm;
    }
    z = sqrt(1.0 - x * x - y * y);  // on the sphere
    vec4 result = {x, y, z, 0.0};
    return result;
}

vec4 vec4_cross(vec4 a, vec4 b) {
    vec4 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    result.w = 0.0f;  

    return result;
}

float vec4_dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

mat4 Rx(float az, float ay, float d){
    vec4 x = {1.0,0.0,0.0,0.0};
    vec4 y = {0.0,az/d,ay/d,0};
    vec4 z = {0,-ay/d,az/d,0};
    vec4 w = {0,0,0,1};
    mat4 result = {x,y,z,w};
    return result;
}

mat4 RyT(float ax, float d){
    vec4 x = {d,0,ax,0};
    vec4 y = {0,1,0,0};
    vec4 z = {-ax,0,d,0};
    vec4 w = {0,0,0,1};
    mat4 result = {x,y,z,w};
    return result;
}


float stored_x = 0;
float stored_y = 0;
int clicked = 0;
int motion_start = 0;

void track_ball(int x, int y, mat4 *ctm){
    float floatx = (float)2*(float)x/1023 -1;
    float floaty = (float)-2*(float)y/1023 +1 ;

    
    if(motion_start ==0){
        stored_x = floatx;
        stored_y = floaty;
        motion_start = 1;
        return;
    }
    if(clicked == 1) {
        if(floatx*floatx+floaty*floaty >=1){
            return;
        }

        float norm_x = floatx;
        float norm_y = floaty;


        vec4 current_pos = map_to_sphere(norm_x, norm_y);
        vec4 previous_pos = map_to_sphere(stored_x, stored_y);

        vec4 rotation_axis = vec4_cross(previous_pos, current_pos);
        rotation_axis = vec_standardlize(rotation_axis);
        float dot = vec4_dot(previous_pos, current_pos);
        float rotation_angle = acos(fmin(fmax(dot, -1.0f), 1.0f));  
        

        float d = sqrt(rotation_axis.y*rotation_axis.y + rotation_axis.z*rotation_axis.z);

        mat4 rotate_x = Rx(rotation_axis.z,rotation_axis.y,d);
        mat4 rotate_y_t = RyT(rotation_axis.x,d);
        mat4 rotate_z = create_rotate_mat(0,0,rotation_angle);

        *ctm = mat_mul_mat(rotate_x, *ctm);
        *ctm = mat_mul_mat(rotate_y_t, *ctm);
        *ctm = mat_mul_mat(rotate_z, *ctm);
        *ctm = mat_mul_mat(mat_transpose(rotate_y_t), *ctm);
        *ctm = mat_mul_mat(mat_transpose(rotate_x), *ctm);
        stored_x = floatx;
        stored_y = floaty;
    }
}


mat4 translate_mat(vec4 translation){
    mat4 result = create_identity_mat();
    result.w = translation;
    result.w.w = 1.0;
    return result;
}

void rotate_at_point_along_axis(mat4 *ctm, vec4 axis, vec4 point){

    vec4 rotation_axis = vec_standardlize(axis);
    float rotation_angle = 15;  
    
    float d = sqrt(rotation_axis.y*rotation_axis.y + rotation_axis.z*rotation_axis.z);

    mat4 rotate_x = Rx(rotation_axis.z,rotation_axis.y,d);
    mat4 rotate_y_t = RyT(rotation_axis.x,d);
    mat4 rotate_z = create_rotate_mat(0,0,rotation_angle);

    mat4 translate = translate_mat(point);
    vec4 translate_back = point;
    translate_back.x = -point.x;
    translate_back.y = -point.y;
    translate_back.z = -point.z;
    mat4 translate_back_mat = translate_mat(translate_back);


    *ctm = mat_mul_mat(translate_back_mat, *ctm);
    *ctm = mat_mul_mat(rotate_x, *ctm);
    *ctm = mat_mul_mat(rotate_y_t, *ctm);
    *ctm = mat_mul_mat(rotate_z, *ctm);
    *ctm = mat_mul_mat(mat_transpose(rotate_y_t), *ctm);
    *ctm = mat_mul_mat(mat_transpose(rotate_x), *ctm);
    *ctm = mat_mul_mat(translate, *ctm);
}


