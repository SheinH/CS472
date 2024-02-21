#version 330

// Interpolated values from the vertex shaders
in vec3 vColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec4 ShadowCoord;

out vec4 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2DShadow u_shadowMap;


vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);


void main()
{
	// Light emission properties
// PUT YOUR CODE HERE
	
	// Material properties
// PUT YOUR CODE HERE

	// Normal of the computed fragment, in camera space
// PUT YOUR CODE HERE

	// Direction of the light (from the fragment to the light)
// PUT YOUR CODE HERE

	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendiular to the triangle -> 0
	//  - light is behind the triangle -> 0
// PUT YOUR CODE HERE
	
	// Eye vector (towards the camera)
// PUT YOUR CODE HERE

	// Direction in which the triangle reflects the light
// PUT YOUR CODE HERE

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
// PUT YOUR CODE HERE
	
	float visibility = 1.0;

	// ...variable bias
// PUT YOUR CODE HERE

	// Sample the shadow map 4 times
	for (int i=0; i<4; ++i) {
		// use either :
		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
		// being fully in the shadow will eat up 4*0.2 = 0.8
		// 0.2 potentially remain, which is quite dark.
		visibility -= 0.2*(1.0-texture( u_shadowMap, vec3(ShadowCoord.xy + poissonDisk[i]/1800.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));

	}
	
	// Ambient : simulates indirect lighting
	// Diffuse : "color" of the object
	// Specular : reflective highlight, like a mirror

	fragColor = vec4((MaterialAmbientColor +
		       visibility * LightPower * cosTheta * MaterialDiffuseColor * LightColor +
		       visibility * LightPower * pow(cosAlpha, 5) * MaterialSpecularColor * LightColor), 1.0);
		       


}
