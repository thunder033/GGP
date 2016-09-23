



// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);
DirectionalLight light : register(b0);
DirectionalLight light2 : register(b1);
float4 Color: register(b2);

float4 getLightColor(DirectionalLight light, float3 normal) {
	float3 lightDir = normalize(-light.direction);
	float lightAmount = saturate(dot(lightDir, normal));

	return lightAmount * light.diffuseColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

	//Sample the texure and use material color
	float4 textureColor = Texture.Sample(Sampler, input.uv) * Color;

	return (getLightColor(light, input.normal) * textureColor) 
		+ (getLightColor(light2, input.normal) * textureColor)
		+ (light.ambientColor * textureColor)
		+ (light2.ambientColor * textureColor);
}