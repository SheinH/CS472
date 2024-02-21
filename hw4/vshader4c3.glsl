#version 330

in vec3		a_Position;	// attribute variable: position vector

out vec2	v_TexCoord;

void main()
{
	gl_Position =  vec4(a_Position, 1);
	v_TexCoord  = (a_Position.xy + vec2(1, 1))/ 2.0;
}

