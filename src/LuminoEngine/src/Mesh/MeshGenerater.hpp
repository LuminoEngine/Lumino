#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {
namespace detail {
class MeshGenerater;

class MeshGeneraterBuffer
{
public:
    void setBuffer(Vertex* vertexBuffer, void* indexBuffer, IndexBufferFormat indexFormat, uint32_t indexNumberOffset);

    void generate(MeshGenerater* generator);


    Vertex* vertexBuffer() const { return m_vertexBuffer; }

    void setV(int index, const Vertex& v)
    {
        m_vertexBuffer[index] = v;
    }

    void setV(int index, const Vector3&	position, const Vector2& uv, const Vector3& normal);

    void setI(int index, uint32_t i)
    {
        if (m_indexFormat == IndexBufferFormat::UInt16)
            ((uint16_t*)m_indexBuffer)[index] = m_indexNumberOffset + i;
        else
            ((uint32_t*)m_indexBuffer)[index] = m_indexNumberOffset + i;
    }

private:
    Vertex* m_vertexBuffer;
    void* m_indexBuffer;
    IndexBufferFormat m_indexFormat;
    uint32_t m_indexNumberOffset;
    MeshGenerater* m_generator;
};

#define LN_MESHGENERATOR_CLONE_IMPLEMENT(type) \
    virtual MeshGenerater* clone(LinearAllocator* allocator) const override \
    { \
        auto* ptr = allocator->allocate(sizeof(type)); \
        type* p = new (ptr)type(/**this*/); \
        p->copyFrom(this); \
        return p; \
    }

// 描画コマンドのバッファに詰め込まれるのであまり大きいデータは持たせないように。
class MeshGenerater
{
public:
    virtual ~MeshGenerater();

    void setColor(const Color& color) { m_color = color; }
    const Color& color() const { return m_color; }
    void setTransform(const Matrix& transform) { m_transform = transform; }

    //virtual size_t instanceSize() const = 0;
    //virtual void copyTo(MeshGenerater* other) const = 0;
    virtual int vertexCount() const = 0;
    virtual int indexCount() const = 0;
    virtual PrimitiveTopology primitiveType() const = 0;
    virtual MeshGenerater* clone(LinearAllocator* allocator) const = 0;
    virtual void onGenerate(MeshGeneraterBuffer* buf) = 0;


    void transform(Vertex* begin, int vertexCount)
    {
        if (!m_transform.isIdentity())
        {
            Vertex* end = begin + vertexCount;
            for (Vertex* v = begin; v < end; v++)
            {
                v->position.transformCoord(m_transform);
            }
        }
    }

protected:

    void copyFrom(const MeshGenerater* other)
    {
        m_color = other->m_color;
        m_transform = other->m_transform;
    }

