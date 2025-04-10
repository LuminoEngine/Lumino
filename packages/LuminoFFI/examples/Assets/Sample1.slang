import lumino;

public struct VSOutput {
    public float4 position: SV_POSITION;
    public float2 uv: TEXCOORD0;
};

[shader("vertex")]
VSOutput vsMain(lumino.VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), ln_WorldViewProjection);
    output.uv = input.uv;
    return output;
}

static const float time = 0.0;

[shader("fragment")]
float4 fsMain(VSOutput input): SV_TARGET {
	float2 pos = input.uv;//( gl_FragCoord.xy / resolution.xy );
	float three = 1.0 - ((pos.x + pos.y) / 2.0);
	float3 color = float3(three, pos.x, pos.y);
	float3 color2 = color;
	
	pos = (pos) * 2.0 - 1.0;
	color *= abs(1.0 / (sin(pos.y + sin(pos.x + time) * 0.21) * 30.0));
	color2 *= abs(1.0 / (sin(pos.y + cos(pos.x + time) * 0.7) * 30.0));
	color += color2;
	color /= 1.0;


    return float4(color, 1.0);
}



// uniform float time;
// uniform vec2 mouse;
// uniform vec2 resolution;


#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif
