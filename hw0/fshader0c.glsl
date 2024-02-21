#version 330		// modern version

in  vec4 v_Color;	// varying variable for passing color from vertex shader
out vec4 fragColor;	// legacy version would have used gl_FragColor instead

void main() 
{
	// legacy version (#version 120) would use: gl_FragColor = v_Color;
	// gl_FragColor can't be used with #version 330
	fragColor = v_Color;
}
