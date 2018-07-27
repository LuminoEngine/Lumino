
float4 _SkyTint;// ("Sky Tint", Color) = (.5, .5, .5, 1)
float _AtmosphereThickness;// ("Atmosphere Thickness", Range(0,5)) = 1.0
float4 _WorldSpaceLightPos0;	// DIRECTIONAL_LIGHT
float _Exposure;//("Exposure", Range(0, 8)) = 1.3
float4 _GroundColor;// ("Ground", Color) = (.369, .349, .341, 1)
float _SunSize;// ("Sun Size", Range(0,1)) = 0.04


float4 _LightColor0;

float4x4 _VPInv;


#define SKYBOX_SUNDISK 1
#define SKYBOX_SUNDISK_NONE 0
#define SKYBOX_SUNDISK_HQ 1
#define SKYBOX_SUNDISK_SIMPLE 2

#define UNITY_COLORSPACE_GAMMA
#if defined(UNITY_COLORSPACE_GAMMA)
	#define GAMMA 2
	#define COLOR_2_GAMMA(color) color
	#define COLOR_2_LINEAR(color) color*color
	#define LINEAR_2_OUTPUT(color) sqrt(color)
#else
	#define GAMMA 2.2
	// HACK: to get gfx-tests in Gamma mode to agree until UNITY_ACTIVE_COLORSPACE_IS_GAMMA is working properly
	#define COLOR_2_GAMMA(color) ((unity_ColorSpaceDouble.r>2.0) ? pow(color,1.0/GAMMA) : color)
	#define COLOR_2_LINEAR(color) color
	#define LINEAR_2_LINEAR(color) color
#endif


// RGB wavelengths
// .35 (.62=158), .43 (.68=174), .525 (.75=190)
static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);

#define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))		// Rayleigh constant
#define kMIE 0.0010      		// Mie constant
#define kSUN_BRIGHTNESS 20.0 	// Sun brightness

static const float kInnerRadius = 1.0;
static const float kInnerRadius2 = kInnerRadius * kInnerRadius;
static const float kCameraHeight = 0.0001;

#define OUTER_RADIUS 1.025
static const float kOuterRadius = OUTER_RADIUS;
static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;

static const float kSunScale = 400.0 * kSUN_BRIGHTNESS;
static const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
static const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH
static const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
static const float kScaleDepth = 0.25;

#define kMAX_SCATTER 50.0 // Maximum scattering value, to prevent math overflows on Adrenos

static const float kKm4PI = kMIE * 4.0 * 3.14159265;
static const float kKmESun = kMIE * kSUN_BRIGHTNESS;

#define SKY_GROUND_THRESHOLD 0.02

struct appdata_t
{
	float4 vertex : POSITION;
};

struct Varying
{
	float4	pos				: SV_POSITION;

#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
	// for HQ sun disk, we need vertex itself to calculate ray-dir per-pixel
	float3	vertex			: TEXCOORD0;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
	float3	rayDir			: TEXCOORD0;
#else
	// as we dont need sun disk we need just rayDir.y (sky/ground threshold)
	float	skyGroundFactor	: TEXCOORD0;
#endif

	// calculate sky colors in vprog
	float3	groundColor		: TEXCOORD1;
	float3	skyColor		: TEXCOORD2;

//#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
	float3	sunColor		: TEXCOORD3;
//#endif

	//UNITY_VERTEX_OUTPUT_STEREO
};


float scale(float inCos)
{
	float x = 1.0 - inCos;
//#if defined(SHADER_API_N3DS)
	// The polynomial expansion here generates too many swizzle instructions for the 3DS vertex assembler
	// Approximate by removing x^1 and x^2
//	return 0.25 * exp(-0.00287 + x*x*x*(-6.80 + x*5.25));
//#else
	return 0.25 * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
//#endif
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float eyeCos2)
{
	return 0.75 + 0.75*eyeCos2;
}
float getRayleighPhase(float3 light, float3 ray)
{
	float eyeCos	= dot(light, ray);
	return getRayleighPhase(eyeCos * eyeCos);
}

