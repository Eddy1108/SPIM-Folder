#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 rotMatrix;	//rotate the skybox
uniform mat4 mMatrix;
uniform mat4 pMatrix;
uniform mat4 vMatrix;

void main()
{
	vec4 pos = pMatrix * vMatrix * rotMatrix * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
	texCoords = aPos.xyz;
}