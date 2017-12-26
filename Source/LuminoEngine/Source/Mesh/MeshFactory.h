/*
	"RegularXXXX" という名前のついているファクトリは、UV 展開したときに自然になるようなジオメトリを生成する。
	例えば Box だと、テクスチャを使用しない単色の Box であれば頂点は 8 個でよい。各6面は、頂点を共有できる。
	しかし、ダイスのように各面に別の UV を設定したい場合は頂点を共有することはできない。この場合は 24 個の頂点が必要になる。
	そのようなジオメトリを作成するのが "RegularXXXX"。
*/
#include "../Internal.h"
#include <Lumino/Graphics/Vertex.h>

LN_NAMESPACE_BEGIN
namespace detail {

class MeshHelper
{
public:

	static Vector3 getXZCirclePoint(int i, int slices)
	{
		float angle = i * Math::PI2 / slices;
		float dx, dz;
		Math::sinCos(angle, &dx, &dz);
		return Vector3(dx, 0, dz);
	}

	static Vector3 getXZArcPoint(float startAngle, float endAngle, int i, int slices)
	{
		float da = endAngle - startAngle;
		float angle = (i * da / slices) + startAngle;
		float dx, dz;
		Math::sinCos(angle, &dx, &dz);
		return Vector3(dx, 0, dz);
	}

	static void transform(Vertex* begin, Vertex* end, const Matrix& transform)
	{
		for (Vertex* v = begin; v < end; v++)
		{
			v->position.transformCoord(transform);
		}
	}
};

class MeshFactoryBase
{
public:
	MeshFactoryBase()
		: m_color(Color::White)
		, m_transform()
	{
	}

	void initialize(const Color& color, const Matrix& transform)
	{
		m_color = color;
		m_transform = transform;
	}

	void setBuffers(Vertex* outVertices, void* outIndices, IndexBufferFormat ibfmt, uint32_t beginIndex)
	{
		m_vertices = outVertices;
		m_indices = outIndices;
		m_indexBufferFormat = ibfmt;
		m_beginIndex = beginIndex;
	}

	void setV2(int index, const Vertex& v)
	{
		m_vertices[index] = v;
	}

	void setV2(int index, const Vector3&	position, const Vector2& uv, const Vector3& normal)
	{
		m_vertices[index].position = position;
		m_vertices[index].uv = uv;
		m_vertices[index].normal = normal;
		m_vertices[index].color = m_color;
	}

	void setI2(int index, uint32_t i)
	{
		if (m_indexBufferFormat == IndexBufferFormat_UInt16)
			((uint16_t*)m_indices)[index] = i + m_beginIndex;
		else
			((uint32_t*)m_indices)[index] = i + m_beginIndex;
	}

