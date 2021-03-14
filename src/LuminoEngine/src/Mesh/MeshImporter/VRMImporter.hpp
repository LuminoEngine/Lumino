
#pragma once
#include "GLTFImporter.hpp"

namespace ln {
namespace detail {

enum class VRMBone
{
    hips,
    leftUpperLeg,
    rightUpperLeg,
    leftLowerLeg,
    rightLowerLeg,
    leftFoot,
    rightFoot,
    spine,
    chest,
    neck,
    head,
    leftShoulder,
    rightShoulder,
    leftUpperArm,
    rightUpperArm,
    leftLowerArm,
    rightLowerArm,
    leftHand,
    rightHand,
    leftToes,
    rightToes,
    leftEye,
    rightEye,
    jaw,
    leftThumbProximal,
    leftThumbIntermediate,
    leftThumbDistal,
    leftIndexProximal,
    leftIndexIntermediate,
    leftIndexDistal,
    leftMiddleProximal,
    leftMiddleIntermediate,
    leftMiddleDistal,
    leftRingProximal,
    leftRingIntermediate,
    leftRingDistal,
    leftLittleProximal,
    leftLittleIntermediate,
    leftLittleDistal,
    rightThumbProximal,
    rightThumbIntermediate,
    rightThumbDistal,
    rightIndexProximal,
    rightIndexIntermediate,
    rightIndexDistal,
    rightMiddleProximal,
    rightMiddleIntermediate,
    rightMiddleDistal,
    rightRingProximal,
    rightRingIntermediate,
    rightRingDistal,
    rightLittleProximal,
    rightLittleIntermediate,
    rightLittleDistal,
    upperChest,

    unknown,
};

class VRM_Meta : public RefObject
{
public:
    String title;
    String version;
    String author;
    String contactInformation;
    String reference;
    int texture = -1;    // When the value is -1, it means that texture is not specified.
    String allowedUserName;
    String violentUssageName;
    String sexualUssageName;
    String commercialUssageName;
    String otherPermissionUrl;
    String licenseName;
    String otherLicenseUrl;
};

class VRM_HumanoidBone : public RefObject
{
public:
    // Human bone name.
    String bone;

    // Reference node index
    // When the value is -1, it means that no node is found.
    int node = -1;

    // Unity's HumanLimit.useDefaultValues")]
    bool useDefaultValues = true;

    // Unity's HumanLimit.min
    Vector3 min;

    // Unity's HumanLimit.max
    Vector3 max;

    // Unity's HumanLimit.center
    Vector3 center;

    // Unity's HumanLimit.axisLength
    float axisLength;
};

class VRM_Humanoid : public RefObject
{
public:
    List<Ref<VRM_HumanoidBone>> humanBones;
    float armStretch = 0.05f;    // Unity's HumanDescription.armStretch
    float legStretch = 0.05f;    // Unity's HumanDescription.legStretch
    float upperArmTwist = 0.5f;  // Unity's HumanDescription.upperArmTwist
    float lowerArmTwist = 0.5f;  // Unity's HumanDescription.lowerArmTwist
    float upperLegTwist = 0.5f;  // Unity's HumanDescription.upperLegTwist
    float lowerLegTwist = 0.5f;  // Unity's HumanDescription.lowerLegTwist
    float feetSpacing = 0;       // Unity's HumanDescription.feetSpacing
    bool hasTranslationDoF = false;  // Unity's HumanDescription.hasTranslationDoF
};

// vrm.firstperson.meshannotation
class VRM_MeshAnnotation : public RefObject
{
public:
    // When the value is -1, it means that no target mesh is found.
    int mesh;
    String firstPersonFlag;
};

// vrm.firstperson.degreemap
class VRM_DegreeMap : public RefObject
{
public:
    // None linear mapping params. time, value, inTangent, outTangent
    std::vector<float> curve;

    // Look at input clamp range degree.
    float xRange = 90.0f;

    // Look at map range degree from xRange.
    float yRange = 10.0f;
};

// vrm.firstperson
class VRM_Firstperson : public RefObject
{
public:
    // The bone whose rendering should be turned off in first-person view. Usually Head is specified.
    // When the value is -1, it means that no bone for first person is found.
    int firstPersonBone = -1;

    // The target position of the VR headset in first-person view. It is assumed that an offset from the head bone to the VR headset is added. )]
    Vector3 firstPersonBoneOffset;

    // Switch display / undisplay for each mesh in first-person view or the others.")]
    List<Ref<VRM_MeshAnnotation>> meshAnnotations;

    // lookat
    // Eye controller mode.
    String lookAtTypeName = "Bone";

    // Eye controller setting.
    Ref<VRM_DegreeMap> lookAtHorizontalInner = makeRef<VRM_DegreeMap>();

    // Eye controller setting.
    Ref<VRM_DegreeMap> lookAtHorizontalOuter = makeRef<VRM_DegreeMap>();

    // Eye controller setting.
    Ref<VRM_DegreeMap> lookAtVerticalDown = makeRef<VRM_DegreeMap>();

