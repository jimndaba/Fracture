#pragma once
#ifndef COMPONENTS_H
#define COMPONETNS_H

#include "Entity.h"
#include "physx/PxPhysicsAPI.h"
#include "Scripting/LuaScript.h"
#include "Common/Frustum.h"
#include "BezierSpline.h"

namespace Fracture
{
	struct IComponent
	{
		virtual UUID GetID() = 0;
	};

	struct TagComponent : public IComponent
	{	
		TagComponent() :IComponent() {};
		TagComponent(const UUID& id, const std::string& name = "") :IComponent(), entity(id), Name(name) {}

		TagComponent(TagComponent& other,UUID new_entity) :IComponent()
		{
			entity = new_entity;
			Name = other.Name;
			IsActive = other.IsActive;
			Tags = other.Tags;
		}

		UUID entity;
		std::string Name;
		bool IsActive = true;
		std::vector<std::string> Tags;

		UUID GetID() { return entity; }
	};

	struct TransformComponent : public IComponent
	{
		TransformComponent() :IComponent() {};
		TransformComponent(const UUID& id, glm::vec3 pos = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::quat rot = glm::quat(glm::vec3(0.0f)))
			:IComponent(), Position(pos), Scale(scale), Rotation(rot),entity(id) {}

		TransformComponent(TransformComponent& other, UUID new_entity) :IComponent()
		{
			entity = new_entity;
			Position = other.Position;
			Scale = other.Scale;
			Rotation = other.Rotation;
		}

		UUID entity;
		bool IsDirty = true;
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;

		glm::mat4 LocalTransform = glm::mat4(1.0f);
		glm::mat4 WorldTransform = glm::mat4(1.0f);

		UUID GetID() { return entity; }
	};

	struct MeshComponent : public IComponent
	{
		MeshComponent() :IComponent() {};
		MeshComponent(UUID id,UUID mesh = UUID()):
			IComponent(),entity(id),Mesh(mesh)
		{}

		MeshComponent(UUID id, UUID mesh, std::vector<uint32_t> materials,bool isSkinned = false ) :
			IComponent(), entity(id), Mesh(mesh), Materials(materials)
		{
			meshType = isSkinned ? MeshType::Skinned : MeshType::Static;
		}

		MeshComponent(MeshComponent& other, UUID new_entity):
			IComponent()
		{
			entity = new_entity;
			Mesh = other.Mesh;
			Materials = other.Materials;
			meshType = other.meshType;
		}
		
		UUID entity;
		UUID PrefabID;
		enum class MeshType
		{
			Static,
			Skinned
		};

		UUID Mesh;
		bool IsPrefab = false;
		std::vector<uint32_t> Materials;
		MeshType meshType = MeshType::Static;


		UUID GetID() { return entity; }
	};

	struct PrefabInstanceComponent : public IComponent
	{
		PrefabInstanceComponent() :IComponent() {};
		PrefabInstanceComponent(UUID entity_id, UUID parentprefab_id) :
			IComponent(), Parent_PrefabID(parentprefab_id), EntityID(entity_id)
		{}

		PrefabInstanceComponent(PrefabInstanceComponent& other, UUID new_entity) :
			IComponent()
		{
			EntityID = new_entity;
			Parent_PrefabID = other.Parent_PrefabID;
			Mesh = other.Mesh;
			Materials = other.Materials;
			meshType = other.meshType;
	
		}

		UUID EntityID;
		UUID Parent_PrefabID;
	
		
		enum class MeshType
		{
			Static,
			Skinned
		};

		UUID Mesh;
		std::vector<uint32_t> Materials;
		MeshType meshType = MeshType::Static;
		UUID GetID() { return  EntityID; }
		UUID GetParentPrefabID() { return  Parent_PrefabID; }
	};

	struct HierachyComponent : public IComponent
	{
		HierachyComponent(const UUID& id) :
			IComponent(), entity(id)
		{}

		HierachyComponent(HierachyComponent& other, UUID new_entity)
			:IComponent()
		{
			entity = new_entity;
			HasParent = other.HasParent;
			Parent = other.Parent;
			Children = other.Children;
		}