	Vertex*				m_vertices;
	void*				m_indices;
	IndexBufferFormat	m_indexBufferFormat;
	uint32_t			m_beginIndex;
	Color				m_color;
	Matrix				m_transform;
};

class PlaneMeshFactory
	: public MeshFactoryBase
{
public:
	PlaneMeshFactory(const Vector2& size)
	{
		m_size = size;
	}

	int getVertexCount() const
	{
		return 4;
	}

	int getIndexCount() const
	{
		return 6;
	}

	void generate()
	{
		Vector2 half = m_size / 2;
		setV2(0, Vector3(-half.x, half.y, 0), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
		setV2(1, Vector3(-half.x, -half.y, 0), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
		setV2(2, Vector3(half.x, half.y, 0), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
		setV2(3, Vector3(half.x, -half.y, 0), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
		setI2(0, 0);
		setI2(1, 1);
		setI2(2, 2);
		setI2(3, 2);
		setI2(4, 1);
		setI2(5, 3);
	}

private:
	Vector2	m_size;
};

//class PlaneMeshFactory2
//	: public MeshFactoryBase
//{
//public:
//	PlaneMeshFactory2(const Vector2& size, const Vector3& front)
//		: m_size(size)
//		, m_front(front)
//	{}
//
//	int getVertexCount() const { return 4; }
//	int getIndexCount() const { return 6; }
//
//	void generate()
//	{
//		Vector2 half = m_size / 2;
//		if (m_front == Vector3::UnitY)
//		{
//			setV2(0, Vector3(-half.x, 0, half.y), Vector2(0.0f, 0.0f), m_front);
//			setV2(1, Vector3(-half.x, 0, -half.y), Vector2(0.0f, 1.0f), m_front);
//			setV2(2, Vector3(half.x, 0, half.y), Vector2(1.0f, 0.0f), m_front);
//			setV2(3, Vector3(half.x, 0, -half.y), Vector2(1.0f, 1.0f), m_front);
//		}
//		else if(m_front == -Vector3::UnitY)
//		{
//			setV2(0, Vector3(half.x, 0, half.y), Vector2(0.0f, 0.0f), m_front);
//			setV2(1, Vector3(half.x, 0, -half.y), Vector2(0.0f, 1.0f), m_front);
//			setV2(2, Vector3(-half.x, 0, half.y), Vector2(1.0f, 0.0f), m_front);
//			setV2(3, Vector3(-half.x, 0, -half.y), Vector2(1.0f, 1.0f), m_front);
//		}
//		else
//		{
//			LN_NOTIMPLEMENTED();
//		}
//		setI2(0, 0);
//		setI2(1, 1);
//		setI2(2, 2);
//		setI2(3, 2);
//		setI2(4, 1);
//		setI2(5, 3);
//	}
//
//private:
//	Vector2	m_size;
//	Vector3	m_front;
//};

// xz 平面。y+ を向く。
// 頂点の並びは
/*
	1 2 3
	4 5 6
	7 8 9
*/
// 頂点は共有する。
class PlaneMeshFactory3
	: public MeshFactoryBase
{
public:
	PlaneMeshFactory3()
		: m_size()
		, m_sliceX(0)
		, m_sliceZ(0)
	{
	}

	void initialize(const Vector2& size, int sliceX, int sliceZ, const Color& color, const Matrix& transform)
	{
		if (LN_REQUIRE(sliceX >= 1)) return;
		if (LN_REQUIRE(sliceZ >= 1)) return;
		m_size = size;
		m_sliceX = sliceX;
		m_sliceZ = sliceZ;
		MeshFactoryBase::initialize(color, transform);
	}

	int getVertexCount() const { return (m_sliceX + 1) * (m_sliceZ + 1); }
	int getIndexCount() const { return (m_sliceX * m_sliceZ * 2) * 3; }

	void generate()
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

		// vertices
		Vector3 pos;
		int iV = 0;
		for (int iZ = 0; iZ < m_sliceZ + 1; ++iZ)
		{
			for (int iX = 0; iX < m_sliceX + 1; ++iX)
			{
				if (iX == m_sliceX)
					pos.x = maxX;	// 誤差を出したくないため直接設定
				else
					pos.x = minX + stepX * iX;

				if (iZ == m_sliceZ)
					pos.z = minZ;	// 誤差を出したくないため直接設定
				else
					pos.z = maxZ - stepZ * iZ;

				pos.y = 0.0f;

				setV2(iV, pos, Vector2(StepU * iX, 1.0f - StepV * iZ), Vector3(0.0f, 1.0f, 0.0f));
				iV++;
			}
		}

		// indices
		int iI = 0;
		for (int iZ = 0; iZ < m_sliceZ; ++iZ)
		{
			for (int iX = 0; iX < m_sliceX; ++iX)
			{
				int p1 = (iX + 0) + (iZ + 0) * (m_sliceX + 1);	// ┏
				int p2 = (iX + 0) + (iZ + 1) * (m_sliceX + 1);	// ┗
				int p3 = (iX + 1) + (iZ + 0) * (m_sliceX + 1);	// ┓
				int p4 = (iX + 1) + (iZ + 1) * (m_sliceX + 1);	// ┛
				setI2(iI + 0, p1);
				setI2(iI + 1, p2);
				setI2(iI + 2, p3);
				setI2(iI + 3, p3);
				setI2(iI + 4, p2);
				setI2(iI + 5, p4);
				iI += 6;
			}
		}

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + iV, m_transform);
	}

private:
	Vector2	m_size;
	int		m_sliceX;
	int		m_sliceZ;
};

//class BoxMeshFactory
//{
//public:
//	BoxMeshFactory(const Vector3& size)
//	{
//		m_size = size;
//	}
//
//	int getVertexCount() const
//	{
//		return 8;
//	}
//
//	int getIndexCount() const
//	{
//		return 36;
//	}
//
//	void generate()
//	{
//		Vector3 minPos = -(m_size / 2);
//		Vector3 maxPos = (m_size / 2);
//
//		// 手前 (Z-)
//		outVertices[0].position.set(minPos.x, maxPos.y, minPos.z);	// 左上
//		outVertices[0].uv.set(0.0f, 0.0f);
//		outVertices[1].position.set(minPos.x, minPos.y, minPos.z);	// 左下
//		outVertices[1].uv.set(0.0f, 1.0f);
//		outVertices[2].position.set(maxPos.x, maxPos.y, minPos.z);	// 右上
//		outVertices[2].uv.set(1.0f, 0.0f);
//		outVertices[3].position.set(maxPos.x, minPos.y, minPos.z);	// 右下
//		outVertices[3].uv.set(1.0f, 1.0f);
//		// 奥 (Z+)
//		outVertices[4].position.set(minPos.x, maxPos.y, maxPos.z);	// 左上
//		outVertices[4].uv.set(1.0f, 0.0f);
//		outVertices[5].position.set(minPos.x, minPos.y, maxPos.z);	// 左下
//		outVertices[5].uv.set(1.0f, 1.0f);
//		outVertices[6].position.set(maxPos.x, maxPos.y, maxPos.z);	// 右上
//		outVertices[6].uv.set(0.0f, 0.0f);
//		outVertices[7].position.set(maxPos.x, minPos.y, maxPos.z);	// 右下
//		outVertices[7].uv.set(0.0f, 1.0f);
//
//		for (int i = 0; i < 8; ++i) outVertices[i].color = Color::White;
//
//		uint16_t indices[] =
//		{
//			// near
//			0, 1, 2,
//			2, 1, 3,
//			// far
//			6, 7, 4,
//			4, 7, 5,
//			// left
//			4, 5, 0,
//			0, 5, 1,
//			// right
//			2, 3, 6,
//			6, 3, 7,
//			// top
//			4, 0, 6,
//			6, 0, 2,
//			// bottom (Z- が面として上方向)
//			1, 5, 3,
//			3, 5, 7,
//		};
//		memcpy(outIndices, indices, sizeof(indices));
//	}
//
//private:
//	Vector3	m_size;
//};


// 6面それぞれ独立した頂点を持つ直方体
class RegularBoxMeshFactory
	: public MeshFactoryBase
{
public:
	RegularBoxMeshFactory() {}

	void initialize(const Vector3& size, const Color& color, const Matrix& transform)
	{
		m_size = size;
		MeshFactoryBase::initialize(color, transform);
	}

	int getVertexCount() const
	{
		return 24;
	}

	int getIndexCount() const
	{
		return 36;
	}
	
	void setI(uint16_t index, uint16_t begin)
	{
		setI2(index + 0, begin + 0);
		setI2(index + 1, begin + 1);
		setI2(index + 2, begin + 2);
		setI2(index + 3, begin + 2);
		setI2(index + 4, begin + 1);
		setI2(index + 5, begin + 3);
	}

	void generate()
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);
		//Vertex* v = outVertices;
		//uint16_t* i = outIndices;
		uint32_t v = 0;
		uint32_t i = 0;

		// 手前 (Z-)
		setV2(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitZ); ++v;	// ┏
		setV2(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitZ); ++v;	// ┗
		setV2(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitZ); ++v;	// ┓
		setV2(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitZ); ++v;	// ┛
		setI(i, 0); i += 6;

		// 奥 (Z+)
		setV2(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), Vector3::UnitZ); ++v;	// ┏
		setV2(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), Vector3::UnitZ); ++v;	// ┗
		setV2(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitZ); ++v;	// ┓
		setV2(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), Vector3::UnitZ); ++v;	// ┛
		setI(i, 4); i += 6;

		// 左 (X-)
		setV2(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitX); ++v;	// ┏
		setV2(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitX); ++v;	// ┗
		setV2(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitX); ++v;	// ┓
		setV2(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitX); ++v;	// ┛
		setI(i, 8); i += 6;

		// 右 (X+)
		setV2(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(0.0f, 0.0f), Vector3::UnitX); ++v;	// ┏
		setV2(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(0.0f, 1.0f), Vector3::UnitX); ++v;	// ┗
		setV2(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitX); ++v;	// ┓
		setV2(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), Vector3::UnitX); ++v;	// ┛
		setI(i, 12); i += 6;

		// 下 (Y-)(Z- がUVの上方向)
		setV2(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitY); ++v;	// ┏
		setV2(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitY); ++v;	// ┗
		setV2(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitY); ++v;	// ┓
		setV2(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitY); ++v;	// ┛
		setI(i, 16); i += 6;

		// 上 (Y+)(Z+ がUVの上方向)
		setV2(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), Vector3::UnitY); ++v;	// ┏
		setV2(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(0.0f, 1.0f), Vector3::UnitY); ++v;	// ┗
		setV2(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitY); ++v;	// ┓
		setV2(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(1.0f, 1.0f), Vector3::UnitY); ++v;	// ┛
		setI(i, 20);

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + v, m_transform);
	}

