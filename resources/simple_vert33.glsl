#version 330 core
layout(location = 0) in vec3 vertPos;
uniform mat4 P;
uniform mat4 MV;
uniform int W;
uniform int H;
uniform float T;

void main()
{
	vec3 v = vec3(0.0, 0.0, 0.0) - vertPos;
	vec4 trans = vec4(vertPos + (T*.25) * v, 1.0);
	gl_Position = P * MV * trans;
//	if (distance(trans, vec4(0.0, 0.0, 0.0, 1.0)) > 1){
//		gl_Position = P * MV * trans;
//	}
}
