
#include <Lumino.fxh>



































float3 frustumRayTL;
float3 frustumRayTR;
float3 frustumRayBL;
//float2 windowSize = float2(640, 480);//ln_ViewportPixelSize.xy;




float3 v3CameraPos;		// The camera's current position
float fCameraHeight;	// The camera's current height

float3 v3LightPos;		// The direction vector to the light source
float fCameraHeight2;	// fCameraHeight^2

float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
float fScale;			// 1 / (fOuterRadius - fInnerRadius)

float fOuterRadius;		// The outer (atmosphere) radius
float fOuterRadius2;	// fOuterRadius^2
float fInnerRadius;		// The inner (planetary) radius
float fInnerRadius2;	// fInnerRadius^2

float fKrESun;			// Kr * ESun
float fKmESun;			// Km * ESun
float fKr4PI;			// Kr * 4 * PI
float fKm4PI;			// Km * 4 * PI

float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
float fScaleOverScaleDepth;	// fScale / fScaleDepth
float g;
float exposure;




const float3 _LightColor0 = float3(1, 1, 1);
const float3 _WorldSpaceLightPos0 = float3(0, 0.75,  0.75);	// directional light direction
const float _Exposure = 1.3;
const float3 _GroundColor = float3(1, 1, 1);
const float _SunSize = 0.04;
const float3 _SkyTint = float3(0, 0, 1);
const float _AtmosphereThickness = 1;


// RGB wavelengths
// .35 (.62=158), .43 (.68=174), .525 (.75=190)
static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);

#define OUTER_RADIUS 1.025
static const float kOuterRadius = OUTER_RADIUS;
static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;
static const float kInnerRadius = 1.0;
static const float kInnerRadius2 = 1.0;


static const float kCameraHeight = 0.0001;

#define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))		// Rayleigh constant
#define kMIE 0.0010      		// Mie constant
#define kSUN_BRIGHTNESS 20.0 	// Sun brightness

struct VSOutput
{
	float4	svPos		: SV_POSITION;

	float3	vertex			: TEXCOORD0;
	float3	groundColor		: TEXCOORD1;
	float3	skyColor		: TEXCOORD2;
	float3	sunColor		: TEXCOORD3;
};

struct PSInput
{
	float3	vertex			: TEXCOORD0;
	float3	groundColor		: TEXCOORD1;
	float3	skyColor		: TEXCOORD2;
	float3	sunColor		: TEXCOORD3;
};

//------------------------------------------------------------------------------
#define COLOR_2_GAMMA(color) color
#define COLOR_2_LINEAR(color) color*color
#define LINEAR_2_OUTPUT(color) sqrt(color)

float getRayleighPhase(float eyeCos2)
{
	return 0.75 + 0.75*eyeCos2;
}
float getRayleighPhase(float3 light, float3 ray)
{
	float eyeCos	= dot(light, ray);
	return getRayleighPhase(eyeCos * eyeCos);
}

VSOutput VS_Main(LN_VSInput v)
{
	VSOutput o;
	o.svPos = float4(v.Pos, 1.0);

	float3 kSkyTintInGammaSpace = COLOR_2_GAMMA(_SkyTint); // convert tint from Linear back to Gamma
	float3 kScatteringWavelength = lerp (
		kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
		kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
		half3(1,1,1) - kSkyTintInGammaSpace); // using Tint in sRGB gamma allows for more visually linear interpolation and to keep (.5) at (128, gray in sRGB) point
	float3 kInvWavelength = 1.0 / pow(kScatteringWavelength, 4);

	float kKrESun = kRAYLEIGH * kSUN_BRIGHTNESS;
	float kKr4PI = kRAYLEIGH * 4.0 * 3.14159265;

	float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0); 	// The camera's current position

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 eyeRay = normalize(mul((float3x3)ln_World, float3(v.Pos.xy, 1.0)));

	float far = 0.0;
	float3 cIn, cOut;


	if(eyeRay.y >= 0.0)
	{
		cIn = float3(0, 0, 0);
		cOut = float3(1, 1, 1);
	}
	else
	{
		cIn = float3(1, 1, 1);
		cOut = float3(0, 0, 0);
	}

	o.vertex 			= -v.Pos;
	o.groundColor	= _Exposure * (cIn + COLOR_2_LINEAR(_GroundColor) * cOut);
	o.skyColor	= _Exposure * (cIn * getRayleighPhase(_WorldSpaceLightPos0.xyz, -eyeRay));
	o.sunColor	= _Exposure * (cOut * _LightColor0.xyz);


	o.skyColor = cOut;

	//OUT.groundColor	= sqrt(OUT.groundColor);
	//OUT.skyColor	= sqrt(OUT.skyColor);
	//OUT.sunColor= sqrt(OUT.sunColor);
	return o;
}
//------------------------------------------------------------------------------
float4 PS_Main(PSInput input) : SV_TARGET
{
	return float4(input.skyColor, 1.0);
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
