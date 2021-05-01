#version 330 core

uniform vec4 obscurityColor;
uniform vec2 center;

void main() {
	vec2 coord = vec2(gl_FragCoord.x, gl_FragCoord.y + (center.y - 400));
	float dist = distance(coord, center);
	float opacity = 0.7;

	if (dist < 100.0) {
		opacity = 0.0;	
	} else if (dist < 300.0) {
		opacity = 0.3;	
	}
	
	gl_FragColor = obscurityColor; 
	gl_FragColor.a *= opacity; 
};