		UUID entity;
		bool HasParent = false;
		UUID Parent;
		std::vector<UUID> Children;
		std::vector<UUID> Prefabs;

		UUID GetID() { return entity; }
	};

	struct CameraComponent:public IComponent
	{
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		CameraComponent(const UUID& id) :entity(id) {
			CameraFustrum = Frustum::CreateFrustumFromCamera(this);
		}

		CameraComponent(CameraComponent& other, UUID new_entity):
			IComponent()
		{
			entity = new_entity;
			Damping = other.Damping;
			Sensitivity = other.Sensitivity;
			Speed = other.Speed;
			Position = other.Position;
			Front = other.Front;
			Up = other.Up;
			Right = other.Right;
			Yaw = other.Yaw;
			Pitch = other.Pitch;
			Roll = other.Roll;
			Near = other.Near;
			Far = other.Far;
			FocalLength = other.FocalLength;
			FocalRange = other.FocalRange;
			FoV = other.FoV;
			EnableDepthOfField = other.EnableDepthOfField;
			ProjectionMode = other.ProjectionMode;
			CameraFustrum = Frustum::CreateFrustumFromCamera(this);
		}

		UUID entity;

		float Damping = 5.0f;
		float Sensitivity = 0.1f;
		float Speed = 30.0f;
		float SpeedModifier = 0.1f;
		float RotationSpeed = 5.0f;
		float ZoomSpeed = 3.0f;

		glm::vec3 Position = glm::vec3(0, 5, 15.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0);
		float Yaw = 0.0f;
		float Pitch = 0.0f;
		float Roll = 0.0f;
		float FoV = 45.0f;
		float Near = 0.1f;
		float Far = 500.0f;
		float Width = 800;
		float Height = 640;
		float FocalLength = 100.0f;
		float FocalRange = 4.0f;
		float AspectRatio = 1.0f;

		glm::vec3 TargetPosition = glm::vec3(0, 5, 15.0f);
		float TargetYaw = 0.0f;
		float TargetPitch = 0.0f;
		float TargetRoll = 0.0f;
		float TargetFoV = 45.0f;

		glm::mat4 ViewMatrix;
		glm::mat4 ProjectMatrix;
		ProjectionType ProjectionMode = ProjectionType::Perspective;
		Frustum CameraFustrum;

		bool FirstMove = true;
		bool IsDirty = true;
		bool EnableDepthOfField = false;
		bool IsActiveCamera = false;

		virtual UUID GetID() { return entity; };
	};

	struct PointlightComponent : public IComponent
	{
		PointlightComponent(const UUID& id,glm::vec3 color = glm::vec3(1.0f),float radius = 10.0f) :IComponent(), entity(id),Diffuse(color),Radius(radius) {}

		PointlightComponent(PointlightComponent& other, UUID new_entity) :IComponent()
		{
			entity = new_entity;
			Radius = other.Radius;
			Compression = other.Compression;
			Strength = other.Strength;
			Diffuse = other.Diffuse;
		}

		UUID entity;

		float Radius = 1.0f;
		float Compression = 1.0f;
		float Strength = 1.0f;
		glm::vec3 Diffuse = glm::vec3(1.0f);

		UUID GetID() { return entity; }
	};

	struct SpotlightComponent : public IComponent
	{
		SpotlightComponent(const UUID& id) :IComponent(), entity(id) {}

		SpotlightComponent(SpotlightComponent& other, UUID new_entity) :IComponent()
		{
			entity = new_entity;
			Strength = other.Strength;
			InnerCutoff = other.InnerCutoff;
			OutCutoff = other.OutCutoff;
			Linear = other.Linear;
			Quadratic = other.Quadratic;
			Constant = other.Constant;
			Diffuse = other.Diffuse;
		}


		UUID entity;
		float Strength = 1.0f;
		float InnerCutoff = 10.0f;
		float OutCutoff  = 30.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
		float Constant = 1.0f;
		
		glm::vec3 Diffuse = glm::vec3(1.0f);
		
		UUID GetID() { return entity; }
	};

	struct SunlightComponent : public IComponent
	{
		SunlightComponent(const UUID& id) :IComponent(), entity(id) {}
		
		SunlightComponent(SunlightComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			Strength = other.Strength;
			Diffuse = other.Diffuse;
		}

