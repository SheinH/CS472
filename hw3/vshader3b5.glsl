#version 330

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
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  u_Projection * u_View * vec4(a_Position, 1);
		
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (vec4(a_Position, 1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	EyeDirection_cameraspace = vec3(0,0,0) - ( u_View * vec4(a_Position, 1)).xyz;

	// Vector that goes from the vertex to the light, in camera space
	LightDirection_cameraspace = (u_View * vec4(u_LightDirection, 0)).xyz;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( u_View * vec4(a_Normal, 0)).xyz; 

	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	v_TexCoord = a_TexCoord;
}
