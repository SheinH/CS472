#version 330

// legacy version (e.g., #version 120 or 130) of the following code would:
// replace "in"  with "attribute" and replace "out" with "varying"

in vec2  a_Position;		// attribute variable: position vector
in vec3  a_Color;		// attribute variable: color vector

uniform   mat4  u_ProjMatrix;	// uniform variable for passing projection matrix
uniform   mat4  u_ModelMatrix;	// uniform variable for passing rotation matrix
uniform   int   u_ReverseColor;	// uniform variable for passing flag

out   vec4  v_Color;		// varying variable for passing color to fragment shader

void main() 
{
	// legacy version would replace u_ProjMatrix with gl_ModelViewProjectionMatrix 
	gl_Position = u_ProjMatrix * u_ModelMatrix * vec4(a_Position, 0, 1);

	if (u_ReverseColor == 1)
		v_Color = vec4(1.0-a_Color.r, 1.0-a_Color.g, 1.0-a_Color.b, 1.0);
	else	v_Color	= vec4(a_Color, 1);
}
