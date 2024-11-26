#include "tempLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// ---------------------------------------
float FLT_MAX = 100000;
float FLT_MIN = -100000;
void normalizeToUnitCube(vec4* vertices, int numVertices) {
    GLfloat minX = FLT_MAX, maxX = FLT_MIN;
    GLfloat minY = FLT_MAX, maxY = FLT_MIN;
    GLfloat minZ = FLT_MAX, maxZ = FLT_MIN;

    // Step 1: Find the bounding box (min and max values for x, y, z)
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
        if (vertices[i].z < minZ) minZ = vertices[i].z;
        if (vertices[i].z > maxZ) maxZ = vertices[i].z;
    }

    // Step 2: Compute the center of the bounding box
    GLfloat centerX = (minX + maxX) / 2.0f;
    GLfloat centerY = (minY + maxY) / 2.0f;
    GLfloat centerZ = (minZ + maxZ) / 2.0f;

    // Step 3: Compute the maximum extent (the largest distance from the center)
    GLfloat extentX = (maxX - minX) / 2.0f;
    GLfloat extentY = (maxY - minY) / 2.0f;
    GLfloat extentZ = (maxZ - minZ) / 2.0f;
    GLfloat maxExtent = (extentX > extentY) ? extentX : extentY;
    maxExtent = (maxExtent > extentZ) ? maxExtent : extentZ;

    // Step 4: Translate and scale the points into the [-1, 1] range
    for (int i = 0; i < numVertices; i++) {
        vertices[i].x = (vertices[i].x - centerX) / extentX;
        vertices[i].y = (vertices[i].y - centerY) / extentY;
        vertices[i].z = (vertices[i].z - centerZ) / extentZ;

    }

}


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
    result.w = 0.0f;  // Ensure w remains 1

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