		UUID entity;
		float Strength = 1.0f;
		glm::vec3 Diffuse = glm::vec3(1.0f);

		UUID GetID() { return entity; }
	};

	struct LightProbeComponent : public IComponent
	{
		enum class LightProbeType
		{
			Global,
			Local,
		};

		enum class LightProbeVolume
		{
			Sphere,
			Box,
		};


		LightProbeComponent(const UUID& id) :IComponent(), entity(id) {}
		LightProbeComponent(LightProbeComponent& other, UUID new_entity) :
			IComponent()
		{
			//TO DO LIGHTPROBE COPY
		}

		bool IsBaked = false;
		bool AutoBaked = false;
		UUID entity;
		UUID IrradianceMap;
		UUID PreFilterBRDFMap;
		UUID BRDFLUTMap;

		float Strength = 1.0f;
		glm::vec3 Diffuse = glm::vec3(1.0f);
		bool IsInterior = false;

		int LightProbeResolution = 16;
		int BRDFResolution = 128;
		int BRDFLUTResolution = 512;
		std::vector<glm::vec4> ProbePositions;
		//BOX: XYW, SPHERE:RADIUS X 
		std::vector<glm::vec4> ProbeVolumeDimensions;

		LightProbeType ProbeType = LightProbeType::Global;		
		UUID GetID() { return entity; }
	};

	struct ReflectionProbeComponent : public IComponent
	{
		ReflectionProbeComponent(const UUID& id) :IComponent(), entity(id) {}
		ReflectionProbeComponent(ReflectionProbeComponent& other, UUID new_entity) :
			IComponent()
		{
			//TO DO LIGHTPROBE COPY
		}

		bool IsBaked = false;
		UUID entity;
		UUID ReflectionMap;		
		UUID GetID() { return entity; }

		float Strength = 1.0f;
		int Resolution = 16;	
		float BoundingSphereRadius = 10.0f;
	};

	struct ShadowCasterComponent : public IComponent
	{
		UUID entity;
		UUID GetID() { return entity; }
		int ShadowResolution = 1024;
	};

	enum class CollisionDetectionType { Discrete, Continuous, ContinuousSpeculative };

	struct RigidbodyComponent : public IComponent
	{
		RigidbodyComponent(const Fracture::UUID& id) :IComponent(), entity(id) {}

		RigidbodyComponent(RigidbodyComponent& other, UUID new_entity) :IComponent()
		{
			entity = new_entity;
			Mass = other.Mass;
			Friction = other.Friction;
			Bouncyness = other.Bouncyness;
			AngularDrag = other.AngularDrag;
			LinearDrag = other.AngularDrag;
			IsKinematic = other.IsKinematic;
			IsDynamic = other.IsDynamic;
			for (int i = 0; i < 3; i++)
			{
				LinearConstraints[i] = other.LinearConstraints[i];
			}
			for (int i = 0; i < 3; i++)
			{
				AngularConstraints[i] = other.AngularConstraints[i];
			}
			LinearVelocity = other.LinearVelocity;
			AngularVelocity = other.AngularVelocity;
			
		}


		UUID entity;
		float Mass = 1.0f;
		float Friction = 0.5f;
		float Bouncyness = 0.0f;
		float AngularDrag = 0.0f;
		float LinearDrag = 0.0f;
		bool IsKinematic = false;
		bool IsDynamic = true;
		bool LinearConstraints[3] = {0,0,0};
		bool AngularConstraints[3] = { 0,0,0 };
		glm::vec3 LinearVelocity = glm::vec3(0);
		glm::vec3 AngularVelocity = glm::vec3(0);


		CollisionDetectionType DetectionType = CollisionDetectionType::Discrete;
		physx::PxRigidActor* btBody;

		UUID GetID() { return entity; }
	};

	enum class ColliderType
	{
		Sphere,
		Box,
		Cylinder,
		Cone,
		Capsule,
		ConvexMesh, 
		TriangleMesh
	};

	enum class CCColliderType
	{		
		Box,		
		Capsule		
	};

	struct ColliderComponent : public IComponent
	{
		ColliderComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {}

