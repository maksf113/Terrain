#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texUV;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat3 transposeInverse;
uniform float minHeight;
uniform float maxHeight;

flat out vec4 Color;
out vec3 crntPos;
out vec3 crntNormal;
out vec2 texCoords;

void main()
{
	gl_Position = camMatrix * model * vec4(position, 1.0);
	//crntPos = position;
	crntPos = vec3(model * vec4(position, 1.0));
	crntNormal = transposeInverse * normal;
	Color = vec4(vec3((position.y - minHeight) / (maxHeight - minHeight)), 1.0);
	texCoords = texUV;
}