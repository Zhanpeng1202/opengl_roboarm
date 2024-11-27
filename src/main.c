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
#include "rotate.h"

mat4 current_transformation_matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 base_rotation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 joint_low_rotation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;
GLuint ctm_type;

int joint_low_points = 0;
int joint_middle_points = 0;
GLuint base_location;
GLuint joint_low_location;

void init(void)
{
    GLuint program = initShader("asset/shader/vshader.glsl", "asset/shader/fshader.glsl");
    glUseProgram(program);

    int num_vertices = 6*10000;

    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    // base
    int start_point = 0;
    generate_cylinder(positions, 0.2, 0.1, 0.0, -0.3, 0.3);
    generate_cylinder(positions, 0.1, 0.3, 0.2, -0.3, 0.3);
    joint_low_points = generate_horizontal_cylinder(positions, 0.1, 0.2, 0.35, -0.3, 0.3);

    generate_cylinder(positions, 0.05, 0.2, 0.5, -0.3, 0.3);
    joint_middle_points = generate_horizontal_cylinder(positions, 0.05, 0.1, 0.6, -0.3, 0.3);

    // generate_cylinder(positions, 0.025, 0.15, 0.7, -0.3, 0.3);
    // generate_horizontal_cylinder(positions, 0.025, 0.07, 0.7+0.15/2);

    // generate_cylinder(positions, 0.025, 0.15, 0.9);
    // generate_horizontal_cylinder(positions, 0.025, 0.07, 0.9+0.15/2);

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
    base_location = glGetUniformLocation(program, "ctm_base");
    joint_low_location = glGetUniformLocation(program, "ctm_joint_low");
    ctm_type = glGetUniformLocation(program, "ctm_type");
}



void display(void)
{
    // glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniform1i(ctm_type, 0); 
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &current_transformation_matrix); 


    glDrawArrays(GL_TRIANGLES, 0, joint_low_points);
    glutSwapBuffers();
}



void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clicked  = 1;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        clicked  = 0;
        motion_start = 0;
    }


}


void motion(int x, int y) {
    track_ball(x,y,&current_transformation_matrix);
    display();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'a'){
        mat4 scale_mat = create_scale_mat(1.1,1.1,1.1);
        current_transformation_matrix = mat_mul_mat(current_transformation_matrix,scale_mat);
        display();
    }
    if(key == 'z'){
        mat4 scale_mat = create_scale_mat(0.9,0.9,0.9);
        current_transformation_matrix = mat_mul_mat(current_transformation_matrix,scale_mat);
        display();
    }

    // if(key == 'w'){
    //     base_rotation = rotate
    // }


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
