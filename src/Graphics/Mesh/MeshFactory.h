
#include "../Internal.h"
#include <Lumino/Graphics/Vertex.h>

LN_NAMESPACE_BEGIN
namespace detail {

class PlaneMeshFactory
{
public:
	PlaneMeshFactory(const Vector2& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 4;
	}

	int GetIndexCount() const
	{
		return 6;
	}

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector2 half = m_size / 2;
		outVertices[0].position.Set(-half.x, half.y, 0);
		outVertices[0].normal.Set(0.0f, 0.0f, -1.0f);
		outVertices[0].uv.Set(0.0f, 0.0f);
		outVertices[1].position.Set(-half.x, -half.y, 0);
		outVertices[1].normal.Set(0.0f, 0.0f, -1.0f);
		outVertices[1].uv.Set(0.0f, 1.0f);
		outVertices[2].position.Set(half.x, half.y, 0);
		outVertices[2].normal.Set(0.0f, 0.0f, -1.0f);
		outVertices[2].uv.Set(1.0f, 0.0f);
		outVertices[3].position.Set(half.x, -half.y, 0);
		outVertices[3].normal.Set(0.0f, 0.0f, -1.0f);
		outVertices[3].uv.Set(1.0f, 1.0f);

		outIndices[0] = 0;
		outIndices[1] = 1;
		outIndices[2] = 2;
		outIndices[3] = 2;
		outIndices[4] = 1;
		outIndices[5] = 3;
	}

private:
	Vector2	m_size;
};

class PlaneMeshFactory2
{
public:
	PlaneMeshFactory2(const Vector2& size, const Vector3& front)
		: m_size(size)
		, m_front(front)
	{}

	int GetVertexCount() const { return 4; }
	int GetIndexCount() const { return 6; }

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector2 half = m_size / 2;
		if (m_front == Vector3::UnitY)
		{
			outVertices[0].position.Set(-half.x, 0, half.y);
			outVertices[1].position.Set(-half.x, 0, -half.y);
			outVertices[2].position.Set(half.x, 0, half.y);
			outVertices[3].position.Set(half.x, 0, -half.y);
		}
		else if(m_front == -Vector3::UnitY)
		{
			outVertices[0].position.Set(half.x, 0, half.y);
			outVertices[1].position.Set(half.x, 0, -half.y);
			outVertices[2].position.Set(-half.x, 0, half.y);
			outVertices[3].position.Set(-half.x, 0, -half.y);
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		outVertices[0].normal = m_front;
		outVertices[0].uv.Set(0.0f, 0.0f);
		outVertices[1].normal = m_front;
		outVertices[1].uv.Set(0.0f, 1.0f);
		outVertices[2].normal = m_front;
		outVertices[2].uv.Set(1.0f, 0.0f);
		outVertices[3].normal = m_front;
		outVertices[3].uv.Set(1.0f, 1.0f);

		outIndices[0] = 0;
		outIndices[1] = 1;
		outIndices[2] = 2;
		outIndices[3] = 2;
		outIndices[4] = 1;
		outIndices[5] = 3;
	}

private:
	Vector2	m_size;
	Vector3	m_front;
};


class PlaneMeshFactory3
{
public:
	PlaneMeshFactory3(const Vector2& size, int sliceX, int sliceZ)
		: m_size(size)
		, m_sliceX(sliceX)
		, m_sliceZ(sliceZ)
	{
		LN_CHECK_ARG(sliceX >= 1);
		LN_CHECK_ARG(sliceZ >= 1);
	}

	int GetVertexCount() const { return (m_sliceX + 1) * (m_sliceZ + 1); }
	int GetIndexCount() const { return (m_sliceX * m_sliceZ * 2) * 3; }

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector2 minPos = -m_size / 2;
		Vector2 maxPos = m_size / 2;

		float minX = -m_size.x / 2;
		float maxX = m_size.x / 2;
		float minZ = -m_size.y / 2;
		float maxZ = m_size.y / 2;
		float stepX = (maxPos.x - minPos.x) / m_sliceX;
		float stepZ = (maxPos.y - minPos.y) / m_sliceZ;
		float StepU = 1.0f / m_sliceX;
		float StepV = 1.0f / m_sliceZ;
		Vertex* v = outVertices;
		uint16_t* i = (uint16_t*)outIndices;

		// vertices
		for (int iZ = 0; iZ < m_sliceZ + 1; ++iZ)
		{
			for (int iX = 0; iX < m_sliceX + 1; ++iX)
			{
				if (iX == m_sliceX)
					v->position.x = maxX;	// 誤差を出したくないため直接設定
				else
					v->position.x = minX + stepX * iX;

				if (iZ == m_sliceZ)
					v->position.z = minZ;	// 誤差を出したくないため直接設定
				else
					v->position.z = maxZ - stepZ * iZ;

				v->position.y = 0.0f;
				v->normal.Set(0.0f, 1.0f, 0.0f);
				v->uv.Set(StepU * iX, 1.0f - StepV * iZ);
				++v;
			}
		}