		ColliderComponent(ColliderComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			Shape = other.Shape;
			Size = other.Size;
			Radius = other.Radius;
			Height = other.Height;
			Offset = other.Offset;
			IsTrigger = other.IsTrigger;
			CollisionLayer = other.CollisionLayer;
			CollisionGroup = other.CollisionGroup;
		}

		UUID entity;		
		UUID GetID() { return entity; }

		ColliderType Shape = ColliderType::Sphere;
		glm::vec3 Size = glm::vec3(1.0f);
		float Radius = 1.0f;
		float Height = 1.0f;
		int CollisionLayer = 0;
		int CollisionGroup = 0;
		glm::vec3 Offset = glm::vec3(0);
		bool IsTrigger = false;
	};

	struct CharacterControllerComponent : public IComponent
	{
		CharacterControllerComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {}

		CharacterControllerComponent(CharacterControllerComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			Shape = other.Shape;
			Size = other.Size;
			Radius = other.Radius;
			Height = other.Height;
			Offset = other.Offset;

			MaxSlopeAngle = other.MaxSlopeAngle;
			StepHeight = other.StepHeight;
			MinMovementDist = other.MinMovementDist;
			MaxSpeed = other.MaxSpeed;
		
			CollisionLayer = other.CollisionLayer;
			CollisionGroup = other.CollisionGroup;
	
		}

		UUID entity;
		UUID GetID() { return entity; }

		CCColliderType Shape = CCColliderType::Box;
		
		glm::vec3 Size = glm::vec3(1.0f);
		glm::vec3 Offset = glm::vec3(0);
		float Radius = 1.0f;
		float Height = 1.0f;

		float MaxSlopeAngle = 30.0;
		float StepHeight  = 0.1f;
		float MinMovementDist = 0.01f;
		float MaxSpeed = 100.0f;

		int CollisionLayer = 0;
		int CollisionGroup = 0;
		bool IsGrounded = false;
			
	};

	struct ScriptComponent : public IComponent
	{
		ScriptComponent(const Fracture::UUID& id, bool ScriptAttached = false) :
			IComponent(), entity(id), HasScriptAttached(ScriptAttached) {}

		ScriptComponent(const Fracture::UUID& id,const Fracture::UUID& script,bool ScriptAttached =true):
			IComponent(), entity(id),Script(script),HasScriptAttached(ScriptAttached) {}

		ScriptComponent(ScriptComponent& other, UUID new_entity):
			IComponent()
		{
			HasScriptAttached = other.HasScriptAttached;
			HasStarted = other.HasStarted;
			Script = other.Script;
			entity = new_entity;
			mProperties = other.mProperties;
		}

		bool HasScriptAttached = false;
		bool HasStarted = false;
		UUID Script;
		UUID entity;

		std::map<std::string,std::shared_ptr<ScriptProperty>> mProperties;

		UUID GetID() { return entity; }
	};

	struct AudioSourceComponent : public IComponent
	{
		AudioSourceComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {}

		AudioSourceComponent(AudioSourceComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			AudioClip = other.AudioClip;
			Volume = other.Volume;
			Pan = other.Pan;
			Mute = other.Mute;
			Looping = other.Looping;
			Is3DSource = other.Is3DSource;
		}

		UUID entity;
		UUID GetID() { return entity; }

		UUID AudioClip;
		float Volume;
		float Pan;
		bool Mute;
		bool Looping;
		bool Is3DSource;
	};

	struct SkyboxComponent : public IComponent
	{
		SkyboxComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {
		
			
		}

		SkyboxComponent(SkyboxComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			SkyTexture = other.SkyTexture;
			IsSkyTextureSet = other.IsSkyTextureSet;
		}

		UUID entity;
		UUID GetID() { return entity; }

		bool IsSkyTextureSet = false;
		bool IsDirty = true;
		bool IsReady = false;
		UUID SkyTexture;		
		glm::vec4 SkyColour = glm::vec4(0.3,0.5,0.8,1.0);
	};


	struct AnimationSyncTrack
	{
		float Duration;
		float FrameTime = 0.0f;
	};

	struct AnimationUniform
	{
		union AnimationUniformValue
		{
			float FLOAT;
			bool BOOL;
			int INT;
			bool TRIGGER;
		};