    // Eye controller setting.
    Ref<VRM_DegreeMap> lookAtVerticalUp = makeRef<VRM_DegreeMap>();
};


enum class VRM_BlendShapePreset
{
    Neutral,

    A,
    I,
    U,
    E,
    O,

    Blink,

    // 喜怒哀楽
    Joy,
    Angry,
    Sorrow,
    Fun,

    // LookAt
    LookUp,
    LookDown,
    LookLeft,
    LookRight,

    Blink_L,
    Blink_R,
};

// vrm.blendshape.bind
class VRM_BlendShapeBind : public RefObject
{
public:
    int mesh = -1;
    int index = -1;

    // SkinnedMeshRenderer.SetBlendShapeWeight
    float weight = 0.0f;
};

class VRM_MaterialValueBind : public RefObject
{
public:
    String materialName;
    String propertyName;
    std::vector<float> targetValues;
};

// vrm.blendshape.group
class VRM_BlendShapeGroup : public RefObject
{
public:
    String name;
    String presetName;
    List<Ref<VRM_BlendShapeBind>> binds;
    List<Ref<VRM_MaterialValueBind>> materialValues;
    bool isBinary = false;
};

// vrm.blendshape
class VRM_BlendShapeMaster : public RefObject
{
public:
    List<Ref<VRM_BlendShapeGroup>> blendShapeGroups;
};

// vrm.secondaryanimation.spring
class VRM_SecondaryAnimationGroup : public RefObject
{
public:
    // Annotation comment
    String comment;

    // The resilience of the swaying object (the power of returning to the initial pose).
    float stiffiness;

    // The strength of gravity.
    float gravityPower;

    // The direction of gravity. Set (0, -1, 0) for simulating the gravity. Set (1, 0, 0) for simulating the wind.
    Vector3 gravityDir;

    // The resistance (deceleration) of automatic animation.
    float dragForce;

    // The reference point of a swaying object can be set at any location except the origin. When implementing UI moving with warp, the parent node to move with warp can be specified if you don't want to make the object swaying with warp movement.
    //   NOTE: This value denotes index but may contain -1 as a value.
    //   When the value is -1, it means that center node is not specified.
    //   This is a historical issue and a compromise for forward compatibility.
    int center;

    // The radius of the sphere used for the collision detection with colliders.
    float hitRadius;

    // Specify the node index of the root bone of the swaying object.
    std::vector<int> bones;

    // Specify the index of the collider group for collisions with swaying objects.
    std::vector<int> colliderGroups;
};

class VRM_SecondaryAnimationCollider : public RefObject
{
public:
    // The local coordinate from the node of the collider group.
    Vector3 offset;

    // The radius of the collider.
    float radius;
};

// vrm.secondaryanimation.collidergroup
// Set sphere balls for colliders used for collision detections with swaying objects.
class VRM_SecondaryAnimationColliderGroup : public RefObject
{
public:
    // The node of the collider group for setting up collision detections.
    int node;

    List<Ref<VRM_SecondaryAnimationCollider>> colliders;
};

// vrm.secondaryanimation
// The setting of automatic animation of string-like objects such as tails and hairs.
class VRM_SecondaryAnimation : public RefObject
{
public:
    List<Ref<VRM_SecondaryAnimationGroup>> boneGroups;

    List<Ref<VRM_SecondaryAnimationColliderGroup>> colliderGroups;
};

// vrm.material
class VRM_Material : public RefObject
{
public:
    String name;
    String shader;
    int renderQueue = -1;

    std::unordered_map<String, float> floatProperties;
    std::unordered_map<String, std::vector<float>> vectorProperties;
    std::unordered_map<String, int> textureProperties;
    std::unordered_map<String, bool> keywordMap;
    std::unordered_map<String, String> tagMap;
};

// vrm
// VRM extension is for 3d humanoid avatars (and models) in VR applications.
class VRM_Extensions : public RefObject
{
public:
    // Version of exporter that vrm created.
    String exporterVersion;

    // Version of VRM specification.
    String specVersion;

    Ref<VRM_Meta> meta = makeRef<VRM_Meta>();
    Ref<VRM_Humanoid> humanoid = makeRef<VRM_Humanoid>();
    Ref<VRM_Firstperson> firstPerson = makeRef<VRM_Firstperson>();
    Ref<VRM_BlendShapeMaster> blendShapeMaster = makeRef<VRM_BlendShapeMaster>();
    Ref<VRM_SecondaryAnimation> secondaryAnimation = makeRef<VRM_SecondaryAnimation>();
    List<Ref<VRM_Material>> materialProperties;
};

// https://github.com/vrm-c/vrm-specification
class VRMImporter : public GLTFImporter
{
public:
	VRMImporter();
	bool onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath) override;
    bool onReadMetadata() override;

private:
    List<Ref<VRM_Material>> parseMaterialProperties(const tinygltf::Value& value);
    Ref<VRM_Material> parseMaterial(const tinygltf::Value& value);
    Shader* getShader(const String& name) const;

    Ref<VRM_Extensions> m_vrm;
};

} // namespace detail
} // namespace ln