		// indices
		for (int iZ = 0; iZ < m_sliceZ; ++iZ)
		{
			for (int iX = 0; iX < m_sliceX; ++iX)
			{
				int p1 = (iX + 0) + (iZ + 0) * (m_sliceX + 1);	// ┏
				int p2 = (iX + 0) + (iZ + 1) * (m_sliceX + 1);	// ┗
				int p3 = (iX + 1) + (iZ + 0) * (m_sliceX + 1);	// ┓
				int p4 = (iX + 1) + (iZ + 1) * (m_sliceX + 1);	// ┛
				i[0] = p1;
				i[1] = p2;
				i[2] = p3;
				i[3] = p3;
				i[4] = p2;
				i[5] = p4;
				i += 6;
			}
		}
	}

private:
	Vector2	m_size;
	int		m_sliceX;
	int		m_sliceZ;
};

class BoxMeshFactory
{
public:
	BoxMeshFactory(const Vector3& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 8;
	}

	int GetIndexCount() const
	{
		return 36;
	}

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);

		// 手前 (Z-)
		outVertices[0].position.Set(minPos.x, maxPos.y, minPos.z);	// 左上
		outVertices[0].uv.Set(0.0f, 0.0f);
		outVertices[1].position.Set(minPos.x, minPos.y, minPos.z);	// 左下
		outVertices[1].uv.Set(0.0f, 1.0f);
		outVertices[2].position.Set(maxPos.x, maxPos.y, minPos.z);	// 右上
		outVertices[2].uv.Set(1.0f, 0.0f);
		outVertices[3].position.Set(maxPos.x, minPos.y, minPos.z);	// 右下
		outVertices[3].uv.Set(1.0f, 1.0f);
		// 奥 (Z+)
		outVertices[4].position.Set(minPos.x, maxPos.y, maxPos.z);	// 左上
		outVertices[4].uv.Set(1.0f, 0.0f);
		outVertices[5].position.Set(minPos.x, minPos.y, maxPos.z);	// 左下
		outVertices[5].uv.Set(1.0f, 1.0f);
		outVertices[6].position.Set(maxPos.x, maxPos.y, maxPos.z);	// 右上
		outVertices[6].uv.Set(0.0f, 0.0f);
		outVertices[7].position.Set(maxPos.x, minPos.y, maxPos.z);	// 右下
		outVertices[7].uv.Set(0.0f, 1.0f);

		for (int i = 0; i < 8; ++i) outVertices[i].color = Color::White;

		uint16_t indices[] =
		{
			// near
			0, 1, 2,
			2, 1, 3,
			// far
			6, 7, 4,
			4, 7, 5,
			// left
			4, 5, 0,
			0, 5, 1,
			// right
			2, 3, 6,
			6, 3, 7,
			// top
			4, 0, 6,
			6, 0, 2,
			// bottom (Z- が面として上方向)
			1, 5, 3,
			3, 5, 7,
		};
		memcpy(outIndices, indices, sizeof(indices));
	}

private:
	Vector3	m_size;
};


// 6面それぞれ独立した頂点を持つ直方体
class RegularBoxMeshFactory
{
public:
	RegularBoxMeshFactory() {}

	RegularBoxMeshFactory(const Vector3& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 24;
	}

	int GetIndexCount() const
	{
		return 36;
	}

	static void SetV(Vertex* vertex, float x, float y, float z, float u, float v, const Vector3& normal)
	{
		vertex->position.Set(x, y, z);
		vertex->uv.Set(u, v);
		vertex->color = Color::White;
		vertex->normal = normal;
	}
	static void SetI(uint16_t* index, uint16_t begin)
	{
		index[0] = begin + 0;
		index[1] = begin + 1;
		index[2] = begin + 2;
		index[3] = begin + 2;
		index[4] = begin + 1;
		index[5] = begin + 3;
	}

