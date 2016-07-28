#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>
#include "../../../src/Scene/MME/MMEShader.h"

class Test_Graphics_XFile : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_XFile, Basic)
{
	//auto shader = MMEShader::Create(LN_LOCALFILE("../../../src/Scene/Resource/BasicForwardRendering.fx"));
	auto shader = MMEShader::Create(LN_LOCALFILE("TestData/Grid.fx"));
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Grid1.png"), TextureFormat::R8G8B8A8, 4);

	//auto mesh = StaticMesh::Create(LN_LOCALFILE("TestData/Box_1.x"));
	//auto mesh = StaticMesh::Create(_T("D:/tmp/box1.x"));
	//auto mesh = StaticMesh::CreateSphere(2, 32, 16);
	//auto mesh = StaticMesh::CreateScreenPlane();
	auto mesh = StaticMesh::CreateSquarePlane(Vector2(1, 2), Vector3::UnitY, MeshCreationFlags::DynamicBuffers);
	mesh->SetShader(shader);
	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, tex);

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	//while (Engine::UpdateFrame());

	struct Line
	{
		Vector3	from;
		Vector3	to;
	};

	while (Engine::UpdateFrame())
	{
		auto& vf = Camera::GetMain3DCamera()->GetViewFrustum();
		Vector3 points[8];
		vf.GetCornerPoints(points);

		Line lines[12] =
		{
			{ points[0], points[1] },
			{ points[1], points[2] },
			{ points[2], points[3] },
			{ points[3], points[0] },

			{ points[0], points[4] },
			{ points[1], points[5] },
			{ points[2], points[6] },
			{ points[3], points[7] },

			{ points[4], points[5] },
			{ points[5], points[6] },
			{ points[6], points[7] },
			{ points[7], points[4] },
		};

		Plane plane(0, 1, 0, 0);
		Array<Vector3> hits;
		for (Line& li : lines)
		{
			Vector3 pt;
			if (plane.Intersects(li.from, li.to, &pt))
			{
				hits.Add(pt);
			}
		}

		Vector3 minPos, maxPos;
		for (Vector3& p : hits)
		{
			minPos = Vector3::Min(p, minPos);
			maxPos = Vector3::Max(p, maxPos);
		}

		auto* model = mesh->GetStaticMeshModel();
		model->SetPosition(0, Vector3(minPos.x, 0, maxPos.z));
		model->SetPosition(1, Vector3(minPos.x, 0, minPos.z));
		model->SetPosition(2, Vector3(maxPos.x, 0, maxPos.z));
		model->SetPosition(3, Vector3(maxPos.x, 0, minPos.z));
		model->SetUV(0, Vector2(-1.0f, 1.0f));
		model->SetUV(1, Vector2(-1.0f, -1.0f));
		model->SetUV(2, Vector2(1.0f, 1.0f));
		model->SetUV(3, Vector2(1.0f, -1.0f));
	}
}
