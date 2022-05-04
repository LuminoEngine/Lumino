#include "TeapotMeshGenerater.hpp"

namespace ln {
namespace detail {


//--------------------------------------------------------------------------------------
// File: TeapotData.inc
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------


// The teapot model consists of 10 bezier patches. Each patch has 16 control
// points, plus a flag indicating whether it should be mirrored in the Z axis
// as well as in X (all of the teapot is symmetrical from left to right, but
// only some parts are symmetrical from front to back). The control points
// are stored as integer indices into the TeapotControlPoints array.

struct TeapotPatch
{
    bool mirrorZ;
    int indices[16];
};


// Static data array defines the bezier patches that make up the teapot.
const TeapotPatch TeapotPatches[] =
{
    // Rim.
    { true, { 102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } },

    // Body.
    { true, { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 } },
    { true, { 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 } },

    // Lid.
    { true, { 96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101, 101, 0, 1, 2, 3 } },
    { true, { 0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117 } },

    // Handle.
    { false, { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 } },
    { false, { 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67 } },

    // Spout.
    { false, { 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83 } },
    { false, { 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95 } },

    // Bottom.
    { true, { 118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37 } },
};


// Static array defines the control point positions that make up the teapot.
const Vector3 TeapotControlPoints[] =
{
    { 0, 0.345f, -0.05f },
    { -0.028f, 0.345f, -0.05f },
    { -0.05f, 0.345f, -0.028f },
    { -0.05f, 0.345f, -0 },
    { 0, 0.3028125f, -0.334375f },
    { -0.18725f, 0.3028125f, -0.334375f },
    { -0.334375f, 0.3028125f, -0.18725f },
    { -0.334375f, 0.3028125f, -0 },
    { 0, 0.3028125f, -0.359375f },
    { -0.20125f, 0.3028125f, -0.359375f },
    { -0.359375f, 0.3028125f, -0.20125f },
    { -0.359375f, 0.3028125f, -0 },
    { 0, 0.27f, -0.375f },
    { -0.21f, 0.27f, -0.375f },
    { -0.375f, 0.27f, -0.21f },
    { -0.375f, 0.27f, -0 },
    { 0, 0.13875f, -0.4375f },
    { -0.245f, 0.13875f, -0.4375f },
    { -0.4375f, 0.13875f, -0.245f },
    { -0.4375f, 0.13875f, -0 },
    { 0, 0.007499993f, -0.5f },
    { -0.28f, 0.007499993f, -0.5f },
    { -0.5f, 0.007499993f, -0.28f },
    { -0.5f, 0.007499993f, -0 },
    { 0, -0.105f, -0.5f },
    { -0.28f, -0.105f, -0.5f },
    { -0.5f, -0.105f, -0.28f },
    { -0.5f, -0.105f, -0 },
    { 0, -0.105f, 0.5f },
    { 0, -0.2175f, -0.5f },
    { -0.28f, -0.2175f, -0.5f },
    { -0.5f, -0.2175f, -0.28f },
    { -0.5f, -0.2175f, -0 },
    { 0, -0.27375f, -0.375f },
    { -0.21f, -0.27375f, -0.375f },
    { -0.375f, -0.27375f, -0.21f },
    { -0.375f, -0.27375f, -0 },
    { 0, -0.2925f, -0.375f },
    { -0.21f, -0.2925f, -0.375f },
    { -0.375f, -0.2925f, -0.21f },
    { -0.375f, -0.2925f, -0 },
    { 0, 0.17625f, 0.4f },
    { -0.075f, 0.17625f, 0.4f },
    { -0.075f, 0.2325f, 0.375f },
    { 0, 0.2325f, 0.375f },
    { 0, 0.17625f, 0.575f },
    { -0.075f, 0.17625f, 0.575f },
    { -0.075f, 0.2325f, 0.625f },
    { 0, 0.2325f, 0.625f },
    { 0, 0.17625f, 0.675f },
    { -0.075f, 0.17625f, 0.675f },
    { -0.075f, 0.2325f, 0.75f },
    { 0, 0.2325f, 0.75f },
    { 0, 0.12f, 0.675f },
    { -0.075f, 0.12f, 0.675f },
    { -0.075f, 0.12f, 0.75f },
    { 0, 0.12f, 0.75f },
    { 0, 0.06375f, 0.675f },
    { -0.075f, 0.06375f, 0.675f },
    { -0.075f, 0.007499993f, 0.75f },
    { 0, 0.007499993f, 0.75f },
    { 0, -0.04875001f, 0.625f },
    { -0.075f, -0.04875001f, 0.625f },
    { -0.075f, -0.09562501f, 0.6625f },
    { 0, -0.09562501f, 0.6625f },
    { -0.075f, -0.105f, 0.5f },
    { -0.075f, -0.18f, 0.475f },
    { 0, -0.18f, 0.475f },
    { 0, 0.02624997f, -0.425f },
    { -0.165f, 0.02624997f, -0.425f },
    { -0.165f, -0.18f, -0.425f },
    { 0, -0.18f, -0.425f },
    { 0, 0.02624997f, -0.65f },
    { -0.165f, 0.02624997f, -0.65f },
    { -0.165f, -0.12375f, -0.775f },
    { 0, -0.12375f, -0.775f },
    { 0, 0.195f, -0.575f },
    { -0.0625f, 0.195f, -0.575f },
    { -0.0625f, 0.17625f, -0.6f },
    { 0, 0.17625f, -0.6f },
    { 0, 0.27f, -0.675f },
    { -0.0625f, 0.27f, -0.675f },
    { -0.0625f, 0.27f, -0.825f },
    { 0, 0.27f, -0.825f },
    { 0, 0.28875f, -0.7f },
    { -0.0625f, 0.28875f, -0.7f },
    { -0.0625f, 0.2934375f, -0.88125f },
    { 0, 0.2934375f, -0.88125f },
    { 0, 0.28875f, -0.725f },
    { -0.0375f, 0.28875f, -0.725f },
    { -0.0375f, 0.298125f, -0.8625f },
    { 0, 0.298125f, -0.8625f },
    { 0, 0.27f, -0.7f },
    { -0.0375f, 0.27f, -0.7f },
    { -0.0375f, 0.27f, -0.8f },
    { 0, 0.27f, -0.8f },
    { 0, 0.4575f, -0 },
    { 0, 0.4575f, -0.2f },
    { -0.1125f, 0.4575f, -0.2f },
    { -0.2f, 0.4575f, -0.1125f },
    { -0.2f, 0.4575f, -0 },
    { 0, 0.3825f, -0 },
    { 0, 0.27f, -0.35f },
    { -0.196f, 0.27f, -0.35f },
    { -0.35f, 0.27f, -0.196f },
    { -0.35f, 0.27f, -0 },
    { 0, 0.3075f, -0.1f },
    { -0.056f, 0.3075f, -0.1f },
    { -0.1f, 0.3075f, -0.056f },
    { -0.1f, 0.3075f, -0 },
    { 0, 0.3075f, -0.325f },
    { -0.182f, 0.3075f, -0.325f },
    { -0.325f, 0.3075f, -0.182f },
    { -0.325f, 0.3075f, -0 },
    { 0, 0.27f, -0.325f },
    { -0.182f, 0.27f, -0.325f },
    { -0.325f, 0.27f, -0.182f },
    { -0.325f, 0.27f, -0 },
    { 0, -0.33f, -0 },
    { -0.1995f, -0.33f, -0.35625f },
    { 0, -0.31125f, -0.375f },
    { 0, -0.33f, -0.35625f },
    { -0.35625f, -0.33f, -0.1995f },
    { -0.375f, -0.31125f, -0 },
    { -0.35625f, -0.33f, -0 },
    { -0.21f, -0.31125f, -0.375f },
    { -0.375f, -0.31125f, -0.21f },
};

//==============================================================================
// DirectXTK
//==============================================================================

static const Vector3 g_XMEpsilon(1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f);
static const Vector3 g_XMNegateX(-1.0f, 1.0f, 1.0f);
static const Vector3 g_XMNegateY(1.0f, -1.0f, 1.0f);
static const Vector3 g_XMNegateZ(1.0f, 1.0f, -1.0f);
static const Vector3 g_XMIdentityR1(0.0f, 1.0f, 0.0f);
static const Vector3 g_XMNegIdentityR1(0.0f, -1.0f, 0.0f);

namespace Bezier
{
    inline Vector3 XMVectorLess
    (
        Vector3 V1,
        Vector3 V2
    )
    {
        Vector3 Control;
        Control.x = (V1.x < V2.x) ? 1.0f : 0.0f;
        Control.y = (V1.y < V2.y) ? 1.0f : 0.0f;
        Control.z = (V1.z < V2.z) ? 1.0f : 0.0f;
        return Control;
    }


