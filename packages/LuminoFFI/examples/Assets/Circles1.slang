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

uniform float u_Time;

float time() {
    return u_Time * 0.5;
}

static const float2 resolution = float2(800, 600); // Set the resolution to a default value

float2 center (float2 border, float2 offset, float2 velocity, float timeOffset) {
    /*
    float2 c = offset + velocity * u_Time;
    c = fmod (c, 2. - 4. * border);
    if (c.x > 1. - border.x) c.x = 2. - c.x - 2. * border.x;
    if (c.x < border.x) c.x = 2. * border.x - c.x;
    if (c.y > 1. - border.y) c.y = 2. - c.y - 2. * border.y;
    if (c.y < border.y) c.y = 2. * border.y - c.y;
    */

    float time = time() + timeOffset;
    float2 c = offset;

    c.x += cos(time * velocity.x) * 0.25 + 0.25;
    c.y += sin(time * velocity.y) * 0.25 + 0.25;

    return c;
}

float3 circle(float2 pos, float r, float3 col, float2 offset, float2 velocity, float timeOffset) {
    float aspect = resolution.x / resolution.y;
    float2 c = center(float2(r / aspect, r), offset, velocity, timeOffset);
    c.x *= aspect;
    float d = distance(pos, c);

    float intensity = 0.0;
    if (d < r) {
        // Inner
        intensity = 0.5;
    }
    else {
        // Outer blur
        float blurDiff = d - r;
        intensity = max(0.5 - min(pow(blurDiff, 0.3), 0.6), -0.1);
    }

    return col * intensity;
}

[shader("fragment")]
float4 fsMain(VSOutput input): SV_TARGET {
    float2 pos = input.uv;//gl_FragCoord.xy / resolution.y;
    float3 color = float3(1.0, 1.0, 1.0);
    color -= circle(pos, 0.03, float3(0.7, 0.2, 0.8), float2(0.6), float2(0.30, 0.70), 0.0);
    color -= circle(pos, 0.05, float3(0.5, 0.3, 0.4), float2(0.1), float2(0.02, 0.20), 1.1);
    color -= circle(pos, 0.07, float3(0.3, 0.4, 0.1), float2(0.1), float2(0.10, 0.04), 2.2);
    color -= circle(pos, 0.10, float3(0.2, 0.5, 0.1), float2(0.3), float2(0.10, 0.20), 3.3);
    color -= circle(pos, 0.20, float3(0.1, 0.3, 0.7), float2(0.2), float2(0.40, 0.25), 4.4);
    color -= circle(pos, 0.30, float3(0.9, 0.4, 0.2), float2(0.0), float2(0.15, 0.20), 5.5);
    return float4(color, 1.0);
}

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