float calcSunSpot(half3 vec1, half3 vec2)
{
	half3 delta = vec1 - vec2;
	half dist = length(delta);
	half spot = 1.0 - smoothstep(0.0, _SunSize, dist);
	return kSunScale * spot * spot;
}

#define MIE_G (-0.990)
#define MIE_G2 0.9801
// Calculates the Mie phase function
float getMiePhase(float eyeCos, float eyeCos2)
{
	float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
	temp = pow(temp, pow(_SunSize,0.65) * 10);
	temp = max(temp,1.0e-4); // prevent division by zero, esp. in half precision
	temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;
	#if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
		temp = pow(temp, .454545);
	#endif
	return temp;
}

Varying vsMain(appdata_t v)
{
	Varying OUT;
	//UNITY_SETUP_INSTANCE_ID(v);
	//UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(OUT);
	OUT.pos = v.vertex;//UnityObjectToClipPos(v.vertex);

	float3 kSkyTintInGammaSpace = COLOR_2_GAMMA(_SkyTint); // convert tint from Linear back to Gamma
	float3 kScatteringWavelength = lerp (
		kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
		kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
		float3(1,1,1) - kSkyTintInGammaSpace); // using Tint in sRGB gamma allows for more visually linear interpolation and to keep (.5) at (128, gray in sRGB) point
	float3 kInvWavelength = 1.0 / pow(kScatteringWavelength, 4);

	float kKrESun = kRAYLEIGH * kSUN_BRIGHTNESS;
	float kKr4PI = kRAYLEIGH * 4.0 * 3.14159265;

	float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0); 	// The camera's current position

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	// 視界の far point への Ray
	//float3 eyeRay = normalize(v.vertex.xyz);////normalize(mul((float3x3)unity_ObjectToWorld, v.vertex.xyz));
	float3 eyeRay = normalize(mul(float4(v.vertex.xyz, 1), _VPInv).xyz);
	

	float far = 0.0;
	float3 cIn, cOut;

	if(eyeRay.y >= 0.0)
	{
		// Sky - Ray の先は空中側


		// Calculate the length of the "atmosphere"
		far = sqrt(kOuterRadius2 + kInnerRadius2 * eyeRay.y * eyeRay.y - kInnerRadius2) - kInnerRadius * eyeRay.y;

		float3 pos = cameraPos + far * eyeRay;

		// Calculate the ray's starting position, then calculate its scattering offset
		float height = kInnerRadius + kCameraHeight;
		float depth = exp(kScaleOverScaleDepth * (-kCameraHeight));
		float startAngle = dot(eyeRay, cameraPos) / height;
		float startOffset = depth*scale(startAngle);


		// Initialize the scattering loop variables
		float sampleLength = far / kSamples;
		float scaledLength = sampleLength * kScale;
		float3 sampleRay = eyeRay * sampleLength;
		float3 samplePoint = cameraPos + sampleRay * 0.5;

		// Now loop through the sample rays
		float3 frontColor = float3(0.0, 0.0, 0.0);
		// Weird workaround: WP8 and desktop FL_9_1 do not like the for loop here
		// (but an almost identical loop is perfectly fine in the ground calculations below)
		// Just unrolling this manually seems to make everything fine again.
//				for(int i=0; i<int(kSamples); i++)
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
			float cameraAngle = dot(eyeRay, samplePoint) / height;
			float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
			float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}
		// ↑↓同じ処理して加算
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
			float cameraAngle = dot(eyeRay, samplePoint) / height;
			float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
			float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}



		// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
		cIn = frontColor * (kInvWavelength * kKrESun);
		cOut = frontColor * kKmESun;
	}
	else
	{
		// Ground - Ray の先は地面側

		far = (-kCameraHeight) / (min(-0.001, eyeRay.y));

		float3 pos = cameraPos + far * eyeRay;

		// Calculate the ray's starting position, then calculate its scattering offset
		float depth = exp((-kCameraHeight) * (1.0/kScaleDepth));
		float cameraAngle = dot(-eyeRay, pos);
		float lightAngle = dot(_WorldSpaceLightPos0.xyz, pos);
		float cameraScale = scale(cameraAngle);
		float lightScale = scale(lightAngle);
		float cameraOffset = depth*cameraScale;
		float temp = (lightScale + cameraScale);

		// Initialize the scattering loop variables
		float sampleLength = far / kSamples;
		float scaledLength = sampleLength * kScale;
		float3 sampleRay = eyeRay * sampleLength;
		float3 samplePoint = cameraPos + sampleRay * 0.5;

		// Now loop through the sample rays
		float3 frontColor = float3(0.0, 0.0, 0.0);
		float3 attenuate;
//				for(int i=0; i<int(kSamples); i++) // Loop removed because we kept hitting SM2.0 temp variable limits. Doesn't affect the image too much.
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float scatter = depth*temp - cameraOffset;
			attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));
			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}

		cIn = frontColor * (kInvWavelength * kKrESun + kKmESun);
		cOut = clamp(attenuate, 0.0, 1.0);
	}

