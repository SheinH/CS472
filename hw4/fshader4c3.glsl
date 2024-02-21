#version 330

uniform sampler2D  u_shadowMap;

in vec2 v_TexCoord;

out vec4 fragColor;

void main()
{
	float depth = texture(u_shadowMap, v_TexCoord).r;
	fragColor = vec4(depth, depth, depth, 1.0);
}