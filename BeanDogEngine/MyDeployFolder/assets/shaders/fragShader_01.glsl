// Fragment shader
#version 420

in vec4 fVertexColour;			// The vertex colour from the original model
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;

// Replaces gl_FragColor
out vec4 pixelColour;			// RGB A   (0 to 1) 

// The "whole object" colour (diffuse and specular)

// Whole object diffuse colour
uniform vec4 wholeObjectDiffuseColour;

// If true, the whole object colour is used (instead of vertex colour)
uniform bool bUseWholeObjectDiffuseColour;

// Colour of the specular highlight (optional)
uniform vec4 wholeObjectSpecularColour;	

// Alpha transparency value
uniform float wholeObjectAlphaTransparency;

// If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
uniform bool bUseDebugColour;	
uniform vec4 objectDebugColour;

// This will not modulate the colour by the lighting contribution.
// i.e. shows object colour "as is". 
// Used for wireframe or debug type objects
// 1 if you want to AVOID lighting
uniform bool bDontLightObject;

// This is the camera eye location (update every frame)
uniform vec4 eyeLocation;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

uniform sampler2D texture_00;		// Main
uniform sampler2D texture_01;		// Main
uniform sampler2D texture_02;		// Main
uniform sampler2D texture_03;		// Main

uniform sampler2D texture_04;		// bump Map
uniform sampler2D texture_05;		// Depth Map
uniform sampler2D texture_06;		// Normal Map
uniform sampler2D texture_07;		// Transparancy Map

uniform vec4 texture2D_Ratios0to3;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );
uniform vec4 texture2D_Ratios4to7;

//The SkyBox check bool
uniform bool isSkyBox;

// Cube maps for skybox, etc.
uniform samplerCube cubeMap_00;
uniform vec4 cubeMap_Ratios0to3;

//If true do a discard on all black pixles in texture unit 0 (The main texture)
uniform bool mainTextureDiscard;

//Gets set if theres a texture in unit 07
uniform bool isTransparent;

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );
							

void main()
{
	// This is the pixel colour on the screen.
	// Just ONE pixel, though.
	
	pixelColour.rgba = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Set the alpha value: 0.0 = clear, 1.0 = solid
	pixelColour.a = wholeObjectAlphaTransparency;

	if(mainTextureDiscard)
	{
		//Sample the texture
		vec3 vec3DisSample = texture( texture_00, fUVx2.xy ).rgb;
		//Get the average
		float fDisSample = (vec3DisSample.r + vec3DisSample.g + vec3DisSample.b)/3.0f;
		//If the average is black
		if(fDisSample < 0.05f)
		{
			discard;
		}
	}

	if (isSkyBox)
	{
		pixelColour.rgb = texture(cubeMap_00, fNormal.xyz).rgb;
		return;
	}

	// Copy model vertex colours?
	vec4 vertexDiffuseColour = fVertexColour;
	
	// Use model vertex colours or not?
	if ( bUseWholeObjectDiffuseColour )
	{
		vertexDiffuseColour = wholeObjectDiffuseColour;
	}
	
	// Use debug colour?
	if ( bUseDebugColour )
	{
		// Overwrite the vertex colour with this debug colour
		vertexDiffuseColour = objectDebugColour;	
	}

	// Makes this "black" but not quite...
	vertexDiffuseColour.rgb *= 0.0001f;
	
	// Add the texture colors
	vertexDiffuseColour.rgb += 	
			(texture( texture_00, fUVx2.xy ).rgb * texture2D_Ratios0to3.x)  + 
		    (texture( texture_01, fUVx2.xy ).rgb * texture2D_Ratios0to3.y)  + 
			(texture( texture_02, fUVx2.xy ).rgb * texture2D_Ratios0to3.z)  + 
			(texture( texture_03, fUVx2.xy ).rgb * texture2D_Ratios0to3.w);

	// Used for drawing "debug" objects (usually wireframe)
	if ( bDontLightObject )
	{
		pixelColour = vertexDiffuseColour;
		// Early exit from shader
		return;
	}


	vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
	                                        fNormal.xyz, 		// Normal at the vertex (in world coords)
                                            fVertWorldLocation.xyz,	// Vertex WORLD position
											wholeObjectSpecularColour.rgba );
		
	if(isTransparent)
	{
		//Sample the texture
		vec3 vec3TranspSample = texture( texture_07, fUVx2.xy ).rgb;
		//Get the average
		float fTranspSample = (vec3TranspSample.r + vec3TranspSample.g + vec3TranspSample.b)/3.0f;
		//If the average is black
		if(!(fTranspSample < 0.2f))
		{
			//Set the transparency to the transparency texture ratio
			outColour.a = 1 - texture2D_Ratios4to7.w;
		}
	}

	pixelColour = outColour;
};

// Calculates the colour of the vertex based on the lighting and vertex information:
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		

			//Sets the max darkness so the models arent pitch black
			lightContrib.rgb = max(lightContrib.rgb, vec3(0.2f, 0.2f, 0.2f));

//			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
			finalObjectColour.rgb += (vertexMaterialColour.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)
			
			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		//Sets the max darkness
		//lightDiffuseContrib.rgb = max(lightDiffuseContrib.rgb, vec3(1.0f, 1.0f, 1.0f));
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}