    Color m_color;
    Matrix m_transform;
};

class SingleLineGenerater
    : public MeshGenerater
{
public:
    Vector3 point1;
    Color point1Color;
    Vector3 point2;
    Color point2Color;

    virtual int vertexCount() const override { return 2; }
    virtual int indexCount() const override { return 2; }
    virtual PrimitiveTopology primitiveType() const override { return PrimitiveTopology::LineList; }
    virtual void onGenerate(MeshGeneraterBuffer* buf) override
    {
        buf->setV(0, Vertex{ point1, Vector3::UnitY, Vector2::Zero, point1Color });
        buf->setV(1, Vertex{ point2, Vector3::UnitY, Vector2::Zero, point2Color });
        buf->setI(0, 0);
        buf->setI(1, 1);
    }
    void copyFrom(const SingleLineGenerater* other)
    {
        MeshGenerater::copyFrom(other);
        point1 = other->point1;
        point1Color = other->point1Color;
        point2 = other->point2;
        point2Color = other->point2Color;
    }
    LN_MESHGENERATOR_CLONE_IMPLEMENT(SingleLineGenerater);
};

// Y+ を正面とする
class PlaneMeshGenerater
    : public MeshGenerater
{
public:
    Vector2	size;

    virtual int vertexCount() const override { return 4; }
    virtual int indexCount() const override { return 6; }
    virtual PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
    virtual void onGenerate(MeshGeneraterBuffer* buf) override
    {
        Vector2 half = size / 2;
        buf->setV(0, Vector3(-half.x, 0, half.y), Vector2(0.0f, 0.0f), Vector3::UnitY);
        buf->setV(1, Vector3(half.x, 0, half.y), Vector2(1.0f, 0.0f), Vector3::UnitY);
        buf->setV(2, Vector3(-half.x, 0, -half.y), Vector2(0.0f, 1.0f), Vector3::UnitY);
        buf->setV(3, Vector3(half.x, 0, -half.y), Vector2(1.0f, 1.0f), Vector3::UnitY);
        buf->setI(0, 0);
        buf->setI(1, 1);
        buf->setI(2, 2);
        buf->setI(3, 2);
        buf->setI(4, 1);
        buf->setI(5, 3);
    }
    void copyFrom(const PlaneMeshGenerater* other)
    {
        MeshGenerater::copyFrom(other);
        size = other->size;
    }
    LN_MESHGENERATOR_CLONE_IMPLEMENT(PlaneMeshGenerater);
};

enum class PlaneMeshDirection
{
	XPlus,			/**< X+ 方向 (右向き) */
	YPlus,			/**< Y+ 方向 (上向き) */
	ZPlus,			/**< Z+ 方向 (奥向き) */
	XMinus,			/**< X- 方向 (左向き) */
	YMinus,			/**< Y- 方向 (下向き) */
	ZMinus,			/**< Z- 方向 (手前向き) */
};

class PlaneMeshGenerater2
	: public MeshGenerater
{
public:
	Vector2	size;
	int sliceH;
	int sliceV;
	PlaneMeshDirection direction = PlaneMeshDirection::YPlus;

	virtual int vertexCount() const override { return (sliceH + 1) * (sliceV + 1); }
	virtual int indexCount() const override { return (sliceH * sliceV * 2) * 3; }
	virtual PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
	virtual void onGenerate(MeshGeneraterBuffer* buf) override
	{
		Vector2 minPos = -size / 2;
		Vector2 maxPos = size / 2;

		float minX = -size.x / 2;
		float maxX = size.x / 2;
		float minZ = -size.y / 2;
		float maxZ = size.y / 2;
		float stepX = (maxPos.x - minPos.x) / sliceH;
		float stepZ = (maxPos.y - minPos.y) / sliceV;
		float StepU = 1.0f / sliceH;
		float StepV = 1.0f / sliceV;

		// vertices
		Vector3 pos;
		int iV = 0;
		for (int iZ = 0; iZ < sliceV + 1; ++iZ)
		{
			for (int iX = 0; iX < sliceH + 1; ++iX)
			{
				float h, v;
				if (iX == sliceH)
					h = maxX;	// Set directly to floating point error
				else
					h = minX + stepX * iX;

				if (iZ == sliceV)
					v = minZ;	// Set directly to floating point error
				else
					v = maxZ - stepZ * iZ;

				switch (direction)
				{
				case ln::detail::PlaneMeshDirection::XPlus:
					LN_NOTIMPLEMENTED();
					break;
				case ln::detail::PlaneMeshDirection::YPlus:
					pos.set(h, 0.0f, v);
					break;
				case ln::detail::PlaneMeshDirection::ZPlus:
					LN_NOTIMPLEMENTED();
					break;
				case ln::detail::PlaneMeshDirection::XMinus:
					LN_NOTIMPLEMENTED();
					break;
				case ln::detail::PlaneMeshDirection::YMinus:
					LN_NOTIMPLEMENTED();
					break;
				case ln::detail::PlaneMeshDirection::ZMinus:
					pos.set(h, v, 0);
					break;
				default:
					break;
				}

				buf->setV(iV, pos, Vector2(StepU * iX, 1.0f - StepV * iZ), Vector3(0.0f, 1.0f, 0.0f));
				iV++;
			}
		}

		// indices
		int iI = 0;
		for (int iV = 0; iV < sliceV; ++iV)
		{
			for (int iH = 0; iH < sliceH; ++iH)
			{
				int p1 = (iH + 0) + (iV + 0) * (sliceH + 1);	// ┏
				int p2 = (iH + 1) + (iV + 0) * (sliceH + 1);	// ┓
				int p3 = (iH + 0) + (iV + 1) * (sliceH + 1);	// ┗
				int p4 = (iH + 1) + (iV + 1) * (sliceH + 1);	// ┛
				buf->setI(iI + 0, p1);
				buf->setI(iI + 1, p2);
				buf->setI(iI + 2, p3);
				buf->setI(iI + 3, p3);
				buf->setI(iI + 4, p2);
				buf->setI(iI + 5, p4);
				iI += 6;
			}
		}
	}
	void copyFrom(const PlaneMeshGenerater2* other)
	{
		MeshGenerater::copyFrom(other);
		size = other->size;
		sliceH = other->sliceH;
		sliceV = other->sliceV;
	}
	LN_MESHGENERATOR_CLONE_IMPLEMENT(PlaneMeshGenerater2);
};

class RegularSphereMeshFactory
    : public MeshGenerater
{
public:
    float m_radius;
    int m_slices;
    int m_stacks;

    virtual int vertexCount() const override { return (m_slices + 1) * (m_stacks + 1); }
    virtual int indexCount() const override { return m_slices * m_stacks * 6; }
    virtual PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
    virtual MeshGenerater* clone(LinearAllocator* allocator) const override
    {
        void* ptr = allocator->allocate(sizeof(RegularSphereMeshFactory));
        return new (ptr)RegularSphereMeshFactory(*this);
    }
    void copyFrom(const RegularSphereMeshFactory* other)
    {
        MeshGenerater::copyFrom(other);
        m_radius = other->m_radius;
        m_slices = other->m_slices;
        m_stacks = other->m_stacks;
    }

    struct sinCos
    {
        float	sin;
        float	cos;
    };

    virtual void onGenerate(MeshGeneraterBuffer* buf) override
    {
        makeSinCosTable();

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

                buf->setV(iV, normal * m_radius, Vector2(sliceUV, stackUV), normal);
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
                int p2 = (iSlice + 1) + (iStack + 0) * (m_slices + 1);	// ┓
                int p3 = (iSlice + 0) + (iStack + 1) * (m_slices + 1);	// ┗
                int p4 = (iSlice + 1) + (iStack + 1) * (m_slices + 1);	// ┛
                buf->setI(iI + 0, p1);
                buf->setI(iI + 1, p2);
                buf->setI(iI + 2, p3);
                buf->setI(iI + 3, p3);
                buf->setI(iI + 4, p2);
                buf->setI(iI + 5, p4);
                iI += 6;
            }
        }

