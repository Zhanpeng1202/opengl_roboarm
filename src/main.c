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
#include "view.h"

mat4 base_ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4* joint_ctm;
vec4* joint_positions;
vec4* joint_axis;
int*  number_of_points;

GLuint ctm_type;
GLuint base_location;
GLuint joint_low_location;
GLuint joint_middle_location;
GLuint joint_upper_location;
GLuint wrist_location;
GLuint finger1_location;
GLuint finger2_location;

vec4 eye =    (vec4) {0,0,1,1};
vec4 center = (vec4) {0,0,0,1};
vec4 up = (vec4) {0,1,0,0};
mat4 view_mat= {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
mat4 proj_mat = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint modelview_location;
GLuint projection_location;

vec4 light_position = (vec4) {0,5,0,1};
GLuint light_position_location, fake_shadow_location;

int num_base;

void init(void)
{
    GLuint program = initShader("asset/shader/vshader.glsl", "asset/shader/fshader.glsl");
    glUseProgram(program);

    int num_vertices = 6*8000;

    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);
    vec4 *normals = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    // initialize position, color, normal
    for(int i = 0; i < num_vertices; i++){
        positions[i] = (vec4) {0,0,0,0};
        colors[i] = (vec4) {0,0,0,0};
        normals[i] = (vec4) {0,0,0,0};
    }

    joint_positions = (vec4 *) malloc(sizeof(vec4) * 7);
    joint_axis = (vec4 *) malloc(sizeof(vec4) * 7);
    number_of_points = (int *) malloc(sizeof(int) * 7);
    joint_ctm = (mat4 *) malloc(sizeof(mat4) * 7);
    for(int i = 0; i < 7; i++){
        joint_ctm[i] = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        joint_axis[i] = (vec4) {0,0,1,0};
        joint_positions[i] = (vec4) {0,0,0,1};
    }

    joint_positions[0] = (vec4) {-0.0, 0, 0.0, 1};
    joint_axis[0] = (vec4) {0,1,0,0};


    num_base =  generate_cylinder(positions, 1, 0.01, 0.0, -0.0, 0.0);
    number_of_points[0] = generate_cylinder(positions, 0.1, 0.2, 0.1, -0.0, 0.0);

    for(int i = 1; i < 4; i++){
        joint_positions[i] = (vec4) {-0.0,0.2*(i),0.0,1};
        joint_axis[i] = (vec4) {0,0,1,0};
        generate_horizontal_cylinder(positions,0.07, 0.07, 0.2*i, -0.0, 0.0);
        number_of_points[i] = generate_cylinder(positions,0.05, 0.2,0.2*i+0.1,-0.0, 0.0);
    }

    joint_positions[4] = (vec4) {-0.0,0.8,0.0,1};
    joint_axis[4] = (vec4) {0,1,0,0};
    number_of_points[4] = generate_horizontal_cylinder(positions,0.07, 0.07, 0.8, -0.0, 0.0);
    number_of_points[5] = generate_cylinder(positions,0.01, 0.05,0.8+0.07,0.0,  0.01);
    number_of_points[6] = generate_cylinder(positions,0.01, 0.05,0.8+0.07,0.0, -0.01);
    joint_axis[5] = (vec4) {0,0,1,0};
    // joint_positions[5] = (vec4) {0.0, 0.8+0.07,0.01,1};
    // joint_positions[6] = (vec4) {0.0, 0.8+0.07,-0.01,1};


    for(int i = 0; i < number_of_points[6]; i+=3){
        vec4 l1 = vec_sub(positions[i+1], positions[i]);
        vec4 l2 = vec_sub(positions[i+2], positions[i]);
        normals[i] =  vec_standardlize(vec_cross(l2, l1));
        normals[i+1] = normals[i];
        normals[i+2] = normals[i];
    }
    random_color(colors);

    // now we add fake shadow to robot arm we need to copy 
    // from base to finger 

    int offset = number_of_points[6] - num_base;
    for(int i = num_base; i<number_of_points[6]; i++){
        positions[i+offset] = positions[i];
        colors[i+offset] = (vec4) {0,0,0,1};
        normals[i+offset] = normals[i];
    }

    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices*3, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices*2, sizeof(vec4) * num_vertices, normals);
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (0));
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices*2));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(0,1);
    base_location = glGetUniformLocation(program, "ctm_base");
    joint_low_location = glGetUniformLocation(program, "ctm_joint_low");
    joint_middle_location = glGetUniformLocation(program, "ctm_joint_middle");
    joint_upper_location = glGetUniformLocation(program, "ctm_joint_upper");
    wrist_location = glGetUniformLocation(program, "ctm_wrist");
    finger1_location = glGetUniformLocation(program, "ctm_finger1");
    finger2_location = glGetUniformLocation(program, "ctm_finger2");
    ctm_type = glGetUniformLocation(program, "ctm_type");
    modelview_location = glGetUniformLocation(program, "view_mat");
    projection_location = glGetUniformLocation(program, "projection_mat");
    light_position_location = glGetUniformLocation(program, "light_position");
    glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    proj_mat = proj_matrix();
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &proj_mat);
    fake_shadow_location = glGetUniformLocation(program, "fake_shadow");
}



