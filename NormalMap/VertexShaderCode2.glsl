#version 430
	
in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec2 uv;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec3 tangent;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 view;

out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec2 daUV;
out vec3 tangentWorld;

void main()
{
	tangentWorld = tangent;
	gl_Position = modelToProjectionMatrix * view * vec4(vertexPositionModel, 1);	
	vertexPositionWorld = vertexPositionModel;
	daUV = uv;

}