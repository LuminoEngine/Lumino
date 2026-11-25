// LNMaterial_SetMatrix で渡した float4x4 の並びを検証するテスト用シェーダ。
//
// u_mode.x で出力を切り替える。色を出すモードでは、値 v を v * 0.25 + 0.5 に写して
// ([-2, 2] -> [0, 1]) RGB に出すので、読み戻した画素から元の値を復元できる。
// A チャンネルはバックバッファによっては読み戻せないため使わない。u_mode.y が 1 のときは
// v.wxyz を出し、w を R で読めるようにする。
//   0: 頂点を mul(u_matrix, float4(position, 1)) で変換し、白を出す
//   1: mul(u_matrix, u_point) を色で出す (頂点は NDC のまま)
//   2: u_matrix[1] (数学の行 1) を色で出す
//   3: 要素 (u_matrix[1][3], u_matrix[0][1], u_matrix[2][0], u_matrix[3][2]) を色で出す
import lumino;

// 先頭の float4 は Material 生成時に白で初期化される。行列が先頭以外のオフセットでも
// 名前で引けることを確かめるため、行列より前に置く。
uniform float4   u_mode;
uniform float4x4 u_matrix;
uniform float4   u_point;

struct VSOutput {
    float4 position : SV_POSITION;
};

float4 encode(float4 v) {
    return v * 0.25 + 0.5;
}

[shader("vertex")]
VSOutput vsMain(VSInput input) {
    VSOutput output;
    if (u_mode.x < 0.5) {
        output.position = mul(u_matrix, float4(input.position, 1.0));
    } else {
        output.position = float4(input.position, 1.0);
    }
    return output;
}

[shader("fragment")]
float4 fsMain(VSOutput input) : SV_TARGET {
    int mode = int(u_mode.x + 0.5);
    float4 v;
    if (mode == 1) {
        v = mul(u_matrix, u_point);
    } else if (mode == 2) {
        v = u_matrix[1];
    } else if (mode == 3) {
        v = float4(u_matrix[1][3], u_matrix[0][1], u_matrix[2][0], u_matrix[3][2]);
    } else {
        return float4(1.0, 1.0, 1.0, 1.0);
    }
    if (u_mode.y > 0.5) v = v.wxyz;
    return float4(encode(v).rgb, 1.0);
}

#ifdef LUMINO_METADATA
{
    "passes": [
        {
            "name": "Forward",
            "vertex": "vsMain",
            "fragment": "fsMain"
        }
    ]
}
#endif