#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
	OUT.vertex 			= -v.vertex;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
	OUT.rayDir 			= float3(-eyeRay);
#else
	OUT.skyGroundFactor	= -eyeRay.y / SKY_GROUND_THRESHOLD;
#endif

	// if we want to calculate color in vprog:
	// 1. in case of linear: multiply by _Exposure in here (even in case of lerp it will be common multiplier, so we can skip mul in fshader)
	// 2. in case of gamma and SKYBOX_COLOR_IN_TARGET_COLOR_SPACE: do sqrt right away instead of doing that in fshader

	OUT.groundColor	= _Exposure * (cIn + COLOR_2_LINEAR(_GroundColor) * cOut);
	OUT.skyColor	= _Exposure * (cIn * getRayleighPhase(_WorldSpaceLightPos0.xyz, -eyeRay));

//#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
	OUT.sunColor	= _Exposure * (cOut * _LightColor0.xyz);
//#endif

#if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
	OUT.groundColor	= sqrt(OUT.groundColor);
	OUT.skyColor	= sqrt(OUT.skyColor);
	//#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
		OUT.sunColor= sqrt(OUT.sunColor);
	//#endif
#endif

	return OUT;
}


float4 psMain(Varying IN) : SV_Target
{
	float3 col = float3(0.0, 0.0, 0.0);

// if y > 1 [eyeRay.y < -SKY_GROUND_THRESHOLD] - ground
// if y >= 0 and < 1 [eyeRay.y <= 0 and > -SKY_GROUND_THRESHOLD] - horizon
// if y < 0 [eyeRay.y > 0] - sky
#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
	//float3 ray = normalize(IN.vertex);//normalize(mul((float3x3)unity_ObjectToWorld, IN.vertex));
	float3 ray = normalize(mul(float4(IN.vertex.xyz, 1), _VPInv).xyz);
	float y = ray.y / SKY_GROUND_THRESHOLD;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
	float3 ray = IN.rayDir.xyz;
	float y = ray.y / SKY_GROUND_THRESHOLD;
#else
	float y = IN.skyGroundFactor;
#endif

	// if we did precalculate color in vprog: just do lerp between them
	col = lerp(IN.skyColor, IN.groundColor, saturate(y));

#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
	if(y < 0.0)
	{
	#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
		float mie = calcSunSpot(_WorldSpaceLightPos0.xyz, -ray);
	#else // SKYBOX_SUNDISK_HQ
		float eyeCos = dot(_WorldSpaceLightPos0.xyz, ray);
		float eyeCos2 = eyeCos * eyeCos;
		float mie = getMiePhase(eyeCos, eyeCos2);
	#endif

		col += mie * IN.sunColor;
	}
#endif

#if defined(UNITY_COLORSPACE_GAMMA) && !SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
	col = LINEAR_2_OUTPUT(col);
#endif

	return float4(col,1.0);

}




technique MainDraw
{
	pass P0
	{
		VertexShader = vsMain;
        PixelShader = psMain;
	}
}
