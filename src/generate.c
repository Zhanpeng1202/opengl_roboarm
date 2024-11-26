#include "tempLib.h" 
#include <stdlib.h>
#include<stdio.h>
#include <math.h>


#define PI 3.14159265359
#define NUM_LATITUDES 20
#define NUM_LONGITUDES 20
int num_vertices_sphere = 6*NUM_LATITUDES*NUM_LONGITUDES;
int num_points = 0;



void generateSphereMesh(vec4 *vertices) {
    int index = 0;
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

            vertices[index++] = p1;
            vertices[index++] = p2;
            vertices[index++] = p3;

            vertices[index++] = p3;
            vertices[index++] = p2;
            vertices[index++] = p4;
            num_points +=6;
        }
    }
}

void generateCylinderMesh(vec4 *vertices) {
    const float height = 2.0f; // Height of the cylinder
    const float radius = 1.0f; // Radius of the cylinder
    int index = 0;

    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        // Bottom vertice
        vec4 p1 = {radius * cos(phi1), -height / 2.0f, radius * sin(phi1), 1.0f};
        vec4 p2 = {radius * cos(phi2), -height / 2.0f, radius * sin(phi2), 1.0f};

        // Top vertices
        vec4 p3 = {radius * cos(phi1), height / 2.0f, radius * sin(phi1), 1.0f};
        vec4 p4 = {radius * cos(phi2), height / 2.0f, radius * sin(phi2), 1.0f};

        // First triangle for the side face
        vertices[index++] = p1;
        vertices[index++] = p2;
        vertices[index++] = p3;

        // Second triangle for the side face
        vertices[index++] = p3;
        vertices[index++] = p2;
        vertices[index++] = p4;

        num_points += 6;
    }

    // Generate the top and bottom caps
    vec4 center_top = {0.0f, height / 2.0f, 0.0f, 1.0f};   // Center of the top cap
    vec4 center_bottom = {0.0f, -height / 2.0f, 0.0f, 1.0f}; // Center of the bottom cap

    for (int j = 0; j < NUM_LONGITUDES; ++j) {
        float phi1 = 2 * PI * (float)(j) / NUM_LONGITUDES;
        float phi2 = 2 * PI * (float)(j + 1) / NUM_LONGITUDES;

        // Points on the bottom circle
        vec4 p1 = {radius * cos(phi1), -height / 2.0f, radius * sin(phi1), 1.0f};
        vec4 p2 = {radius * cos(phi2), -height / 2.0f, radius * sin(phi2), 1.0f};

        // Points on the top circle
        vec4 p3 = {radius * cos(phi1), height / 2.0f, radius * sin(phi1), 1.0f};
        vec4 p4 = {radius * cos(phi2), height / 2.0f, radius * sin(phi2), 1.0f};

        // Bottom cap triangle
        vertices[index++] = center_bottom;
        vertices[index++] = p2;
        vertices[index++] = p1;

        // Top cap triangle
        vertices[index++] = center_top;
        vertices[index++] = p3;
        vertices[index++] = p4;

        num_points += 6;
    }
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