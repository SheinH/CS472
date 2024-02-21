#version 120		// legacy version

varying	  vec4 v_Color;	// varying variable for passing color from vertex shader

void main() 
{
	// legacy version:
	gl_FragColor = v_Color;
}
