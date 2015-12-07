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
uniform samplerCube cube;

void main()
{

	vec3 biTangent = cross(normalWorld, tangentWorld);
	mat3 normalRotator = mat3(tangent, biTangent, normalWorld);

	//Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(normalize(lightVectorWorld), normalize(normalWorld));    //(vec3(0,1,0)));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);*/

	daColour = texture(cube, vertexPositionWorld);
}