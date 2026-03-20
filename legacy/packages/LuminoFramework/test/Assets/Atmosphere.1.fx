
float3 frustumRayTL;
float3 frustumRayTR;
float3 frustumRayBL;
float2 windowSize = float2(640, 480);//ln_Resolution.xy;




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



static float ssPos=500;
static float ssRad=500;

struct LN_VSInput
{
	float3	Pos		: POSITION;		// 位置
};

struct VSOutput
{
	float4	Pos			: POSITION;
	float4	ScreenPos	: TEXCOORD1;
	float4 tmpUV: TEXCOORD12;
};

struct PSInput
{
	float4	ScreenPos	: TEXCOORD1;
	float4 tmpUV: TEXCOORD12;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	VSOutput o;
	
	//float4x4 mat = ln_WorldViewProjection;
	
	o.Pos	= mul(float4(v.Pos, 1.0), ln_WorldViewProjection);
	//o.Pos.xy *= -1;
	o.ScreenPos = o.Pos;
	
	
	o.tmpUV	= mul(float4(0, 1, 0, 1), ln_WorldViewProjection);
	o.Pos.z = 0.0;
	return o;
}

//------------------------------------------------------------------------------

float dist_func(float3 pos, float size)
{
	//pos.y -= fCameraHeight*2.1;
	pos.y -= ssPos;
    return length(pos) - size;
}

float3 getNormal(float3 pos, float size)
{
	//float3 pos2 = pos;
	//pos.y -= fCameraHeight*2.1;
    float ep = 1.0001;
    return normalize(float3(
            dist_func(pos, size) - dist_func(float3(pos.x - ep, pos.y, pos.z), size),
            dist_func(pos, size) - dist_func(float3(pos.x, pos.y - ep, pos.z), size),
            dist_func(pos, size) - dist_func(float3(pos.x, pos.y, pos.z - ep), size)
        ));
}
//------------------------------------------------------------------------------


float3 IntersectionPos(float3 rayPos, float3 rayDir, float sphereRadius)
{
	const float A = dot(rayDir, rayDir);
	const float B = 2.0*dot(rayPos, rayDir);
	const float C = dot(rayPos, rayPos) - sphereRadius*sphereRadius;

	return B*B - 4.0*A*C < 0.000001 ? float3(0, 0, 0) : (rayPos + rayDir*(0.5*(-B + sqrt(B*B - 4.0*A*C)) / A));
}

float IntegralApproximation(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

float3 easeInOutExpo(float t, float3 b, float3 c, float d)
{
	t /= d / 2;
	return c / 2 * pow(2, 10 * (t - 1)) + b;
}
float bias(float b, float x)
{
  return pow(x, log(b) / log(0.5));
}

float4 PSBasic(PSInput input) : COLOR0
{
	float2 uv = ((input.ScreenPos.xy) + 1.0) / 2;// / float2(windowSize));
	uv.y = 1.0 - uv.y;
	uv.y *= input.tmpUV.y;

	float3 deltaX = (frustumRayTR - frustumRayTL)*uv.x;
	float3 deltaY = (frustumRayBL - frustumRayTL)*uv.y;
	float3 ray = normalize((frustumRayTL + deltaX + deltaY).xyz);
	
	
	float3 cameraPos = v3CameraPos.xyz;
	//cameraPos.y *= -1;
	//float3 cameraPos = mul(float4(v3CameraPos, 1.0), _refrect).xyz;
	
	float3 lightPos = v3LightPos;
	//lightPos.y *= -1;
	//float3 lightPos = mul(float4(v3LightPos, 1.0), _refrect).xyz;
	

	float3 skyPos = IntersectionPos(cameraPos, ray, fOuterRadius);
	if (skyPos.x == 0)
	{
		//Camera is out of celestial sphere.
		clip(-1);
	}
	//skyPos.y *= -1;
	//return float4(skyPos.xyz, 1);
	//return float4(uv.xy, 0, 1);
	
	//skyPos.y *= -1;

	float3 invWavelength = v3InvWavelength;

	float3 v3Pos = skyPos;
	float3 v3Ray = v3Pos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = cameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*IntegralApproximation(fStartAngle);

	const int nSamples = 5;
	const float fSamples = nSamples;

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for (int i = 0; i<nSamples; ++i)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(lightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(IntegralApproximation(fLightAngle) - IntegralApproximation(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (invWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
	//return float4(v3FrontColor.xyz, 1);

	float4 primaryColor = float4(v3FrontColor * (invWavelength * fKrESun), 1.0);
	
	float4 secondaryColor = float4(v3FrontColor * fKmESun, 1.0);
	float3 v3Direction = cameraPos - v3Pos;

	float fCos = dot(lightPos, v3Direction) / length(v3Direction);
	float fRayPhase = 0.75 * (1.0 + fCos * fCos);

	const float g2 = g*g;
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
	
	float3 raycolor = (primaryColor*fRayPhase).xyz;
	float3 miecolor = (secondaryColor*fMiePhase).xyz;

	float3 c = float3(1.0, 1.0, 1.0) - exp(-exposure * (raycolor + miecolor));
	
	c *= c;
	c *= c;
	
#if 0
	{
		float3 cameraPos = float3(0, 0, 10);
		float3 cur = cameraPos;
		//float3 ray = normalize(v3Ray);

	    float size = ssRad;//fCameraHeight;
	    for (int i = 0; i < 16; i++)
	    {
	        float d = dist_func(cur, size);
	        if (d < 0.0001)
	        {
	            float3 normal = getNormal(cur, size);
	        	
	        	
	        	float3 toCamera = normalize(cameraPos - (normal * ssRad + float3(0, ssPos, 0)));
	        	
	        	// equal 1.0 - 0.0 back
	        	float d = abs(dot(normal, toCamera));// + 1.0f) / 2;
	    		d = 1.0 - d;
	        	d = bias(0.2, d);
	        	//
	        	
	        	c = lerp (c, c + float3(0.21, 0.21, 0.21), d);
	        	
	        	//c = easeInOutExpo(d, c, float3(0.21, 0.21, 0.21), 1.0);
	        	
	        	
	        	//c = float3(d, d, d);
	            //float diff = dot(normal, float3(1, 0, 0));
	            //c = float3(diff, diff, diff);// + float3(0.1);
	            break;
	        }
	        cur += ray * d;
	    }
	}
#endif
	
    
    
    
	return float4(saturate(c), 1.0);
}

//------------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSBasic();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}