    inline Vector3 XMVectorSelect
    (
        Vector3 V1,
        Vector3 V2,
        Vector3 Control
    )
    {
        Vector3 Result;
        Result.x = (Control.x == 0) ? V1.x : V2.x;
        Result.y = (Control.y == 0) ? V1.y : V2.y;
        Result.z = (Control.z == 0) ? V1.z : V2.z;
        return Result;
    }

    template<typename T>
    T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
    {
        return p1 * (1 - t) * (1 - t) * (1 - t) +
            p2 * 3 * t * (1 - t) * (1 - t) +
            p3 * 3 * t * t * (1 - t) +
            p4 * t * t * t;
    }

    template<typename T>
    T CubicTangent(T const& p1, T const& p2, T const& p3, T const& p4, float t)
    {
        return p1 * (-1 + 2 * t - t * t) +
            p2 * (1 - 4 * t + 3 * t * t) +
            p3 * (2 * t - 3 * t * t) +
            p4 * (t * t);
    }

    // Creates vertices for a patch that is tessellated at the specified level.
    // Calls the specified outputVertex function for each generated vertex,
    // passing the position, normal, and texture coordinate as parameters.
    template<typename TOutputFunc>
    void CreatePatchVertices(
        //_In_reads_(16) DirectX::XMVECTOR patch[16],
        const Vector3* patch,
        size_t tessellation,
        bool isMirrored,
        TOutputFunc outputVertex)
    {
        for (size_t i = 0; i <= tessellation; i++)
        {
            float u = (float)i / tessellation;

            for (size_t j = 0; j <= tessellation; j++)
            {
                float v = (float)j / tessellation;

                // Perform four horizontal bezier interpolations
                // between the control points of this patch.
                Vector3 p1 = CubicInterpolate(patch[0], patch[1], patch[2], patch[3], u);
                Vector3 p2 = CubicInterpolate(patch[4], patch[5], patch[6], patch[7], u);
                Vector3 p3 = CubicInterpolate(patch[8], patch[9], patch[10], patch[11], u);
                Vector3 p4 = CubicInterpolate(patch[12], patch[13], patch[14], patch[15], u);

                // Perform a vertical interpolation between the results of the
                // previous horizontal interpolations, to compute the position.
                Vector3 position = CubicInterpolate(p1, p2, p3, p4, v);

                // Perform another four bezier interpolations between the control
                // points, but this time vertically rather than horizontally.
                Vector3 q1 = CubicInterpolate(patch[0], patch[4], patch[8], patch[12], v);
                Vector3 q2 = CubicInterpolate(patch[1], patch[5], patch[9], patch[13], v);
                Vector3 q3 = CubicInterpolate(patch[2], patch[6], patch[10], patch[14], v);
                Vector3 q4 = CubicInterpolate(patch[3], patch[7], patch[11], patch[15], v);

                // Compute vertical and horizontal tangent vectors.
                Vector3 tangent1 = CubicTangent(p1, p2, p3, p4, v);
                Vector3 tangent2 = CubicTangent(q1, q2, q3, q4, u);

                // cross the two tangent vectors to compute the normal.
                Vector3 normal = Vector3::cross(tangent1, tangent2);

                if (!Vector3::nearEqual(normal, Vector3::Zero/*, g_XMEpsilon*/))
                {
                    normal = Vector3::normalize(normal);

                    // If this patch is mirrored, we must invert the normal.
                    if (isMirrored)
                    {
                        normal = -normal;
                    }
                }
                else
                {
                    // In a tidy and well constructed bezier patch, the preceding
                    // normal computation will always work. But the classic teapot
                    // model is not tidy or well constructed! At the top and bottom
                    // of the teapot, it contains degenerate geometry where a patch
                    // has several control points in the same place, which causes
                    // the tangent computation to fail and produce a zero normal.
                    // We 'fix' these cases by just hard-coding a normal that points
                    // either straight up or straight down, depending on whether we
                    // are on the top or bottom of the teapot. This is not a robust
                    // solution for all possible degenerate bezier patches, but hey,
                    // it's good enough to make the teapot work correctly!

                    normal = XMVectorSelect(g_XMIdentityR1, g_XMNegIdentityR1, XMVectorLess(position, Vector3::Zero));
                }

                // Compute the texture coordinate.
                float mirroredU = isMirrored ? 1 - u : u;

                Vector2 textureCoordinate(mirroredU, v);

                // Output this vertex.
                outputVertex(position, normal, textureCoordinate);
            }
        }
    }


