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


mat4 base_ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4* joint_ctm;
vec4* joint_positions;
int*  number_of_points;

GLuint ctm_type;

GLuint base_location;
GLuint joint_low_location;
GLuint joint_middle_location;
GLuint joint_upper_location;
GLuint wrist_location;

void init(void)
{
    GLuint program = initShader("asset/shader/vshader.glsl", "asset/shader/fshader.glsl");
    glUseProgram(program);

    int num_vertices = 6*10000;

    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    joint_positions = (vec4 *) malloc(sizeof(vec4) * 4);
    number_of_points = (int *) malloc(sizeof(int) * 4);
    joint_ctm = (mat4 *) malloc(sizeof(mat4) * 4);
    joint_ctm[0] = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    joint_ctm[1] = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    joint_ctm[2] = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    joint_ctm[3] = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    joint_positions[0] = (vec4) {-0.3, 0, 0.3, 1};


    // base
    generate_cylinder(positions, 0.2, 0.01, 0.0, -0.3, 0.3);
    number_of_points[0] = generate_cylinder(positions, 0.1, 0.2, 0.1, -0.3, 0.3);

    for(int i = 0; i < 3; i++){
        int j = i+1;
        joint_positions[i] = (vec4) {-0.3,0.2*(j),0.3,1};
        generate_horizontal_cylinder(positions,0.07, 0.07, 0.2*j, -0.3, 0.3);
        number_of_points[j] = generate_cylinder(positions,0.05, 0.2,0.2*j+0.1,-0.3, 0.3);
    }
    joint_positions[3] = (vec4) {-0.3,0.8,0.3,1};
    generate_horizontal_cylinder(positions,0.07, 0.07, 0.2*4, -0.3, 0.3);
    number_of_points[4] = generate_cylinder(positions,0.05, 0.2,0.2*4+0.1,-0.3, 0.3);

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
    base_location = glGetUniformLocation(program, "ctm_base");
    joint_low_location = glGetUniformLocation(program, "ctm_joint_low");
    joint_middle_location = glGetUniformLocation(program, "ctm_joint_middle");
    joint_upper_location = glGetUniformLocation(program, "ctm_joint_upper");
    wrist_location = glGetUniformLocation(program, "ctm_wrist");
    ctm_type = glGetUniformLocation(program, "ctm_type");
}



void display(void)
{
    // glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniform1i(ctm_type, 1); 
    glUniformMatrix4fv(base_location, 1, GL_FALSE, (GLfloat *) &base_ctm);
    glDrawArrays(GL_TRIANGLES, 0, number_of_points[0]);

    glUniform1i(ctm_type, 2); 
    glUniformMatrix4fv(joint_low_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[0]);
    glDrawArrays(GL_TRIANGLES, number_of_points[0], number_of_points[1]-number_of_points[0]);

    glUniform1i(ctm_type, 3); 
    glUniformMatrix4fv(joint_middle_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[1]);
    glDrawArrays(GL_TRIANGLES, number_of_points[1], number_of_points[2]-number_of_points[1]);   

    glUniform1i(ctm_type, 4); 
    glUniformMatrix4fv(joint_upper_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[2]);
    glDrawArrays(GL_TRIANGLES, number_of_points[2], number_of_points[3]-number_of_points[2]);   

    glUniform1i(ctm_type, 5); 
    glUniformMatrix4fv(wrist_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[3]);
    glDrawArrays(GL_TRIANGLES, number_of_points[3], number_of_points[4]-number_of_points[3]); 
    print_mat(joint_ctm[3]);


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
    display();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'a'){
        mat4 scale_mat = create_scale_mat(1.1,1.1,1.1);
        base_ctm = mat_mul_mat(scale_mat,base_ctm);
        for(int i = 0; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(scale_mat,joint_ctm[i]);
        }
        for(int i = 0; i < 4; i++){
            joint_positions[i] = mat_mul_vec(scale_mat,joint_positions[i]);
        }
    }
    if(key == 'z'){
        mat4 scale_mat = create_scale_mat(0.9,0.9,0.9);
        base_ctm = mat_mul_mat(scale_mat,base_ctm);
        for(int i = 0; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(scale_mat,joint_ctm[i]);
        }
        for(int i = 0; i < 4; i++){
            joint_positions[i] = mat_mul_vec(scale_mat,joint_positions[i]);
        }
    }

    if(key == 'w'){
        vec4 axis = (vec4) {0,1,0,0};
        vec4 point = joint_positions[0];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        base_ctm = mat_mul_mat(rotate,base_ctm); 
        for(int i = 0; i < 4; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 0; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 's'){
        vec4 axis = (vec4) {0,1,0,0};
        vec4 point = joint_positions[0];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        base_ctm = mat_mul_mat(rotate,base_ctm); 
        for(int i = 0; i < 4; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 0; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }
    if(key == 'e'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[0];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 0; i < 4; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 1; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 'd'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[0];
        // print_vec(point);
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 0; i < 4; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 1; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 'r'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[1];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 1; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 2; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 'f'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[1];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 1; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 2; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 't'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[2];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 2; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 3; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }
    
    if(key == 'g'){
        vec4 axis = (vec4) {0,0,-1,0};
        vec4 point = joint_positions[2];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 2; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 3; i < 4; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
        }
    }

    if(key == 'y'){
        vec4 axis = (vec4) {joint_positions[3].x-joint_positions[2].x,
                            joint_positions[3].y-joint_positions[2].y,
                            joint_positions[3].z-joint_positions[2].z,0};
        vec4 point = joint_positions[3];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 3; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
    }
    if(key == 'h'){
        vec4 axis = (vec4) {joint_positions[3].x-joint_positions[2].x,
                            joint_positions[3].y-joint_positions[2].y,
                            joint_positions[3].z-joint_positions[2].z,0};
        vec4 point = joint_positions[3];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 3; i < 4; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
    }

    display();

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