	void Generate(Vertex* outVertices, uint16_t* outIndices, uint16_t beginVertexIndex)
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);
		Vertex* v = outVertices;
		uint16_t* i = outIndices;

		// 手前 (Z-)
		SetV(v, minPos.x, maxPos.y, minPos.z, 0.0f, 0.0f, -Vector3::UnitZ); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, minPos.z, 0.0f, 1.0f, -Vector3::UnitZ); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, minPos.z, 1.0f, 0.0f, -Vector3::UnitZ); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, minPos.z, 1.0f, 1.0f, -Vector3::UnitZ); ++v;	// ┛
		SetI(i, beginVertexIndex + 0); i += 6;

		// 奥 (Z+)
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f, Vector3::UnitZ); ++v;	// ┏
		SetV(v, maxPos.x, minPos.y, maxPos.z, 0.0f, 1.0f, Vector3::UnitZ); ++v;	// ┗
		SetV(v, minPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f, Vector3::UnitZ); ++v;	// ┓
		SetV(v, minPos.x, minPos.y, maxPos.z, 1.0f, 1.0f, Vector3::UnitZ); ++v;	// ┛
		SetI(i, beginVertexIndex + 4); i += 6;

		// 左 (X-)
		SetV(v, minPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f, -Vector3::UnitX); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, maxPos.z, 0.0f, 1.0f, -Vector3::UnitX); ++v;	// ┗
		SetV(v, minPos.x, maxPos.y, minPos.z, 1.0f, 0.0f, -Vector3::UnitX); ++v;	// ┓
		SetV(v, minPos.x, minPos.y, minPos.z, 1.0f, 1.0f, -Vector3::UnitX); ++v;	// ┛
		SetI(i, beginVertexIndex + 8); i += 6;

		// 右 (X+)
		SetV(v, maxPos.x, maxPos.y, minPos.z, 0.0f, 0.0f, Vector3::UnitX); ++v;	// ┏
		SetV(v, maxPos.x, minPos.y, minPos.z, 0.0f, 1.0f, Vector3::UnitX); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f, Vector3::UnitX); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, maxPos.z, 1.0f, 1.0f, Vector3::UnitX); ++v;	// ┛
		SetI(i, beginVertexIndex + 12); i += 6;

		// 下 (Y-)(Z- がUVの上方向)
		SetV(v, minPos.x, minPos.y, minPos.z, 0.0f, 0.0f, -Vector3::UnitY); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, maxPos.z, 0.0f, 1.0f, -Vector3::UnitY); ++v;	// ┗
		SetV(v, maxPos.x, minPos.y, minPos.z, 1.0f, 0.0f, -Vector3::UnitY); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, maxPos.z, 1.0f, 1.0f, -Vector3::UnitY); ++v;	// ┛
		SetI(i, beginVertexIndex + 16); i += 6;

		// 上 (Y+)(Z+ がUVの上方向)
		SetV(v, minPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f, Vector3::UnitY); ++v;	// ┏
		SetV(v, minPos.x, maxPos.y, minPos.z, 0.0f, 1.0f, Vector3::UnitY); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f, Vector3::UnitY); ++v;	// ┓
		SetV(v, maxPos.x, maxPos.y, minPos.z, 1.0f, 1.0f, Vector3::UnitY); ++v;	// ┛
		SetI(i, beginVertexIndex + 20);
	}

private:
	Vector3	m_size;
};

// top と bottom は TriangleFan
class SphereMeshFactory
{
public:
	SphereMeshFactory()
		: m_radius(0)
		, m_slices(0)
		, m_stacks(0)
	{
	}
	SphereMeshFactory(float radius, int slices, int stacks)
	{
		Initialize(radius, slices, stacks);
	}

	void Initialize(float radius, int slices, int stacks)
	{
		//if (LN_CHECKEQ_ARG(radius <= 0.0f)) return;
		//if (LN_CHECKEQ_ARG(slices <= 1)) return;
		//if (LN_CHECKEQ_ARG(stacks <= 1)) return;
		m_radius = radius;
		m_slices = slices;
		m_stacks = stacks;
		MakeSinCosTable();
	}

	// Squashed

	int GetVertexCount() const
	{
		return (m_slices + 1) * (m_stacks + 1);
		//return 2 + m_slices * (m_stacks - 1);	// (top と bottom の 2 点) + リングの頂点数 * 重ねる数
	}

	int GetIndexCount() const
	{
		return m_slices * m_stacks * 6;
		//return 2 * m_slices + (m_stacks - 2) * (2 * m_slices);
	}

	//typedef uint16_t Face[3];
	//typedef uint16_t QuadFace[6];

	struct SinCos
	{
		float	sin;
		float	cos;
	};

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vertex* v = outVertices;
		uint16_t* i = (uint16_t*)outIndices;

		float stackUVStep = 1.0f / m_stacks;
		float stackUV = 0.0f;

		float sliceUVStep = 1.0f / m_slices;

		// XY 平面上の step
		float theta_step = Math::PI / m_stacks;
		float theta = 0.0f;//theta_step;

		// Z+ を起点とし、X- 方向へ回っていく

