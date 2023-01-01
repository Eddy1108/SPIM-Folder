#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec2 TexCoords;

uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 uTransform;

void main()
{
	TexCoords = vertexUV;

	mat4 uViewProj = pMatrix * vMatrix;

	gl_Position = uViewProj * uTransform * vec4(aPosition, 1.0);
}