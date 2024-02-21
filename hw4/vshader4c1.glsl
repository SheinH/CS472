#version 330

in vec3  a_Position;	// attribute variable: position vector

uniform mat4 u_depthMVP;

void main() 
{
	gl_Position = u_depthMVP * vec4(a_Position,1);
}