private:
	Vector3	m_size;
};



class RegularSphereMeshFactory
	: public MeshFactoryBase
{
public:
	RegularSphereMeshFactory()
		: m_radius(0)
		, m_slices(0)
		, m_stacks(0)
	{
	}

	void initialize(float radius, int slices, int stacks, const Color& color, const Matrix& transform)
	{
		if (LN_REQUIRE(slices >= 3)) return;
		if (LN_REQUIRE(stacks >= 2)) return;
		m_radius = radius;
		m_slices = slices;
		m_stacks = stacks;
		makeSinCosTable();
		MeshFactoryBase::initialize(color, transform);
	}

	// Squashed

	int getVertexCount() const
	{
		return (m_slices + 1) * (m_stacks + 1);
		//return 2 + m_slices * (m_stacks - 1);	// (top と bottom の 2 点) + リングの頂点数 * 重ねる数
	}

	int getIndexCount() const
	{
		return m_slices * m_stacks * 6;
		//return 2 * m_slices + (m_stacks - 2) * (2 * m_slices);
	}

	//typedef uint16_t Face[3];
	//typedef uint16_t QuadFace[6];

	struct sinCos
	{
		float	sin;
		float	cos;
	};

	void generate()
	{
		uint32_t iV = 0;
		uint32_t iI = 0;

		float stackUVStep = 1.0f / m_stacks;
		float stackUV = 0.0f;

		float sliceUVStep = 1.0f / m_slices;

		// XY 平面上の step
		float theta_step = Math::PI / m_stacks;
		float theta = 0.0f;//theta_step;

		// Z+ を起点とし、X- 方向へ回っていく

		// rings (Vertex)
		Vector3 normal;
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
					normal.x = 0.0f;
					normal.y = 1.0f;
					normal.z = 0.0f;
				}
				// bottom
				else if (iStack == m_stacks)
				{
					normal.x = 0.0f;
					normal.y = -1.0f;
					normal.z = 0.0f;
				}
				// between
				else
				{
					normal.x = sin_theta * m_sincosTable[iSlice].cos;
					normal.y = cos_theta;
					normal.z = sin_theta * m_sincosTable[iSlice].sin;
				}

				setV2(iV, normal * m_radius, Vector2(sliceUV, stackUV), normal);
				++iV;

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
				setI2(iI + 0, p1);
				setI2(iI + 1, p2);
				setI2(iI + 2, p3);
				setI2(iI + 3, p3);
				setI2(iI + 4, p2);
				setI2(iI + 5, p4);
				iI += 6;
			}
		}

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + iV, m_transform);
	}

	static uint16_t vertex_index(int slices, int slice, int stack)
	{
		return stack*slices + slice + 1;
	}

	void makeSinCosTable()
	{
		float phi_start = Math::PI / 2.0f;
		float phi_step = -2.0f * Math::PI / m_slices;
		m_sincosTable.resize(m_slices + 1);

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

	List<sinCos>	m_sincosTable;
};

