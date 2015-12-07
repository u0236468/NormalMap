#version 430

out vec4 daColour;

in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 daUV;
in vec3 tangentWorld;
in vec3 reflectDir;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;
uniform samplerCube meTexture;
//uniform samplerCube cubemapTexture;
//uniform vec3 materialColor;
//uniform bool drawSkybox;
//uniform float reflectFactor;
//varying vec3 vReflection;

void main()
{
    //vec3 color = texture( cubemapTexture, reflectDir ).rgb;

    /*if( drawSkybox )
    {
        daColour = vec4( color, 1.0 );
    }
    else
    {
        daColour = vec4( mix( materialColor, color, reflectFactor ), 1.0 );
    }*/

	/*vec3 tangent = normalize(tangentWorld);
	vec3 biTangent = cross(normalWorld, tangentWorld);
	mat3 normalRotator = mat3(tangent, biTangent, normalWorld);

	//Diffuse
	//vec3 normalWorld = normalRotator * (2 * vec3(texture(meTexture, daUV)) - 1);
	//normalWorld = vec3(0.0f, 0.0f, 1.0f);
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(normalize(lightVectorWorld), normalize(normalWorld));    //(vec3(0,1,0)));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);*/

	//Specular
	/*vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float specularity = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld),0,1);
	specularity = pow(specularity, 10);
	vec4 specularLight = vec4(0, specularity, specularity, 1);*/

	//daColour = vec4(1,0,0,1); //clamp(diffuseLight, 0, 1);

	daColour = texture(meTexture, vertexPositionWorld);
	//daColour = texture(meTexture, vReflection);
}