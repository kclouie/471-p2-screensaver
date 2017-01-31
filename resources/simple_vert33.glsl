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
	float d1 = distance(vertPos, vec3(0.0, 0.0, 0.0));
	float d2 = distance(trans, vec4(0.0, 0.0, 0.0, 1.0));
	if (d2 > 1.0 && d2 < d1){
		gl_Position = P * MV * trans;
	}
	else {
		float angle = atan(vertPos.y, vertPos.x);
		float x = cos(angle);
		float y = sin(angle);
		gl_Position = P * MV * vec4(x, y, 0.0, 1.0);
	}
}
