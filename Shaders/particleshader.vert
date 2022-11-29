#version 410 core

layout (location = 0) in vec3 aPosition;

//uniform mat4 uViewProj;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 uTransform;

void main()
{
	mat4 uViewProj = pMatrix * vMatrix;

	gl_Position = uViewProj * uTransform * vec4(aPosition, 1.0);
}