		enum class AnimationUniformType
		{
			FLOAT,
			BOOL,
			INT,
			TRIGGER
		};

		std::string Name;
		AnimationUniformValue Value;
		AnimationUniformType UniformType;
	};

	struct AnimationComponent : public IComponent
	{
		AnimationComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {
		}

		AnimationComponent(AnimationComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			CurrentGraph = other.CurrentGraph;
			Mesh = other.Mesh;		
			IsGraphSet = other.IsGraphSet;
		}


		UUID entity;
		UUID GetID() { return entity; }
		
		UUID CurrentGraph = -1;
		UUID Mesh = -1;

		bool IsPlaying = true;
		float FrameTime = 0.0f;
		bool IsGraphSet = false;
		std::unordered_map<UUID, AnimationSyncTrack> AnimationTracks;

		std::vector<AnimationUniform> Uniforms;
		
	};

	struct ParticleSystemComponent : public IComponent
	{
		ParticleSystemComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {
		}

		ParticleSystemComponent(ParticleSystemComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			particleFXID = other.particleFXID;
			IsFXAttached = other.IsFXAttached;
		}

		bool IsFXAttached = false;
		UUID entity;
		UUID particleFXID;

		UUID GetID() { return entity; }
	};


	struct TerrainTileSet
	{
		Fracture::UUID DiffuseTexture;
		Fracture::UUID NormalTexture;

		bool HasDiffuse;
		bool HasNormal;
		float Weight = 0;
	};

	struct TerrainComponent : public IComponent
	{
		TerrainComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {
		}

		TerrainComponent(TerrainComponent& other, UUID new_entity) :
			IComponent()
		{
			entity = new_entity;
			TerrainID = other.TerrainID;
			TerrianSizeX = other.TerrianSizeX;
			TerrianSizeY = other.TerrianSizeX;
			HeightMapID = other.HeightMapID;
			MixMapID = other.MixMapID;
			TerrianMaxHeight = other.TerrianMaxHeight;
			TerrianYShift = other.TerrianYShift;
			HasHeightMap = other.HasHeightMap;
			HasMixMap = other.MixMapID;
		}

		UUID entity;
		Fracture::UUID TerrainID;
		Fracture::UUID MaterialID;

		Fracture::UUID HeightMapID;
		Fracture::UUID RoadMapID;
		std::string HeightMapPath;

		Fracture::UUID MixMapID;
		std::string MixMapPath;
		Fracture::UUID IndexMapID;
		Fracture::UUID DiffuseTextureAtlasID;
		Fracture::UUID NormalTextureAtlasID;

		std::vector<UUID> PlacementLayers;
		std::vector<TerrainTileSet> TerrainTextures;

		bool HasHeightMap = false;
		bool HasRoadsMap = false;
		bool HasMixMap = false;
		bool IsMixMapDirty = false;
		bool IsRoadMapDirty = false;
		bool IsAtlasDirty = false;
		bool HasTextureAtlasIndexMap = false;
		bool HasDiffuseTextureAtlas = false;
		bool HasNormalTextureAtlas = false;
		bool HasMaterial = false;
		bool IsGenerated = false;
		bool Dirty = false;
		
		int TerrianSizeX = 256;
		int TerrianSizeY = 256;
		int TerrianResolution = 20;
		float TerrianMaxHeight = 100.0f;
		float TerrianYShift = 0.0f;

		//Texture Params
		int AtlasPerTextureSize = 512;
		int MaxTextures = 8;

		UUID GetID() { return entity; }
	};

	struct SplineComponent : public IComponent
	{

		SplineComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {
		}


		SplineComponent(SplineComponent& other, UUID new_entity) :
			IComponent()
		{
		}

		enum class SplineTypeComponent
		{

		};


		UUID entity;
		CubicBezierSpline Spline;
		float SplineU = 0.0f;

		//Properties
		float Width = 3.0f;
		float Falloff = 0.1f;
		float FalloffWidth = 0.1f;
		//Property bools
		bool UseWidth = false;
		bool UseFalloff = false;

		//Mesh Instancing
		bool IsDirty = false;
		UUID GetID() { return entity; }
	};
}




#endif