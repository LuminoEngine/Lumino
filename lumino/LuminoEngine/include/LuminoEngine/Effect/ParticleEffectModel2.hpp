#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "Common.hpp"
#include "EffectModel.hpp"
#include "ParticleEffectModel.hpp"

namespace ln {
class Material;
class ParticleEmitterModel;

enum class ParticleGeometryType
{
	Sprite,
};

/** パーティクルの向く方向 */
LN_ENUM()
enum class ParticleGeometryDirection
{
	//Default,

	/** 常に視点方向を向く */
	ToView,

	/** 進行方向を軸に、表面 (Y+) が視点から見えるようにする */
	Top,

	/** Y 軸方向に直立し、カメラの方を向きます。 */
	VerticalBillboard,

	/** XZ 軸上の「床」平面と平行になります。 */
	HorizontalBillboard,
};

/** パーティクルのソート方法 */
enum class ParticleSortMode : uint8_t
{
	/** 
	 * ソートを行いません。加算合成や減算合成で描画するパーティクルは、このモードにすると高速に処理できます。
	 * 一方アルファ値を持つテクスチャは正しく描画できなくなります。(深度テストを無効化することである程度カバーできます)
	 */
	None,

	/**
	 * ビュー平面からの距離でソートを行います。
	 */
	DistanceToView,
};

class ParticleGeometry
	: public ln::Object
{
public:
	ParticleGeometryType type() const { return m_type; }
	virtual uint64_t calculateRendererHashKey(ParticleEmitterModel* emitterModel) const = 0;

LN_CONSTRUCT_ACCESS:
	ParticleGeometry(ParticleGeometryType type);
	bool init();

private:
	ParticleGeometryType m_type;
};

class SpriteParticleGeometry
	: public ParticleGeometry
{
public:
	void setMaterial(Material* material);
	const Ref<Material>& material() const { return m_material; }
	uint64_t calculateRendererHashKey(ParticleEmitterModel* emitterModel) const override;

LN_CONSTRUCT_ACCESS:
	SpriteParticleGeometry();
	bool init();

private:
	Ref<Material> m_material;
};


/** ParticleEmitterModel */
LN_CLASS()
class ParticleEmitterModel
	: public Object
{
public:
	const Ref<ParticleGeometry>& geometry() const { return m_geometry; }




	/** 同時に表示できるパーティクルの最大数を設定します。(default: 100) */
	LN_METHOD(Property)
	void setMaxParticles(int count) { m_maxParticles = count; }

	/** 1秒間に放出するパーティクルの数を設定します。(default: 1) */
	LN_METHOD(Property)
	void setSpawnRate(float rate) { m_spawnRate = rate; }

	/** パーティクルの生存時間を設定します。(default: 5.0) */
	LN_METHOD(Property)
	void setLifeTime(float time) { m_lifeTime.minValue = m_lifeTime.maxValue = time; }

	
	///** setShapeType (default: Sphere) */
	//LN_METHOD(Property)
	//void setShapeType(ParticleEmitterShapeType value) { m_shapeType = value; }

	/** setupBoxShape */
	LN_METHOD(Property)
	void setupBoxShape(const Vector3& size);

	
	/**  (default: 1.0) */
	LN_METHOD(Property)
	void setSize(float value) { m_size.set(value); }

	/**  */
	LN_METHOD(Property)
	void setSizeVelocity(float value) { m_sizeVelocity.set(value); }
	
	/**  */
	LN_METHOD(Property)
	void setSizeAcceleration(float value) { m_sizeAcceleration.set(value); }


	
	/**  (default: 0) */
	LN_METHOD(Property)
	void setForwardVelocityMin(float value) { m_forwardVelocity.minValue = value; }

	/**  (default: 0) */
	LN_METHOD(Property)
	void setForwardVelocityMax(float value) { m_forwardVelocity.maxValue = value; }

	/** 進行方向に対するスケール値。通常、Z軸。ParticleGeometryDirection::ToView では Y scale (default: 1.0) */
	LN_METHOD(Property)
	void setForwardScale(float value) { m_forwardScale.set(value); }
	



	/**  (default: ToView) */
	LN_METHOD(Property)
	void setGeometryDirection(ParticleGeometryDirection value) { m_geometryDirection = value; }



	/** setupSpriteModule */
	LN_METHOD()
	void setupSpriteModule(Material* material);



	// パーティクルのライフタイムを秒単位で
	RadomRangeValue<float> m_lifeTime;



	int m_maxParticles = 1;		// 粒子最大数
	float m_spawnRate = 1;	// 1秒間に放出するパーティクル数
	int m_burstCount = 1;	// 1度の放出タイミングで生成するパーティクル数


	// Dynamics module
	RadomRangeValue<Vector3> m_velocity;
	RadomRangeValue<Vector3> m_acceleration;



	ParticleEmitterShapeType m_shapeType = ParticleEmitterShapeType::Sphere;
	Vector3 m_shapeParam;	// Coneの場合、.x が有効角度。0~PI。Boxの場合は各辺の幅
	RadomRangeValue<float> m_forwardPosition;
	RadomRangeValue<float> m_forwardVelocity;// = { 1, 1 };
	RadomRangeValue<float> m_forwardAccel;
	// ↑ forwardXXXX は、 ShapeType で決まった正面方向に対する Dynamics.
	// Sphere,Cone の場合は放射方向、Box の場合は Y+.
	


	RadomRangeValue<float> m_size;			// default:1 ベースの geom 対するスケール値。
	RadomRangeValue<float> m_sizeVelocity;
	RadomRangeValue<float> m_sizeAcceleration;

	RadomRangeValue<float> m_forwardScale;	// default:1 進行方向に対するスケール値。通常、Z軸。ParticleGeometryDirection::ToView では Y scale
	RadomRangeValue<float> m_forwardScaleVelocity;
	RadomRangeValue<float> m_forwardScaleAcceleration;

	RadomRangeValue<float> m_crossScale;	// default:1 進行方向以外に対するスケール値。XとY軸。ParticleGeometryDirection::ToView では X scale
	RadomRangeValue<float> m_crossScaleVelocity;
	RadomRangeValue<float> m_crossScaleAcceleration;

	ParticleGeometryDirection m_geometryDirection = ParticleGeometryDirection::ToView;

	float m_trailSeconds = 0.0f; //1.0f;	// Trail を生成する時間

	ParticleSortMode m_sortMode = ParticleSortMode::DistanceToView;

protected:
	void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
	ParticleEmitterModel();

	/** init */
	LN_METHOD()
	bool init();

private:
	Ref<ParticleGeometry> m_geometry;
};

/** ParticleModel */
LN_CLASS()
class ParticleModel
	: public EffectResource
{
public:
	/** setLoop */
	LN_METHOD(Property)
	void setLoop(bool value) { m_loop = true; }

	/** setLoop */
	LN_METHOD(Property)
	bool isLoop() const { return m_loop; }
	
	/** setLoop */
	LN_METHOD()
	void addEmitter(ParticleEmitterModel* emitter);


	bool m_loop = true;
	int seed = 0;

	const List<Ref<ParticleEmitterModel>>& emitters() const { return m_emitters; }

protected:
	void serialize(Serializer2& ar) override;


LN_CONSTRUCT_ACCESS:
	ParticleModel();

	/** init */
	LN_METHOD()
	bool init();

private:
	List<Ref<ParticleEmitterModel>> m_emitters;
};

} // namespace ln
