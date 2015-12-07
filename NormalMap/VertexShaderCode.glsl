#version 430
	
in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec2 uv;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec3 tangent;

//uniform vec3 lightPosition;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
//uniform bool drawSkybox;
//uniform vec3 world_cameraPosition;
//uniform mat3 normalMatrix;
uniform vec3 camera;

//out vec3 theColour;
out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec2 daUV;
out vec3 tangentWorld;
//out vec3 reflectDir;

void main()
{
    //if( drawSkybox )
    //{
        //reflectDir = vertexPositionModel;
    //}
    //else
    //{	
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//vertexPositionWorld = vec4(vertexPositionModel, 1);
	//normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0)); 
		//vec3 world_view = normalize( world_cameraPosition - vertexPositionWorld );
		
	//reflectDir = reflect( -world_view, normalWorld );
	//}

	tangentWorld = tangent;
	gl_Position = modelToProjectionMatrix * vec4(vertexPositionModel, 1);	
	vertexPositionWorld = vertexPositionModel;
	daUV = uv;

	//vec3 vView = gl_Vertex.xyz - camera.xyz; 
    //vReflection = reflect(vView, gl_Normal);
}