// Sphere 同様、uv 展開の都合上 ring の始点と終点、底の位置は同一。
class RegularCylinderMeshFactory
	: public MeshFactoryBase
{
public:
	RegularCylinderMeshFactory()
		: m_radius(0)
		, m_height(0)
		, m_slices(0)
		, m_stacks(0)
	{
	}

	void initialize(float radius, float height, int slices, int stacks, const Color& color, const Matrix& transform)
	{
		if (LN_REQUIRE(slices >= 3)) return;
		if (LN_REQUIRE(stacks >= 1)) return;
		m_radius = radius;
		m_height = height;
		m_slices = slices;
		m_stacks = stacks;
		MeshFactoryBase::initialize(color, transform);
	}

	int getVertexCount() const
	{
		return (m_slices + 1) * (m_stacks + 3);
	}

	int getIndexCount() const
	{
		return m_slices * (m_stacks + 2) * 6;
	}

	void generate()
	{
		uint32_t iV = 0;
		uint32_t iI = 0;

		float yStep = m_height / m_stacks;
		float y;
		float yu = m_height / 2;
		float yd = -m_height / 2;

		//for (int iSlice = 0; iSlice < m_slices + 1; ++iSlice)
		for (int iSlice = m_slices; iSlice >= 0; iSlice--)
		{
			Vector3 n = MeshHelper::getXZCirclePoint(iSlice, m_slices);
			Vector3 xz = n * m_radius;

			// upper base
			{
				Vertex v;
				v.position.set(0, yu, 0);
				v.normal = Vector3::UnitY;
				setV2(iV, v); iV++;
			}
			// side
			y = yu;
			for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
			{
				Vertex v;
				v.position.x = xz.x;
				v.position.y = y;
				v.position.z = xz.z;
				v.normal = n;
				setV2(iV, v); iV++;
				y -= yStep;
			}
			// lower base
			{
				Vertex v;
				v.position.set(0, yd, 0);
				v.normal = -Vector3::UnitY;
				setV2(iV, v); iV++;
			}
		}

		// faces
		int stacks = m_stacks + 2;
		for (int iSlice = 0; iSlice < m_slices; ++iSlice)	// x
		{
			for (int iStack = 0; iStack < stacks; ++iStack)	// y
			{
				int p1 = (iStack + 0) + (iSlice + 0) * (stacks + 1);	// ┏
				int p2 = (iStack + 1) + (iSlice + 0) * (stacks + 1);	// ┗
				int p3 = (iStack + 0) + (iSlice + 1) * (stacks + 1);	// ┓
				int p4 = (iStack + 1) + (iSlice + 1) * (stacks + 1);	// ┛
				setI2(iI + 0, p1);
				setI2(iI + 1, p2);
				setI2(iI + 2, p3);
				setI2(iI + 3, p3);
				setI2(iI + 4, p2);
				setI2(iI + 5, p4);
				iI += 6;
			}
		}

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + iV, m_transform);
	}

