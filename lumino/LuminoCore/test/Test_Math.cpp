#include "Test_Math_Common.hpp"
#include <LuminoCore/Serialization/Serialization.hpp>
#include <LuminoCore/Math/MathSerialization.hpp>

class Test_Math : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Math, Basic)
{
	// DegreesToRadians
	{
		ASSERT_FLOAT_EQ(Math::PI, Math::degreesToRadians(180.0f));
	}
	// RadiansToDegrees
	{
		ASSERT_FLOAT_EQ(180.0f, Math::radiansToDegrees(Math::PI));
	}
	// clamp
	{
		ASSERT_FLOAT_EQ(0.0, Math::clamp(0.0, 0.0, 1.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp(1.0, 0.0, 1.0));
		ASSERT_FLOAT_EQ(0.0, Math::clamp(0.0 - FLT_EPSILON, 0.0, 1.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp(1.0 + FLT_EPSILON, 0.0, 1.0));
	}
	// clamp01
	{
		ASSERT_FLOAT_EQ(0.0, Math::clamp01(0.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp01(1.0));
		ASSERT_FLOAT_EQ(0.0, Math::clamp01(0.0 - FLT_EPSILON));
		ASSERT_FLOAT_EQ(1.0, Math::clamp01(1.0 + FLT_EPSILON));
	}
	// nextPow2
	{
		ASSERT_EQ(2, Math::nextPow2(0));
		ASSERT_EQ(128, Math::nextPow2(127));
		ASSERT_EQ(128, Math::nextPow2(128));
		ASSERT_EQ(256, Math::nextPow2(129));
		ASSERT_EQ(1024, Math::nextPow2(900));
	}
	// NearEqual
	{
		ASSERT_EQ(true, Math::nearEqual(1.0, 1.0));
		ASSERT_EQ(false, Math::nearEqual(1.0, 1.0 + FLT_EPSILON));
		ASSERT_EQ(false, Math::nearEqual(1.0, 1.0 - FLT_EPSILON));
	}
	// isnan/isinf (float)
	{
		volatile float Float_Zero = 0.0f;
		float a = 0.0f / Float_Zero;	// NaN
		float b = 1.0f / Float_Zero;	// Inf

		ASSERT_EQ(true, Math::isNaN(a));
		ASSERT_EQ(false, Math::isNaN(b));
		ASSERT_EQ(false, Math::isInf(a));
		ASSERT_EQ(true, Math::isInf(b));
	}
	// isnan/isinf (double)
	{
		volatile double Float_Zero = 0.0;
		double a = 0.0 / Float_Zero;	// NaN
		double b = 1.0 / Float_Zero;	// Inf

		ASSERT_EQ(true, Math::isNaN(a));
		ASSERT_EQ(false, Math::isNaN(b));
		ASSERT_EQ(false, Math::isInf(a));
		ASSERT_EQ(true, Math::isInf(b));
	}
	// QuadAccel
	{
		ASSERT_FLOAT_EQ(0.0f, Math::quadAccel(0, 20.0, -9.8f, 0.0));
		ASSERT_FLOAT_EQ(20.4f, Math::quadAccel(0, 20.0, -9.8f, 2.0));
		ASSERT_FLOAT_EQ(1.599998f, Math::quadAccel(0, 20.0, -9.8f, 4.0));
		ASSERT_FLOAT_EQ(-56.400009f, Math::quadAccel(0, 20.0, -9.8f, 6.0));
		ASSERT_FLOAT_EQ(-153.600006f, Math::quadAccel(0, 20.0, -9.8f, 8.0));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Math, Serialization)
{
	// Three.js では VectorX は配列として保存されるのでそれに合わせてみる
	// https://github.com/josdirksen/learning-threejs/blob/master/chapter-08/04-load-save-json-scene.html
	// Export: {"metadata":{"formatVersion":3.2,"type":"scene","generatedBy":"SceneExporter","objects":5,"geometries":3,"materials":3,"textures":0},"urlBaseType":"relativeToScene","objects":{"Object_3":{"geometry":"Geometry_3","material":"Material_4","position":[15,0,0],"rotation":[-1.5707963267948966,0,0],"scale":[1,1,1],"visible":true},"Object_4":{"geometry":"Geometry_5","material":"Material_5","position":[-4,3,0],"rotation":[0,0,0],"scale":[1,1,1],"visible":true},"Object_5":{"geometry":"Geometry_6","material":"Material_6","position":[20,0,2],"rotation":[0,0,0],"scale":[1,1,1],"visible":true},"Object_6":{"type":"AmbientLight","color":789516},"Object_7":{"type":"PointLight","color":16777215,"intensity":1,"position":[-40,60,-10],"distance":0}},"geometries":{"Geometry_3":{"type":"plane","width":60,"height":20,"widthSegments":1,"heightSegments":1},"Geometry_5":{"type":"cube","width":4,"height":4,"depth":4,"widthSegments":1,"heightSegments":1,"depthSegments":1},"Geometry_6":{"type":"sphere","radius":4,"widthSegments":20,"heightSegments":20}},"materials":{"Material_4":{"type":"MeshLambertMaterial","parameters":{"color":16777215,"ambient":16777215,"emissive":0,"reflectivity":1,"transparent":false,"opacity":1,"wireframe":false,"wireframeLinewidth":1}},"Material_5":{"type":"MeshLambertMaterial","parameters":{"color":16711680,"ambient":16777215,"emissive":0,"reflectivity":1,"transparent":false,"opacity":1,"wireframe":false,"wireframeLinewidth":1}},"Material_6":{"type":"MeshLambertMaterial","parameters":{"color":7829503,"ambient":16777215,"emissive":0,"reflectivity":1,"transparent":false,"opacity":1,"wireframe":false,"wireframeLinewidth":1}}},"textures":{},"fogs":{},"transform":{"position":[0,0,0],"rotation":[0,0,0],"scale":[1,1,1]},"defaults":{"camera":"","fog":""}}

	struct Data
	{
		Vector2 v2;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(v2);
		}
	};
	Data data1;
	data1.v2 = Vector2(1, 2);

	ln::String json = JsonSerializer::serialize(data1, JsonFormatting::None);

	Data data2;
	JsonSerializer::deserialize(json, data2);

	ASSERT_FLOAT_EQ(1, data2.v2.x);
	ASSERT_FLOAT_EQ(2, data2.v2.y);
}

