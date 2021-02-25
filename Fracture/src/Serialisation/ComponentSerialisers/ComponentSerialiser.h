#pragma once
#ifndef COMPONENTSERIALISER_H
#define COMPONENTSERIALISER_H

#include <memory>
#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;
	class TagComponent;
	class RelationShipComponent;
	class TransformComponent;
	class CameraControllerComponent;
	class RigidBodyComponent;
	class RenderComponent;
	class ScriptComponent;
	class LightComponent;
	class BoxColliderComponent;
	class BillboardComponent;
	class AnimatorComponent;

	class ComponentSerialiser
	{
	public:
		ComponentSerialiser()
		{};
		~ComponentSerialiser() = default;
		virtual json visitTagComponent(const TagComponent& node)const { json j; return j; };
		virtual json visitRelationShipComponent(const RelationShipComponent& node)const { json j; return j; };
		virtual json visitTransformComponent(const TransformComponent& node)const { json j; return j; };
		virtual json visitCameraControllerComponent(const CameraControllerComponent& node)const { json j; return j; };
		virtual json visitRigidBodyComponent(const RigidBodyComponent& node)const { json j; return j; };
		virtual json visitRenderComponent(const RenderComponent& node)const { json j; return j; };
		virtual json visitScriptComponent(const ScriptComponent& node)const { json j; return j; };
		virtual json visitLightComponent(const LightComponent& node)const { json j; return j; };
		virtual json visitBoxColliderComponent(const BoxColliderComponent& node)const { json j; return j; };
		virtual json visitBillboardComponent(const BillboardComponent& node)const { json j; return j; };
		virtual json visitAnimatorComponent(const AnimatorComponent& node)const { json j; return j; };
	private:
	};

}

#endif