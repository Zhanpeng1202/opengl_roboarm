#include "tempLib.h" 
#include <stdlib.h>
#include<stdio.h>
#include <math.h>


#define PI 3.14159265359
#define NUM_LATITUDES 20
#define NUM_LONGITUDES 20
int num_points = 0;


void generateSphereMesh(vec4 *vertices) {
    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        for (int i = 0; i < NUM_LATITUDES; ++i) {
            float theta1 = PI * (-0.5 + (float)(i) / NUM_LATITUDES);
            float theta2 = PI * (-0.5 + (float)(i + 1) / NUM_LATITUDES);

            vec4 p1 = {cos(theta1) * cos(phi1), sin(theta1), cos(theta1) * sin(phi1), 1.0f};
            vec4 p2 = {cos(theta2) * cos(phi1), sin(theta2), cos(theta2) * sin(phi1), 1.0f};
            vec4 p3 = {cos(theta1) * cos(phi2), sin(theta1), cos(theta1) * sin(phi2), 1.0f};
            vec4 p4 = {cos(theta2) * cos(phi2), sin(theta2), cos(theta2) * sin(phi2), 1.0f};

            vertices[0] = p1;
            vertices[1] = p2;
            vertices[2] = p3;

            vertices[3] = p3;
            vertices[4] = p2;
            vertices[5] = p4;
            vertices+=6;
            num_points +=6;
        }
    }
}

int generate_cylinder(vec4 *vertices, float radius, float height, float y, float x_offset, float z_offset){
    vertices+=num_points;


    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        vec4 p1 = {radius * cos(phi1)+x_offset, -height / 2.0f+y, radius * sin(phi1)+z_offset, 1.0f};
        vec4 p2 = {radius * cos(phi2)+x_offset, -height / 2.0f+y, radius * sin(phi2)+z_offset, 1.0f};
        vec4 p3 = {radius * cos(phi1)+x_offset, height / 2.0f+y, radius * sin(phi1)+z_offset, 1.0f};
        vec4 p4 = {radius * cos(phi2)+x_offset, height / 2.0f+y, radius * sin(phi2)+z_offset, 1.0f};

        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;

        vertices[3] = p3;
        vertices[4] = p2;
        vertices[5] = p4;

        num_points += 6;
        vertices+=6;
    }

    vec4 center_top = {0.0f+x_offset, height / 2.0f + y, 0.0f+z_offset, 1.0f};   // Center of the top cap
    vec4 center_bottom = {0.0f+x_offset, -height / 2.0f + y, 0.0f+z_offset , 1.0f}; // Center of the bottom cap

    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        vec4 p1 = {radius * cos(phi1)+x_offset, -height / 2.0f + y, radius * sin(phi1)+z_offset, 1.0f};
        vec4 p2 = {radius * cos(phi2)+x_offset, -height / 2.0f + y, radius * sin(phi2)+z_offset, 1.0f};

        vec4 p3 = {radius * cos(phi1)+x_offset, height / 2.0f + y, radius * sin(phi1)+z_offset, 1.0f};
        vec4 p4 = {radius * cos(phi2)+x_offset, height / 2.0f + y, radius * sin(phi2)+z_offset, 1.0f};

        vertices[0] = center_bottom;
        vertices[1] = p2;
        vertices[2] = p1;

        vertices[3] = center_top;
        vertices[4] = p3;
        vertices[5] = p4;

        num_points += 6;
        vertices+=6;
    }
    return num_points;
}

int generate_horizontal_cylinder(vec4 *vertices, float radius, float height, float y, float x_offset, float z_offset){
    vertices+=num_points;

    vec4 center_near= {0.0f+x_offset,  y, -height/2.0f+z_offset, 1.0f};   // Center of the near cap
    vec4 center_far = {0.0f+x_offset,  y, height/2.0f+z_offset, 1.0f}; // Center of the far cap

    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        vec4 p1 = {radius * cos(phi1)+x_offset, radius * sin(phi1)+y, -height/2.0f+z_offset, 1.0f};
        vec4 p2 = {radius * cos(phi2)+x_offset, radius * sin(phi2)+y, -height/2.0f+z_offset, 1.0f};

        vec4 p3 = {radius * cos(phi1)+x_offset, radius * sin(phi1)+y, height/2.0f+z_offset, 1.0f};
        vec4 p4 = {radius * cos(phi2)+x_offset, radius * sin(phi2)+y, height/2.0f+z_offset, 1.0f};

        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;

        vertices[3] = p3;
        vertices[4] = p2;
        vertices[5] = p4;

        num_points += 6;
        vertices+=6;
    }

    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        vec4 p1 = {radius * cos(phi1)+x_offset, radius * sin(phi1)+y, -height/2.0f+z_offset, 1.0f};
        vec4 p2 = {radius * cos(phi2)+x_offset, radius * sin(phi2)+y, -height/2.0f+z_offset, 1.0f};

        vec4 p3 = {radius * cos(phi1)+x_offset, radius * sin(phi1)+y, height/2.0f+z_offset, 1.0f};
        vec4 p4 = {radius * cos(phi2)+x_offset, radius * sin(phi2)+y, height/2.0f+z_offset, 1.0f};

        vertices[0] = center_near;
        vertices[1] = p2;
        vertices[2] = p1;

        vertices[3] = center_far;
        vertices[4] = p3;
        vertices[5] = p4;

        num_points += 6;
        vertices+=6;
    }
    return num_points;
}


void random_color(vec4 * colors){
    for (int i=0; i<num_points; i+=3){
        float randomFloat = (float)rand() / (float)RAND_MAX;
        colors[i] = (vec4) {(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX, 1.0};
        colors[i+1] =  colors[i];
        colors[i+2] =  colors[i];
    }
}


// void random_color(vec4 * colors){
//     vec4 same_color = (vec4) {(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX, 1.0};

//     for (int i=0; i<num_points; i+=3){
//         colors[i] = same_color;
//         colors[i+1] =  colors[i];
//         colors[i+2] =  colors[i];
//     }
// }