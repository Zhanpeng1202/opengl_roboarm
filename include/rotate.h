#include "stdio.h"
#include "stdlib.h"
#include "tempLib.h"

extern float stored_x ;
extern float stored_y ;
extern int clicked ;
extern int motion_start ;

void track_ball(int x, int y, mat4 *ctm);