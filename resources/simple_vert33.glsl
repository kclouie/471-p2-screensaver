#version 330 core
layout(location = 0) in vec3 vertPos;
uniform mat4 P;
uniform mat4 MV;
uniform float T;

void main()
{
/* Using distances */
	float angle = atan(vertPos.y, vertPos.x);
	vec4 intersect = vec4(cos(angle), sin(angle), 0.0, 1.0);	// uses line angle to find point of intersection with circle
	vec3 v = vec3(0.0, 0.0, 0.0) - vertPos;				// vector to move points in towards center
	float dInner = distance(intersect, vec4(0.0, 0.0, 0.0, 1.0));	// dist from edge of circle to center
	vec4 trans = vec4(vertPos + (T*.15) * v, 1.0); 			// transformed vertPos
	float dOuter = distance(trans, vec4(0.0, 0.0, 0.0, 1.0));	// dist from transformed vertPos to center
	float implres = pow(0.0 - vertPos.x, 2) - pow(0.0 - vertPos.y, 2) - 1;
	if (dOuter > dInner && distance(vec4(vertPos, 1.0), trans) < distance(vertPos, vec3(0.0, 0.0, 0.0))){
		gl_Position = P * MV * trans;
	}
	else {
		gl_Position = P * MV * intersect;
	}
}
