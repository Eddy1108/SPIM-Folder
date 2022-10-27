#version 410 core
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec4 colorOut;
out vec2 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform vec3 Pos;

float Size = 1.0f;

vec3 CameraRight = vec3(vMatrix[0][0], vMatrix[1][0], vMatrix[2][0]);
vec3 CameraUp = vec3(vMatrix[0][1], vMatrix[1][1], vMatrix[2][1]);

//uniform vec3 vertexPosition_worldspace;

void main() 
{
	mat4 VP = pMatrix * vMatrix;

	vec4 v1 = VP * vec4(Pos + CameraRight * 0.5 * Size + CameraUp * -0.5 * Size, 1.0);  
	vec4 v2 = VP * vec4(Pos + CameraRight * 0.5 * Size + CameraUp * 0.5 * Size, 1.0);     
	vec4 v3 = VP * vec4(Pos + CameraRight * -0.5 * Size + CameraUp * -0.5 * Size, 1.0); 
	vec4 v4 = VP * vec4(Pos + CameraRight * -0.5 * Size + CameraUp * 0.5 * Size, 1.0);



//   colorOut = colorIn;
//   UV = vertexUV;
//   gl_Position = pMatrix * vMatrix * mMatrix * positionIn;
}