		// rings (Vertex)
		for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
		{
			float sin_theta = sinf(theta);
			float cos_theta = cosf(theta);
			float sliceUV = 0.0f;

			for (int iSlice = 0; iSlice < m_slices + 1; ++iSlice)
			{
				// top
				if (iStack == 0)
				{
					v->normal.x = 0.0f;
					v->normal.y = 1.0f;
					v->normal.z = 0.0f;
				}
				// bottom
				else if (iStack == m_stacks)
				{
					v->normal.x = 0.0f;
					v->normal.y = -1.0f;
					v->normal.z = 0.0f;
				}
				// between
				else
				{
					v->normal.x = sin_theta * m_sincosTable[iSlice].cos;
					v->normal.y = cos_theta;
					v->normal.z = sin_theta * m_sincosTable[iSlice].sin;
				}
				v->position = v->normal * m_radius;
				v->uv.x = sliceUV;
				v->uv.y = stackUV;
				++v;

				sliceUV += sliceUVStep;
			}

			stackUV += stackUVStep;
			theta += theta_step;
		}

		// faces
		for (int iStack = 0; iStack < m_stacks; ++iStack)
		{
			for (int iSlice = 0; iSlice < m_slices; ++iSlice)
			{
				int p1 = (iSlice + 0) + (iStack + 0) * (m_slices + 1);	// ┏
				int p2 = (iSlice + 0) + (iStack + 1) * (m_slices + 1);	// ┗
				int p3 = (iSlice + 1) + (iStack + 0) * (m_slices + 1);	// ┓
				int p4 = (iSlice + 1) + (iStack + 1) * (m_slices + 1);	// ┛
				i[0] = p1;
				i[1] = p2;
				i[2] = p3;
				i[3] = p3;
				i[4] = p2;
				i[5] = p4;
				i += 6;
			}
		}

#if 0
		Face* faces = (Face*)outIndices;

		float theta_step = Math::PI / m_stacks;
		float theta = theta_step;	// XY 平面上の step

		int vertex = 0;
		int face = 0;

		// top
		vertices[vertex].normal.x = 0.0f;
		vertices[vertex].normal.y = 0.0f;
		vertices[vertex].normal.z = 1.0f;
		vertices[vertex].position.x = 0.0f;
		vertices[vertex].position.y = 0.0f;
		vertices[vertex].position.z = m_radius;
		vertex++;

		// top faces (triangle fan)
		for (int iSlice = 0; iSlice < m_slices; ++iSlice)
		{
			faces[face][0] = 0;
			faces[face][1] = iSlice + 1;
			faces[face][2] = iSlice;
			face++;
		}

		// rings
		for (int iStack = 0; iStack < m_stacks - 1; ++iStack)
		{
			float sin_theta = sinf(theta);
			float cos_theta = cosf(theta);

			for (int iSlice = 0; iSlice < m_slices; ++iSlice)
			{
				// vertex
				vertices[vertex].normal.x = sin_theta * m_sincosTable[iSlice].cos;
				vertices[vertex].normal.y = sin_theta * m_sincosTable[iSlice].sin;
				vertices[vertex].normal.z = cos_theta;
				vertices[vertex].position.x = m_radius * sin_theta * m_sincosTable[iSlice].cos;
				vertices[vertex].position.y = m_radius * sin_theta * m_sincosTable[iSlice].sin;
				vertices[vertex].position.z = m_radius * cos_theta;
				vertex++;

				// faces
				if (iSlice > 0)
				{
					faces[face][0] = vertex_index(m_slices, iSlice - 1, iStack - 1);
					faces[face][1] = vertex_index(m_slices, iSlice, iStack - 1);
					faces[face][2] = vertex_index(m_slices, iSlice - 1, iStack);
					face++;

					faces[face][0] = vertex_index(m_slices, iSlice, iStack - 1);
					faces[face][1] = vertex_index(m_slices, iSlice, iStack);
					faces[face][2] = vertex_index(m_slices, iSlice - 1, iStack);
					face++;
				}
			}
		}
#endif
	}

	static uint16_t vertex_index(int slices, int slice, int stack)
	{
		return stack*slices + slice + 1;
	}

	void MakeSinCosTable()
	{
		float phi_start = Math::PI / 2.0f;
		float phi_step = -2.0f * Math::PI / m_slices;
		m_sincosTable.Resize(m_slices + 1);

		float angle = phi_start;
		for (int i = 0; i < m_slices; ++i)
		{
			m_sincosTable[i].sin = std::sinf(angle);
			m_sincosTable[i].cos = std::cosf(angle);
			angle -= phi_step;
		}

		m_sincosTable[m_slices] = m_sincosTable[0];
	}

private:
	float	m_radius;
	int		m_slices;
	int		m_stacks;

	List<SinCos>	m_sincosTable;
};

} // namespace detail
LN_NAMESPACE_END