private:
	float	m_radius;
	float	m_height;
	int		m_slices;
	int		m_stacks;
};


class RegularConeMeshFactory
	: public MeshFactoryBase
{
public:
	RegularConeMeshFactory()
		: m_radius(0)
		, m_height(0)
		, m_slices(0)
	{
	}

	void initialize(float radius, float height, int slices, const Color& color, const Matrix& transform)
	{
		if (LN_REQUIRE(slices >= 3)) return;
		m_radius = radius;
		m_height = height;
		m_slices = slices;
		MeshFactoryBase::initialize(color, transform);
	}

	int getVertexCount() const
	{
		return (m_slices + 1) * 3;
	}

	int getIndexCount() const
	{
		return m_slices * 3 * 6;
	}

	void generate()
	{
		uint32_t iV = 0;
		uint32_t iI = 0;

		for (int iSlice = m_slices; iSlice >= 0; iSlice--)
		{
			Vector3 n = MeshHelper::getXZCirclePoint(iSlice, m_slices);
			Vector3 xz = n * m_radius;

			// top
			{
				Vertex v;
				v.position.set(0, m_height / 2, 0);
				v.normal = Vector3::UnitY;
				setV2(iV, v); iV++;
			}
			// side
			float y = -m_height / 2;
			{
				Vertex v;
				v.position.set(xz.x, y, xz.z);
				v.normal = n;
				setV2(iV, v); iV++;
			}
			// lower base
			{
				Vertex v;
				v.position.set(0, y, 0);
				v.normal = -Vector3::UnitY;
				setV2(iV, v); iV++;
			}
		}

		// faces
		int stacks = 2;
		for (int iSlice = 0; iSlice < m_slices; ++iSlice)	// x
		{
			for (int iStack = 0; iStack < stacks; ++iStack)	// y
			{
				int p1 = (iStack + 0) + (iSlice + 0) * (stacks + 1);	// ┏
				int p2 = (iStack + 1) + (iSlice + 0) * (stacks + 1);	// ┗
				int p3 = (iStack + 0) + (iSlice + 1) * (stacks + 1);	// ┓
				int p4 = (iStack + 1) + (iSlice + 1) * (stacks + 1);	// ┛
				setI2(iI + 0, p1);
				setI2(iI + 1, p2);
				setI2(iI + 2, p3);
				setI2(iI + 3, p3);
				setI2(iI + 4, p2);
				setI2(iI + 5, p4);
				iI += 6;
			}
		}

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + iV, m_transform);
	}

