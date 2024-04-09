#version 330

// PUT YOUR CODE HERE
// input vertex data
in vec3  a_Position;
in vec2  a_TexCoord;
in vec3  a_Color;
in vec3  a_Normal;

// output data; will be interpolated for each fragment.
out vec2 v_TexCoord;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec3 u_LightDirection;

void main() 
{
	// PUT YOUR CODE HERE
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  u_Projection * u_View * vec4(a_Position, 1);
		
}

