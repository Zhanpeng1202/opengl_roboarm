#include "tempLib.h" 

extern int num_vertices_sphere;
extern int num_points;
int generate_cylinder(vec4 *vertices, float radius, float height, float y, float x_offset, float z_offset);
int generate_horizontal_cylinder(vec4 *vertices, float radius, float height, float y, float x_offset, float z_offset);

// For test 
void generateSphereMesh(vec4 *vertices);
void random_color(vec4 * colors);