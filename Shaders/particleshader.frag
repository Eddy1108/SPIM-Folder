#version 410 core
layout (location = 0) out vec4 oColor;

in vec2 TexCoords;

uniform sampler2D textureSampler;
uniform vec4 uColor;

void main()
{
	//oColor = uColor;
	oColor = texture(textureSampler, TexCoords) * uColor;
	//fragColor = texture(textureSampler, UV).rgb;
}