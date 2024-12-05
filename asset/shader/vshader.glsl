#version 120

attribute vec4 vPosition, vColor, vNormal;
varying vec4 color;


uniform mat4 ctm_base, ctm_joint_low, ctm_joint_middle, ctm_joint_upper, ctm_wrist, ctm_finger1, ctm_finger2;
uniform int ctm_type, fake_shadow;
uniform mat4 view_mat, projection_mat;

uniform vec4 light_position;
vec4 ambient, diffuse, specular;

void main()
{
	mat4 ctm;
	if (ctm_type == 0) {
	} else if (ctm_type == 1) {
		ctm = ctm_base;
	} else if (ctm_type == 2) {
		ctm = ctm_joint_low;
	} else if (ctm_type == 3) {
		ctm = ctm_joint_middle;
	} else if (ctm_type == 4) {
		ctm = ctm_joint_upper;
	} else if (ctm_type == 5) {
		ctm = ctm_wrist;
	} else if (ctm_type == 6) {
		ctm = ctm_finger1;
	} else if (ctm_type == 7) {
		ctm = ctm_finger2;
	}


	ambient = 0.3 * vColor;
	vec4 normal = normalize(vNormal);
	vec4 n = normalize(view_mat * ctm * vNormal);
	vec4 l = normalize(view_mat * (light_position - ctm * vPosition));
	diffuse = max(dot(n, l), 0.0) * vColor;
	vec4 v = normalize(vec4(0,0,0,1) - view_mat * ctm * vPosition);
	vec4 h = normalize(l + v);
	specular = pow(max(dot(n, h), 0.0), 50) * vec4(1,1,1,1);

	if(fake_shadow == 0){
		color = ambient + diffuse + specular;
		gl_Position = projection_mat * view_mat * ctm * vPosition;
	} else {
		// color = vec4(0,0,0,1);
		// we use similar triangle to calculate the shadow
		// we set y to 0.0056 since it is on the ground
		// set x and z according to the ratio between light and this vertices
		vec4 xyz = ctm * vPosition;
		float x_diff = xyz.x - light_position.x;
		float z_diff = xyz.z - light_position.z;
		float y_diff = xyz.y - light_position.y;
		xyz.w = 1.0;

		xyz.x = light_position.x + x_diff / (y_diff)*(0.01-light_position.y);
		xyz.z = light_position.z + z_diff / (y_diff)*(0.01-light_position.y);
		xyz.y = 0.01;
		gl_Position = projection_mat * view_mat * xyz;
		color = vColor;
	}

}
