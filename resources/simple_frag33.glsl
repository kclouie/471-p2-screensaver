#version 330 core
uniform int W;
uniform int H;
out vec4 color;

void main()
{
	if (distance(gl_PointCoord, vec2(0.5, 0.5)) < 0.5){
		vec2 center = vec2(W / 2, H / 2);
		int radius = 240;
		if (distance(gl_FragCoord, vec4(center, 0.0, 1.0)) <= radius){
			color = vec4(0.0, 1.0, 0.0, .5) * (1 - distance(gl_PointCoord, vec2(0.5, 0.5)));
		}
		else {
			color = vec4(1.0, 1.0, 0.0, .5) * (1 - distance(gl_PointCoord, vec2(0.5, 0.5)));
		}
	}
	else {
		discard;
	}
}
