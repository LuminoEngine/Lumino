// lumino.slang — Lumino standard shader library
//
// Common structures shared across Lumino shaders.
// Usage: import lumino;

struct ViewParams {
    float4x4 viewProj;
    float4   cameraPos;    // xyz = position
    float4x4 view;         // view matrix (world -> view)
    float4x4 proj;         // projection matrix (view -> clip)
    float4x4 invViewProj;  // inverse of viewProj (clip -> world)
    float4x4 invProj;      // inverse of proj (clip -> view)
    float4   screenSize;   // (width, height, 1/width, 1/height)
};

struct SceneParams {
    float4 lightDir;       // xyz = direction (towards light)
    float4 lightColor;     // xyz = color, w = intensity
    float4 ambientColor;   // xyz = ambient color
};

struct ObjectParams {
    float4x4 world;
    float4x4 normalMatrix;
};

struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
    float4 tangent  : TANGENT;
};