void display(void)
{
    // glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_BACK, GL_LINE);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    view_mat = look_at(eye, center, up);
    glUniformMatrix4fv(modelview_location, 1, GL_FALSE, (GLfloat *) &view_mat);

    glUniform1i(fake_shadow_location, 0);
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


    glUniform1i(ctm_type, 6); 
    glUniformMatrix4fv(finger1_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[4]);
    glDrawArrays(GL_TRIANGLES, number_of_points[4], number_of_points[5]-number_of_points[4]); 

    glUniform1i(ctm_type, 7); 
    glUniformMatrix4fv(finger2_location, 1, GL_FALSE, (GLfloat *) &joint_ctm[5]);
    glDrawArrays(GL_TRIANGLES, number_of_points[5], number_of_points[6]-number_of_points[5]); 

    glUniform1i(fake_shadow_location, 1);
    // the offset is for each part for fake shaow is like this 
    // base cylinder start: number_of_points[6], length1: number_of_points[0]-num_base
    // lower joint   start: number_of_points[6] + length1, length2: number_of_points[1]-number_of_points[0]
    // middle joint  start: number_of_points[6] + length1 + length2, length3: number_of_points[2]-number_of_points[1]
    // upper joint   start: number_of_points[6] + length1 + length2 + length3, length4: number_of_points[3]-number_of_points[2]
    // wrist         start: number_of_points[6] + length1 + length2 + length3 + length4, length5: number_of_points[4]-number_of_points[3]
    // finger1       start: number_of_points[6] + length1 + length2 + length3 + length4 + length5, length6: number_of_points[5]-number_of_points[4]
    // finger2       start: number_of_points[6] + length1 + length2 + length3 + length4 + length5 + length6, length7: number_of_points[6]-number_of_points[5]
    
    glUniform1i(ctm_type, 1); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6], number_of_points[0]-num_base);
    glUniform1i(ctm_type, 2); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base, number_of_points[1]-number_of_points[0]);
    glUniform1i(ctm_type, 3); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base + number_of_points[1]-number_of_points[0], number_of_points[2]-number_of_points[1]);
    glUniform1i(ctm_type, 4); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base + number_of_points[1]-number_of_points[0] + number_of_points[2]-number_of_points[1], number_of_points[3]-number_of_points[2]);
    glUniform1i(ctm_type, 5); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base + number_of_points[1]-number_of_points[0] + number_of_points[2]-number_of_points[1] + number_of_points[3]-number_of_points[2], number_of_points[4]-number_of_points[3]);
    glUniform1i(ctm_type, 6); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base + number_of_points[1]-number_of_points[0] + number_of_points[2]-number_of_points[1] + number_of_points[3]-number_of_points[2] + number_of_points[4]-number_of_points[3], number_of_points[5]-number_of_points[4]);
    glUniform1i(ctm_type, 7); 
    glDrawArrays(GL_TRIANGLES, number_of_points[6] + number_of_points[0]-num_base + number_of_points[1]-number_of_points[0] + number_of_points[2]-number_of_points[1] + number_of_points[3]-number_of_points[2] + number_of_points[4]-number_of_points[3] + number_of_points[5]-number_of_points[4], number_of_points[6]-number_of_points[5]);
    glutSwapBuffers();
}



void mouse(int button, int state, int x, int y) {
}


void motion(int x, int y) {
    // glutPostRedisplay();
}


float theta = 0;
float phi = 0;
float radius = 1;