private:
	float	m_radius;
	float	m_height;
	int		m_slices;
};


class ArcMeshFactory
	: public MeshFactoryBase
{
public:
	ArcMeshFactory()
		: m_startAngle(0)
		, m_endAngle(0)
		, m_innerRadius(0)
		, m_outerRadius(0)
		, m_slices(0)
	{
	}

	void initialize(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color, const Matrix& transform)
	{
		if (LN_REQUIRE(slices >= 1)) return;
		m_startAngle = startAngle;
		m_endAngle = endAngle;
		m_innerRadius = innerRadius;
		m_outerRadius = outerRadius;
		m_slices = slices;
		MeshFactoryBase::initialize(color, transform);
	}

	int getVertexCount() const
	{
		return (m_slices + 1) * 2;
	}

	int getIndexCount() const
	{
		return m_slices * 6;
	}

	void generate()
	{
		uint32_t iV = 0;
		uint32_t iI = 0;

		for (int iSlice = 0; iSlice < m_slices + 1; iSlice++)
		{
			Vector3 n = MeshHelper::getXZArcPoint(m_startAngle, m_endAngle, iSlice, m_slices);

			// outer
			{
				Vertex v;
				v.position = n * m_outerRadius;
				v.normal = Vector3::UnitY;
				setV2(iV, v); iV++;
			}
			// inner
			{
				Vertex v;
				v.position = n * m_innerRadius;
				v.normal = Vector3::UnitY;
				setV2(iV, v); iV++;
			}
		}

		// faces
		for (int iSlice = 0; iSlice < m_slices; iSlice++)
		{
			int p1 = ((iSlice + 0) * 2 + 0);	// ┏
			int p2 = ((iSlice + 0) * 2 + 1);	// ┗
			int p3 = ((iSlice + 1) * 2 + 0);	// ┓
			int p4 = ((iSlice + 1) * 2 + 1);	// ┛
			setI2(iI + 0, p1);
			setI2(iI + 1, p2);
			setI2(iI + 2, p3);
			setI2(iI + 3, p3);
			setI2(iI + 4, p2);
			setI2(iI + 5, p4);
			iI += 6;
		}

		if (!m_transform.isIdentity())
			MeshHelper::transform(m_vertices, m_vertices + iV, m_transform);
	}

private:
	float	m_startAngle;
	float	m_endAngle;
	float 	m_innerRadius;
	float	m_outerRadius;
	int		m_slices;
};


struct TeapotPatch;

class TeapotMeshFactory
	: public MeshFactoryBase
{
public:
	TeapotMeshFactory();

	void initialize(float size, int tessellation, const Color& color, const Matrix& transform);

	int getVertexCount() const;

	int getIndexCount() const;

	void generate();

private:
	void addVertex(const Vector3& pos, const Vector3& normal, const Vector2& texUV);
	void addIndex(uint32_t index);

	void computeTeapot(float size, size_t tessellation/*, bool rhcoords*/);
	void tessellatePatch(const TeapotPatch& patch, size_t tessellation, const Vector3& scale, bool isMirrored);

	float		m_size;
	int			m_tessellation;
	Vertex*		m_vbBegin;
	Vertex*		m_vbPos;
	uint32_t	m_ibPos;
	//uint16_t	m_beginIndex;
};

} // namespace detail
LN_NAMESPACE_END