    // Creates indices for a patch that is tessellated at the specified level.
    // Calls the specified outputIndex function for each generated index value.
    template<typename TOutputFunc>
    void CreatePatchIndices(size_t tessellation, bool isMirrored, TOutputFunc outputIndex)
    {
        size_t stride = tessellation + 1;

        for (size_t i = 0; i < tessellation; i++)
        {
            for (size_t j = 0; j < tessellation; j++)
            {
                // Make a list of six index values (two triangles).
                std::array<size_t, 6> indices =
                {
#ifdef LN_COORD_RH
                    i* stride + j,
                    (i + 1)* stride + j + 1,
                    (i + 1)* stride + j,

                    i* stride + j,
                    i* stride + j + 1,
                    (i + 1)* stride + j + 1,
#else
                    i * stride + j,
                    (i + 1) * stride + j,
                    (i + 1) * stride + j + 1,

                    i * stride + j,
                    (i + 1) * stride + j + 1,
                    i * stride + j + 1,
#endif
                };

                // If this patch is mirrored, reverse indices to fix the winding order.
                if (isMirrored)
                {
                    std::reverse(indices.begin(), indices.end());
                }

                // Output these index values.
                std::for_each(indices.begin(), indices.end(), outputIndex);
            }
        }
    }

} // namespace Bezier

//==============================================================================
// TeapotMeshGenerater

int TeapotMeshGenerater::vertexCount() const
{
    const int tess = m_tessellation + 1;
    return getPatchBaseCalls() * (tess * tess);
}

int TeapotMeshGenerater::indexCount() const
{
    return getPatchBaseCalls() * (m_tessellation * m_tessellation) * 6;
}

void TeapotMeshGenerater::onGenerate(MeshGeneraterBuffer* buf)
{
    m_buffer = buf;
    m_vbPos = 0;
    m_ibPos = 0;
    computeTeapot(m_size, m_tessellation, false);
}

int TeapotMeshGenerater::getPatchBaseCalls()
{
	int baseCall = 0;

	for (int i = 0; i < sizeof(TeapotPatches) / sizeof(TeapotPatches[0]); i++)
	{
		const TeapotPatch& patch = TeapotPatches[i];
		baseCall += 2;
		if (patch.mirrorZ)
		{
			baseCall += 2;
		}
	}

	return baseCall;
}

void TeapotMeshGenerater::computeTeapot(float size, size_t tessellation, bool rhcoords)
{
    if (tessellation < 1)
        throw std::out_of_range("tesselation parameter out of range");

    Vector3 scaleVector = Vector3::replicate(size);

    Vector3 scaleNegateX = scaleVector * g_XMNegateX;
    Vector3 scaleNegateZ = scaleVector * g_XMNegateZ;
    Vector3 scaleNegateXZ = scaleVector * g_XMNegateX * g_XMNegateZ;

    for (int i = 0; i < sizeof(TeapotPatches) / sizeof(TeapotPatches[0]); i++)
    {
        const TeapotPatch& patch = TeapotPatches[i];

        // Because the teapot is symmetrical from left to right, we only store
        // data for one side, then tessellate each patch twice, mirroring in X.
        // ティーポットは左から右に対称であるため、片側のみのデータを保存し、各パッチを2回テッセレーションしてXでミラーリングします。
        tessellatePatch(patch, tessellation, scaleVector, false);
        tessellatePatch(patch, tessellation, scaleNegateX, true);

        if (patch.mirrorZ)
        {
            // Some parts of the teapot (the body, lid, and rim, but not the
            // handle or spout) are also symmetrical from front to back, so
            // we tessellate them four times, mirroring in Z as well as X.
            // ティーポットの一部（ボディ、フタ、リム、しかしハンドルやスパウトではありません）も前から後に対称ですので、それらを4回テッセレーションし、ZとXをミラーリングします。
            tessellatePatch(patch, tessellation, scaleNegateZ, true);
            tessellatePatch(patch, tessellation, scaleNegateXZ, false);
        }
    }

    // Built RH above
    //if (!rhcoords)
    //	ReverseWinding(indices, vertices);
}

void TeapotMeshGenerater::tessellatePatch(const TeapotPatch& patch, size_t tessellation, const Vector3& scale, bool isMirrored)
{
    // Look up the 16 control points for this patch.
    Vector3 controlPoints[16];

    for (int i = 0; i < 16; i++)
    {
        controlPoints[i] = TeapotControlPoints[patch.indices[i]] * scale;
    }

    // Create the index data.
    Bezier::CreatePatchIndices(tessellation, isMirrored, [&](size_t index)
    {
        addIndex(m_vbPos + (uint16_t)index);
    });

    // Create the vertex data.
    Bezier::CreatePatchVertices(controlPoints, tessellation, isMirrored, [&](
        const Vector3& position,
        const Vector3& normal,
        const Vector2& textureCoordinate)
    {
        addVertex(position, normal, textureCoordinate);
    });
}

void TeapotMeshGenerater::addVertex(const Vector3& pos, const Vector3& normal, const Vector2& texUV)
{
    m_buffer->setV(m_vbPos, pos, texUV, normal);
    m_vbPos++;
}


void TeapotMeshGenerater::addIndex(uint32_t index)
{
    m_buffer->setI(m_ibPos, index);
    m_ibPos++;
}

} // namespace detail
} // namespace ln