void keyboard(unsigned char key, int mousex, int mousey)
{


    if(key == '1'){
        theta += 0.1;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }

    if(key == '2'){
        theta -= 0.1;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }
    if(key == '3'){
        phi += 0.1;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }

    if(key == '4'){
        phi -= 0.1;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }

    if(key == '5'){
        light_position.x += 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }   

    if(key == '6'){
        light_position.x -= 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }
    if(key == '7'){
        light_position.y += 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }

    if(key == '8'){
        light_position.y -= 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }

    if(key == '9'){
        light_position.z += 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }
    if(key == '0'){
        light_position.z -= 0.1;
        glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);
    }

    if(key == 'a'){
        radius -= 0.01;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }
    if(key == 'z'){
        radius += 0.01;
        eye.x = radius*sin(theta)*cos(phi);
        eye.y = radius*sin(phi);
        eye.z = radius*cos(theta)*cos(phi);
        view_mat = look_at(eye, center, up);
    }

    if(key == 'w'){
        vec4 axis = joint_axis[0];
        vec4 point = joint_positions[0];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        base_ctm = mat_mul_mat(rotate,base_ctm); 
        for(int i = 0; i < 7; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 0; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 's'){
        vec4 axis = joint_axis[0];
        vec4 point = joint_positions[0];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        base_ctm = mat_mul_mat(rotate,base_ctm); 
        for(int i = 0; i < 7; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 0; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }
    if(key == 'e'){
        vec4 axis = joint_axis[1];
        vec4 point = joint_positions[1];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 0; i < 7; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 1; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 'd'){
        vec4 axis = joint_axis[1];
        vec4 point = joint_positions[1];
        // print_vec(point);
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 0; i < 7; i++){
            if(i == 0){
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            } else {
                joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
            }
        }
        for(int i = 1; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 'r'){
        vec4 axis = joint_axis[2];
        vec4 point = joint_positions[2];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 1; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 2; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 'f'){
        vec4 axis = joint_axis[2];
        vec4 point = joint_positions[2];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 1; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 2; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 't'){
        vec4 axis = joint_axis[3];
        vec4 point = joint_positions[3];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 2; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 3; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }
    
    if(key == 'g'){
        vec4 axis = joint_axis[3];
        vec4 point = joint_positions[3];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 2; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 3; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }

    if(key == 'y'){

        vec4 axis = joint_axis[4];
        vec4 point = joint_positions[4];
        mat4 rotate = rotate_at_point_along_axis(axis, point, 0.1);
        for(int i = 3; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 4; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }
    if(key == 'h'){

        vec4 axis = joint_axis[4];
        vec4 point = joint_positions[4];
        mat4 rotate = rotate_at_point_along_axis(axis, point, -0.1);
        for(int i = 3; i < 7; i++){
            joint_ctm[i] = mat_mul_mat(rotate,joint_ctm[i]);
        }
        for(int i = 4; i < 7; i++){
            joint_positions[i] = mat_mul_vec(rotate,joint_positions[i]);
            joint_axis[i] = mat_mul_vec(rotate,joint_axis[i]);
        }
    }
    if(key == 'u'){
        // open the finger
        vec4 translation = joint_axis[5];
        translation = vec_standardlize(translation);
        translation = vec_mul_scalar(0.01,translation);
        mat4 translate = translate_mat(translation);

        joint_ctm[4] = mat_mul_mat(translate,joint_ctm[4]);
        translate.w.x = -translate.w.x;
        translate.w.y = -translate.w.y;
        translate.w.z = -translate.w.z;

        joint_ctm[5] = mat_mul_mat(translate,joint_ctm[5]);

    }
    if(key == 'j'){
        // close the finger
        vec4 translation =joint_axis[5];
        translation = vec_standardlize(translation);
        translation = vec_mul_scalar(0.01,translation);
        mat4 translate = translate_mat(translation);

        translate.w.x = -translate.w.x;
        translate.w.y = -translate.w.y;
        translate.w.z = -translate.w.z;

        joint_ctm[4] = mat_mul_mat(translate,joint_ctm[4]);

        translate.w.x = -translate.w.x;
        translate.w.y = -translate.w.y;
        translate.w.z = -translate.w.z;

        joint_ctm[5] = mat_mul_mat(translate,joint_ctm[5]);

    }



    glutPostRedisplay();

}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(300,300);
    glutCreateWindow("Computer Generated Object");
    init();
    // display();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
