/*
 * template.c
 *
 *  Created on: September 10, 2024
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <time.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include <stdlib.h>
#include<stdio.h>
#include <math.h>

#include "tempLib.h" 
#include "generate.h"
#include "trackball.h"

mat4 current_transformation_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;
int display_type = 0;
int read_vertices = 0;




void init(void)
{
    GLuint program = initShader("asset/shader/vshader.glsl", "asset/shader/fshader.glsl");
    glUseProgram(program);

    int num_vertices = 6*10000;

    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);


    // generateCylinderMesh(positions);
    generateSphereMesh(positions);
    random_color(colors);


    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices + sizeof(vec4) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    ctm_location = glGetUniformLocation(program, "ctm");

}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &current_transformation_matrix);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices_sphere);
    glutSwapBuffers();
}



void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clicked  = 1;
        motion_start = 1;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        printf("LEFT DOWN \n");
        clicked  = 0;
        motion_start = 0;
        printf("-----clicked: %d motion%d\n", clicked,motion_start);
    }


}


void motion(int x, int y) {
    track_ball(x,y,&current_transformation_matrix);
    display();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == '1'){
        display_type = 1;
        display();
    }
    if(key == '2'){
        display_type = 2;
        display();
    }
    if(key == '3'){
        display_type = 3;
        display();
    }


    if(key == 'u'){
        mat4 scale_mat = create_scale_mat(1.1,1.1,1.1);
        current_transformation_matrix = mat_mul_mat(current_transformation_matrix,scale_mat);
        display();
    }

    if(key == 'i'){
        mat4 scale_mat = create_scale_mat(0.9,0.9,0.9);
        current_transformation_matrix = mat_mul_mat(current_transformation_matrix,scale_mat);
        display();
    }
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(300,300);
    glutCreateWindow("Computer Generated Object");
    init();
    display();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
