#version 120			// use 120 (legacy) to switch between standard OpenGL and shaders

attribute vec2  a_Position;	// attribute variable: position vector
attribute vec3  a_Color;	// attribute variable: color vector

uniform   mat4  u_ProjMatrix;	// uniform variable for passing projection matrix
uniform   mat4  u_ModelMatrix;	// uniform variable for passing rotation matrix

varying   vec4  v_Color;	// varying variable for passing color to fragment shader

void main() 
{
	gl_Position = u_ProjMatrix * u_ModelMatrix * vec4(a_Position, 0, 1);
	v_Color	= vec4(a_Color, 1);
}