        transform(buf->vertexBuffer(), vertexCount());
    }

    void makeSinCosTable()
    {
        if (m_sincosTable.isEmpty())
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
    }

private:
    List<sinCos> m_sincosTable;
};


// 6面それぞれ独立した頂点を持つ直方体
class RegularBoxMeshFactory
	: public MeshGenerater
{
public:
	Vector3	m_size;

	virtual int vertexCount() const override { return 24; }
	virtual int indexCount() const override { return 36; }
	virtual PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
	virtual MeshGenerater* clone(LinearAllocator* allocator) const override
	{
		void* ptr = allocator->allocate(sizeof(RegularBoxMeshFactory));
		return new (ptr)RegularBoxMeshFactory(*this);
	}
	void copyFrom(const RegularBoxMeshFactory* other)
	{
		MeshGenerater::copyFrom(other);
		m_size = other->m_size;
	}


	void setIndices(MeshGeneraterBuffer* buf, uint16_t index, uint16_t begin)
	{
		buf->setI(index + 0, begin + 0);
		buf->setI(index + 1, begin + 1);
		buf->setI(index + 2, begin + 2);
		buf->setI(index + 3, begin + 2);
		buf->setI(index + 4, begin + 1);
		buf->setI(index + 5, begin + 3);
	}

	virtual void onGenerate(MeshGeneraterBuffer* buf) override
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);
		//Vertex* v = outVertices;
		//uint16_t* i = outIndices;
		uint32_t v = 0;
		uint32_t i = 0;

		// 手前 (Z-)
		buf->setV(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitZ); ++v;	// ┏
		buf->setV(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitZ); ++v;	// ┓
		buf->setV(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitZ); ++v;	// ┗
		buf->setV(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitZ); ++v;	// ┛
		setIndices(buf, i, 0); i += 6;

		// 奥 (Z+)
		buf->setV(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), Vector3::UnitZ); ++v;	// ┏
		buf->setV(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitZ); ++v;	// ┓
		buf->setV(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), Vector3::UnitZ); ++v;	// ┗
		buf->setV(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), Vector3::UnitZ); ++v;	// ┛
		setIndices(buf, i, 4); i += 6;

		// 左 (X-)
		buf->setV(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitX); ++v;	// ┏
		buf->setV(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitX); ++v;	// ┓
		buf->setV(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitX); ++v;	// ┗
		buf->setV(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitX); ++v;	// ┛
		setIndices(buf, i, 8); i += 6;

		// 右 (X+)
		buf->setV(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(0.0f, 0.0f), Vector3::UnitX); ++v;	// ┏
		buf->setV(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitX); ++v;	// ┓
		buf->setV(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(0.0f, 1.0f), Vector3::UnitX); ++v;	// ┗
		buf->setV(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), Vector3::UnitX); ++v;	// ┛
		setIndices(buf, i, 12); i += 6;

		// 下 (Y-)(Z- がUVの上方向)
		buf->setV(v, Vector3(minPos.x, minPos.y, minPos.z), Vector2(0.0f, 0.0f), -Vector3::UnitY); ++v;	// ┏
		buf->setV(v, Vector3(maxPos.x, minPos.y, minPos.z), Vector2(1.0f, 0.0f), -Vector3::UnitY); ++v;	// ┓
		buf->setV(v, Vector3(minPos.x, minPos.y, maxPos.z), Vector2(0.0f, 1.0f), -Vector3::UnitY); ++v;	// ┗
		buf->setV(v, Vector3(maxPos.x, minPos.y, maxPos.z), Vector2(1.0f, 1.0f), -Vector3::UnitY); ++v;	// ┛
		setIndices(buf, i, 16); i += 6;

		// 上 (Y+)(Z+ がUVの上方向)
		buf->setV(v, Vector3(minPos.x, maxPos.y, maxPos.z), Vector2(0.0f, 0.0f), Vector3::UnitY); ++v;	// ┏
		buf->setV(v, Vector3(maxPos.x, maxPos.y, maxPos.z), Vector2(1.0f, 0.0f), Vector3::UnitY); ++v;	// ┓
		buf->setV(v, Vector3(minPos.x, maxPos.y, minPos.z), Vector2(0.0f, 1.0f), Vector3::UnitY); ++v;	// ┗
		buf->setV(v, Vector3(maxPos.x, maxPos.y, minPos.z), Vector2(1.0f, 1.0f), Vector3::UnitY); ++v;	// ┛
		setIndices(buf, i, 20);

		transform(buf->vertexBuffer(), vertexCount());
	}
};


} // namespace detail
} // namespace ln

