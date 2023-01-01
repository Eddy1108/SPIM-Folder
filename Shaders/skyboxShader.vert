#version 410 core
layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec3 TexCoords;

uniform mat4 rotMatrix; //Used to rotate the cubemap
uniform mat4 pMatrix;
uniform mat4 vMatrix;

void main()
{
    TexCoords = positionIn;
    mat4 view = mat4(mat3(vMatrix));

    gl_Position = pMatrix * view * rotMatrix * vec4(positionIn, 1.0);
}
