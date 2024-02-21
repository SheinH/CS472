#version 330

in	vec2	a_Position;	// attribute variable: position vector
in	vec2	a_TexCoord;	// attribute variable: texture coordinate

uniform mat4	u_Modelview;	// uniform variable for modelview matrix
uniform mat4	u_Projection;	// uniform variable for projection matrix
uniform float	u_Theta;	// Theta parameter
uniform int	u_Twist;	// Twist flag

out	vec2	v_TexCoord;	// varying variable for passing texture coordinate to fragment shader

void main() {
	if(u_Twist == 0)
		gl_Position = u_Projection * u_Modelview * vec4(a_Position, 0, 1);
	else {
		float x = a_Position.x;
		float y = a_Position.y;
		float t = sqrt(x*x + y*y) * u_Theta;
		gl_Position = u_Projection * vec4(x*cos(t)-y*sin(t), x*sin(t)+y*cos(t), 0, 1);
	}
	v_TexCoord = a_TexCoord;
}
