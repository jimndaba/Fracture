#pragma once
#ifndef COMPONENTS_H
#define COMPONETNS_H

#include "Entity.h"

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

		UUID entity;
		std::string Name;
		bool IsActive = true;
		std::vector<std::string> Tags;

		UUID GetID() { return entity; }
	};

	struct TransformComponent : public IComponent
	{
		TransformComponent() :IComponent() {};
		TransformComponent(const UUID& id, glm::vec3 pos = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::quat rot = glm::quat(0,0,0,0))
			:IComponent(), Position(pos), Scale(scale), Rotation(rot),entity(id) {}

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
		MeshComponent(UUID id, UUID mesh,UUID shader) :
			IComponent(), entity(id), Mesh(mesh),Shader(shader)
		{}
		
		UUID entity;
		enum class MeshType
		{
			Static,
			Skinned
		};
		UUID Mesh;
		UUID Material;
		UUID Shader;
		MeshType meshType = MeshType::Static;


		UUID GetID() { return entity; }
	};

	struct HierachyComponent : public IComponent
	{
		HierachyComponent(const UUID& id) :
			IComponent(), entity(id)
		{}
		UUID entity;
		bool HasParent = false;
		UUID Parent;
		std::vector<UUID> Children;

		UUID GetID() { return entity; }
	};

	struct CameraComponent:public IComponent
	{
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		CameraComponent(const UUID& id) :entity(id) {}
		UUID entity;

		float Damping = 5.0f;
		float Sensitivity = 0.1f;
		float Speed = 30.0f;
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

		glm::vec3 TargetPosition = glm::vec3(0, 5, 15.0f);
		float TargetYaw = 0.0f;
		float TargetPitch = 0.0f;
		float TargetRoll = 0.0f;
		float TargetFoV = 45.0f;

		glm::mat4 ViewMatrix;
		glm::mat4 ProjectMatrix;
		ProjectionType ProjectionMode = ProjectionType::Perspective;

		bool FirstMove = true;
		bool IsDirty = true;
		bool EnableDepthOfField = false;

		virtual UUID GetID() { return entity; };
	};

	struct PointlightComponent : public IComponent
	{
		PointlightComponent(const UUID& id,glm::vec3 color = glm::vec3(1.0f),float radius = 10.0f) :IComponent(), entity(id),Diffuse(color),Radius(radius) {}

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
		UUID entity;

		float Strength = 1.0f;
		glm::vec3 Diffuse = glm::vec3(1.0f);

		UUID GetID() { return entity; }
	};

	struct ShadowCasterComponent : public IComponent
	{
		UUID entity;
		UUID GetID() { return entity; }
	};

	struct RigidbodyComponent : public IComponent
	{
		RigidbodyComponent(const Fracture::UUID& id) :IComponent(), entity(id) {}
		UUID entity;
		UUID GetID() { return entity; }

		float Mass = 1.0f;
		float Friction = 0.5f;
		float Bouncyness = 0.0f;
		bool IsDynamic = false;
		bool LinearConstraints[3] = {0,0,0};
		bool AngularConstraints[3] = { 0,0,0 };
	};

	enum class ColliderType
	{
		Sphere,
		Box,
		Cylinder,
		Cone,
		Capsule
	};

	struct ColliderComponent : public IComponent
	{
		ColliderComponent(const Fracture::UUID& id) :
			IComponent(), entity(id) {}

		UUID entity;
		UUID GetID() { return entity; }

		ColliderType Shape = ColliderType::Sphere;
		glm::vec3 Size = glm::vec3(1.0f);
		float Radius = 1.0f;
		float Height = 1.0f;
	};


}



#endif