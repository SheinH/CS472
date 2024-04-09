#version 330

// PUT YOUR CODE HERE
in vec3  a_Position;
in vec2  a_TexCoord;
in vec3  a_Color;
in vec3  a_Normal;

out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec3 u_LightDirection;

void main() 
{
    gl_Position =  u_Projection * u_View * vec4(a_Position, 1);
    v_TexCoord = a_TexCoord;
}
