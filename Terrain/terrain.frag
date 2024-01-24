#version 430

layout(location = 0) out vec4 FragColor;

flat in vec4 Color;
in vec3 crntPos;
in vec3 crntNormal;
in vec2 texCoords;

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;

uniform float h0;
uniform float h1;
uniform float h2;
uniform float h3;

uniform vec3 lightDir = normalize(vec3(0.8, -1.0, 0.2));

vec4 CalcTexColor()
{
	vec4 texColor;
	float height = crntPos.y;

	if(height < h0)
		texColor = texture(diffuse0, texCoords);
	else if(height < h1)
	{
		vec4 color0 = texture(diffuse0, texCoords);
		vec4 color1 = texture(diffuse1, texCoords);
		float factor = (height - h0) / (h1 - h0);
		texColor = mix(color0, color1, factor);
	}
	else if(height < h2)
	{
		vec4 color0 = texture(diffuse1, texCoords);
		vec4 color1 = texture(diffuse2, texCoords);
		float factor = (height - h1) / (h2 - h1);
		texColor = mix(color0, color1, factor);
	}
	else if(height < h3)
	{
		vec4 color0 = texture(diffuse2, texCoords);
		vec4 color1 = texture(diffuse3, texCoords);
		float factor = (height - h2) / (h3 - h2);
		texColor = mix(color0, color1, factor);
	}
	else
		texColor = texture(diffuse3, texCoords);

	return texColor;
}

void main()
{
	vec4 texColor = CalcTexColor();
	float dirIntensity = clamp(dot(normalize(crntNormal), lightDir), 0.3, 1.0);
	FragColor = texColor * dirIntensity;
}