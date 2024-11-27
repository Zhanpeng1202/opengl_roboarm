#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;



uniform mat4 ctm_base;
uniform mat4 ctm_joint_low;
uniform mat4 ctm_joint_middle;
uniform mat4 ctm_joint_upper;
uniform mat4 ctm_wrist;
uniform int ctm_type;


void main()
{
	color = vColor;
	// since glsl do not support switch, we use if else

	if (ctm_type == 0) {

	} else if (ctm_type == 1) {
		gl_Position = ctm_base * vPosition;
	} else if (ctm_type == 2) {
		gl_Position = ctm_joint_low * vPosition;
	} else if (ctm_type == 3) {
		gl_Position = ctm_joint_middle * vPosition;
	} else if (ctm_type == 4) {
		gl_Position = ctm_joint_upper * vPosition;
	} else if (ctm_type == 5) {
		gl_Position = ctm_wrist * vPosition;
	}							
}
