#version 330 core
out vec4 color;

void main()
{
//*	color = vec3(1.0, 1.0, 0.0);
	if (distance(gl_PointCoord, vec2(0.5, 0.5)) < 0.5){
		color = vec4(1.0, 1.0, 0.0, .5) * distance(gl_PointCoord, vec2(0.5, 0.5));
	}
	else {
		discard;
	}
}
