#include "stdio.h"
#include "stdlib.h"
#include "tempLib.h"

extern float stored_x ;
extern float stored_y ;
extern int clicked ;
extern int motion_start ;

void track_ball(int x, int y, mat4 *ctm);
mat4 rotate_at_point_along_axis(vec4 axis, vec4 point, float rotation_angle);  
mat4 translate_mat(vec4 translation);
