
#include <Lumino.fxh>

float4x4 _localWorld;
float4x4 _scaleMatrix;

const float PI = 3.14159;
const float Kr = 0.0025;		// Rayleigh scattering constant
const float Km = 0.0010;		// Mie scattering constant
const float ESun = 20.0f;		// Sun brightness constant
const float3 fWavelength = float3(0.650, 0.570, 0.475);
const float3 fWavelength4 = pow(fWavelength, 4.0);

uniform float3 v3CameraPos;		// The camera's current position
uniform float fCameraHeight;	// The camera's current height
const float3 v3LightPos = float3(0, 1, 0);		// The direction vector to the light source
const float3 v3InvWavelength = 1.0 / fWavelength4;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
const float fOuterRadius = 10.25f;						// The outer (atmosphere) radius
const float fOuterRadius2 = fOuterRadius * fOuterRadius;	// fOuterRadius^2
const float fInnerRadius = 10.0f;							// The inner (planetary) radius
const float fInnerRadius2 = fInnerRadius * fInnerRadius;	// fInnerRadius^2
const float fKrESun = Kr * ESun;
const float fKmESun = Km * ESun;
const float fKr4PI = Kr * 4.0 * PI;
const float fKm4PI = Km * 4.0 * PI;
const float fScale = 1.0 / (fOuterRadius - fInnerRadius);			// 1 / (fOuterRadius - fInnerRadius)
const float fScaleDepth = 0.25;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
const float fScaleOverScaleDepth = ((1.0 / (fOuterRadius - fInnerRadius)) / fScaleDepth);

const int nSamples = 2;
const float fSamples = 2.0;

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
};

struct PSInput
{
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
};


//------------------------------------------------------------------------------

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

VSOutput VS_Main(LN_VSInput v)
{
	VSOutput o;
	o.svPos = float4(v.Pos, 1.0);

	float3 viewportPos = float3(v.Pos.x, v.Pos.y, 1.0);
	float3 localRay = mul(viewportPos, (float3x3)_scaleMatrix);
	float3 v3Ray = normalize(mul(localRay, (float3x3)_localWorld));
	float fFar = length(v3Ray);

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	o.FrontSecondaryColor.rgb = v3FrontColor * fKmESun;
	o.FrontColor.rgb = v3FrontColor * (v3InvWavelength * fKrESun);
	o.v3Direction = -v3Ray;//v3CameraPos - v3Pos;

	return o;
}
//------------------------------------------------------------------------------

#define MIE_G (-0.990)
#define MIE_G2 0.9801

float4 PS_Main(PSInput input) : SV_TARGET
{
	float fCos = dot(v3LightPos, input.v3Direction) / length(input.v3Direction);
	float fMiePhase = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + fCos*fCos) / pow(1.0 + MIE_G2 - 2.0*MIE_G*fCos, 1.5);
	float4 color;
    color.rgb = input.FrontColor + fMiePhase * input.FrontSecondaryColor;
	color.a = color.b;
	return color;
}
//------------------------------------------------------------------------------


technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
