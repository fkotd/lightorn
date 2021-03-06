#version 330 core

uniform vec4 obscurityColor;
uniform vec2 center;
uniform float centerRadius;
uniform float middleRadius;
uniform float outerRadius;

void main() {
	vec2 coord = vec2(gl_FragCoord.x, gl_FragCoord.y + (center.y - 400));  // why ?
	float dist = distance(coord, center);
	float opacity = 0.99;

	if (dist < centerRadius) {
		opacity = 0.0;	
	} else if (dist < middleRadius) {
		opacity = 0.3;	
	} else if (dist < outerRadius ) {
		opacity = 0.7;	
	}
	
	gl_FragColor = obscurityColor; 
	gl_FragColor.a *= opacity; 
};
