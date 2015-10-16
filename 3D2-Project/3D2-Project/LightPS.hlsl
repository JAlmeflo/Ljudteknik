Texture2D shaderTexture[2];
Texture2D depthMapTexture;

SamplerState samplerTypeClamp;
SamplerState samplerTypeWrap;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float fogFactor : FOG;
};

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
	float bias;
	float4 textureColor;
	float4 textureColor2;
	float4 blendColor;
	float lightIntensity;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float4 fogColor;

	bias = 0.001f;

	color = ambientColor;

	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

	// Calc projected texture coordinates
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w * 0.5f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w * 0.5f + 0.5f;

	// if texcoords is whitin (0-1) then pixel is in the view of light
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(samplerTypeClamp, projectTexCoord).r;

		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		// Light hit the pixel
		if (lightDepthValue <= depthValue)
		{
			lightIntensity = saturate(dot(input.normal, input.lightPos));

			if (lightIntensity > 0.0f)
			{
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
			}
		}
	}
	// point is outside of the shadowmap texture
	else
	{
		lightIntensity = saturate(dot(input.normal, input.lightPos));

		if (lightIntensity > 0.0f)
		{
			color += (diffuseColor * lightIntensity);
			color = saturate(color);
		}
	}	
	

	textureColor = shaderTexture[0].Sample(samplerTypeWrap, input.tex);
	textureColor2 = shaderTexture[1].Sample(samplerTypeWrap, input.tex);

	blendColor = textureColor * textureColor2 * 2.0f;
	fogColor.a = textureColor.a;

	color = color * blendColor * input.fogFactor + (1.0 - input.fogFactor) * fogColor;

	return color;
}