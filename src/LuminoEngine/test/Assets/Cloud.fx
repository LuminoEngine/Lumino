// 1 / 289
#define NOISE_SIMPLEX_1_DIV_289 0.00346020761245674740484429065744f

float mod289(float x) {
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

float2 mod289(float2 x) {
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

float3 mod289(float3 x) {
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

float4 mod289(float4 x) {
	return x - floor(x * NOISE_SIMPLEX_1_DIV_289) * 289.0;
}

float4 taylorInvSqrt(float4 r) {
	return 1.79284291400159 - 0.85373472095314 * r;
}

// ( x*34.0 + 1.0 )*x = 
// x*x*34.0 + x
float permute(float x) {
	return mod289(
		x*x*34.0 + x
	);
}

float3 permute(float3 x) {
	return mod289(
		x*x*34.0 + x
	);
}

float4 permute(float4 x) {
	return mod289(
		x*x*34.0 + x
	);
}


float snoise(float3 v)
{
	const float2 C = float2(
		0.166666666666666667, // 1/6
		0.333333333333333333  // 1/3
	);
	const float4 D = float4(0.0, 0.5, 1.0, 2.0);
	
// First corner
	float3 i = floor( v + dot(v, C.yyy) );
	float3 x0 = v - i + dot(i, C.xxx);
	
// Other corners
	float3 g = step(x0.yzx, x0.xyz);
	float3 l = 1 - g;
	float3 i1 = min(g.xyz, l.zxy);
	float3 i2 = max(g.xyz, l.zxy);
	
	float3 x1 = x0 - i1 + C.xxx;
	float3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	float3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y
	
// Permutations
	i = mod289(i);
	float4 p = permute(
		permute(
			permute(
					i.z + float4(0.0, i1.z, i2.z, 1.0 )
			) + i.y + float4(0.0, i1.y, i2.y, 1.0 )
		) 	+ i.x + float4(0.0, i1.x, i2.x, 1.0 )
	);
	
// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1/7
	float3 ns = n_ * D.wyz - D.xzx;
	
	float4 j = p - 49.0 * floor(p * ns.z * ns.z); // mod(p,7*7)
	
	float4 x_ = floor(j * ns.z);
	float4 y_ = floor(j - 7.0 * x_ ); // mod(j,N)
	
	float4 x = x_ *ns.x + ns.yyyy;
	float4 y = y_ *ns.x + ns.yyyy;
	float4 h = 1.0 - abs(x) - abs(y);
	
	float4 b0 = float4( x.xy, y.xy );
	float4 b1 = float4( x.zw, y.zw );
	
	//float4 s0 = float4(lessThan(b0,0.0))*2.0 - 1.0;
	//float4 s1 = float4(lessThan(b1,0.0))*2.0 - 1.0;
	float4 s0 = floor(b0)*2.0 + 1.0;
	float4 s1 = floor(b1)*2.0 + 1.0;
	float4 sh = -step(h, 0.0);
	
	float4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	float4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
	
	float3 p0 = float3(a0.xy,h.x);
	float3 p1 = float3(a0.zw,h.y);
	float3 p2 = float3(a1.xy,h.z);
	float3 p3 = float3(a1.zw,h.w);
	
//Normalise gradients
	float4 norm = taylorInvSqrt(float4(
		dot(p0, p0),
		dot(p1, p1),
		dot(p2, p2),
		dot(p3, p3)
	));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;
	
// Mix final noise value
	float4 m = max(
		0.6 - float4(
			dot(x0, x0),
			dot(x1, x1),
			dot(x2, x2),
			dot(x3, x3)
		),
		0.0
	);
	m = m * m;
	return 42.0 * dot(
		m*m,
		float4(
			dot(p0, x0),
			dot(p1, x1),
			dot(p2, x2),
			dot(p3, x3)
		)
	);
}


//---------------------------------------------------------------------------------------


float4x4 _VPInv;
float _VolumeUpper;
float _VolumeLower;
float3 _CameraPos;

struct appdata_t
{
	float4 vertex : POSITION;
};

struct Varying
{
	float4	pos				: SV_POSITION;
    
    float3  ray : TEXCOORD0;
    float3  worldPos : TEXCOORD1;   // Flustum ner のピクセルのワールド空間上の位置
};

bool IntersectRayPlane(float3 rayOrigin, float3 rayDirection, float3 posOnPlane, float3 planeNormal, out float3 intersectionPoint)
{
  float rDotn = dot(rayDirection, planeNormal);

  //parallel to plane or pointing away from plane?
  if (rDotn > 0.0000001 )
    return false;
 
  float s = dot(planeNormal, (posOnPlane - rayOrigin)) / rDotn;
	
  intersectionPoint = rayOrigin + s * rayDirection;

  return true;
}

float _LN_CalcFogFactor(float depth)
{
	//float f = ln_FogParams.a * depth;
    float f = 0.05 * depth;
	return exp2(-f);
	//return exp2(-f*f);
}

float rangeExp(float height)
{
    float d =  abs(height - lerp(_VolumeUpper, _VolumeLower, 0.5));
    float f = 0.5 * d;
	return exp2(-f);// * exp2(f);
}
float nearPosExp(float depth)
{
    float f = 0.05 * (-depth);
	return exp2(-f);
}

Varying vsMain(appdata_t v)
{
	Varying output;
	//UNITY_SETUP_INSTANCE_ID(v);
	//UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(OUT);
	output.pos = v.vertex;//UnityObjectToClipPos(v.vertex);


	//float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0); 	// The camera's current position

    output.ray = normalize(mul(float4(v.vertex.xyz, 1), _VPInv).xyz);

    float4 sp = mul(float4(v.vertex.xy, 0, 1), _VPInv);
    output.worldPos = sp.xyz / sp.w;

    return output;
}


float4 psMain(Varying input) : SV_Target
{
    //float4 result = float4(1, 0, 0, 0);
    float3 samplePoint;
    float density = 0;

    //return float4(input.worldPos, 1);

    if (input.worldPos.y < _VolumeLower)
    {
        // 視点が低層雲の下端より下にある
    
        float3 pos;
        if (IntersectRayPlane(input.worldPos, input.ray, float3(0, _VolumeLower, 0), float3(0, -1, 0), pos))
        {
            samplePoint = pos;
        }
        else {
            //return float4(1, 0, 0, 1);
            clip(-1);
        }

        //return float4(0, 0, 1, 1);
    }
    else if (input.worldPos.y > _VolumeUpper)
    {
        // 視点が低層雲の下端より下にある
    
        float3 pos;
        if (IntersectRayPlane(input.worldPos, input.ray, float3(0, _VolumeUpper, 0), float3(0, 1, 0), pos))
        {
            samplePoint = pos;
        }
        else {
            //return float4(1, 0, 0, 1);
            clip(-1);
        }

        //return float4(0, 0, 1, 1);
    }
    else
    {
        // 低層雲の領域の中にいる
        samplePoint = input.worldPos;
    }

#define LOOP 200
    float sampleLength = 0.5;
    for (int i = 0; i < LOOP; i++)
    {
        samplePoint += input.ray * sampleLength;
        float o1 = snoise(samplePoint * 0.01);// * 1.5;
        float o2 = snoise(samplePoint * 0.1);// * 0.5;   // 細かい粒度
        float cov = (o1 * o2);// / LOOP;
        cov *= rangeExp(samplePoint.y) * nearPosExp(distance(samplePoint, _CameraPos));
        density += cov;


        if (density > 1.0) break;
        if (samplePoint.y < _VolumeLower) break;
        if (samplePoint.y > _VolumeUpper) break;
    }
    density *= 1.25;

    density *= _LN_CalcFogFactor(distance(samplePoint, _CameraPos));

    return float4(density, density, density, 1);
}



technique MainDraw
{
	pass P0
	{
		VertexShader = vsMain;
        PixelShader = psMain;
	}
}
