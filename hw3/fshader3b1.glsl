#version 330

// PUT YOUR CODE HERE
in vec2 v_TexCoord;

uniform	sampler2D u_Sampler;	// uniform variable for the texture image

out vec4 fragColor;

void main() 
{
	// PUT YOUR CODE HERE
	fragColor = texture(u_Sampler, v_TexCoord);
}
