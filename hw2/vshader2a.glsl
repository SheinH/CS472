#version 330

in	vec2	a_Position;	// attribute variable
uniform	mat4	u_Projection;	// uniform variable for passing projection matrix

void main()
{
    gl_Position = u_Projection * vec4(a_Position, 0, 1);
    gl_PointSize = 4;
}
