#pragma once
#ifndef NodeSerialisers_H
#define NodeSerialisers_H

#include "ComponentSerialiser.h"

namespace Fracture
{
	class TagComponentSerialiser : public ComponentSerialiser
	{
	public:
		TagComponentSerialiser();
		json visitTagComponent(const TagComponent& node)const override;
	};
	
	class RelationShipComponentSerialiser : public ComponentSerialiser
	{
	public:
		RelationShipComponentSerialiser();
		json visitRelationShipComponent(const RelationShipComponent& node)const override;
	};

	class TransformComponentSerialiser : public ComponentSerialiser
	{
	public:
		TransformComponentSerialiser();
		json visitTransformComponent(const TransformComponent& node)const override;
	};

	class CameraControllerComponentSerialiser : public ComponentSerialiser
	{
	public:
		CameraControllerComponentSerialiser();
		json visitCameraControllerComponent(const CameraControllerComponent& node)const override;
	};

	class RigidBodyComponentSerialiser : public ComponentSerialiser
	{
	public:
		RigidBodyComponentSerialiser();
		json visitRigidBodyComponent(const RigidBodyComponent& node)const override;
	};

	class RenderComponentSerialiser : public ComponentSerialiser
	{
	
	public:
		RenderComponentSerialiser();
		json visitRenderComponent(const RenderComponent& node)const override;	
	};

	class ScriptComponentSerialiser : public ComponentSerialiser
	{
	public:
		ScriptComponentSerialiser();
		json visitScriptComponent(const ScriptComponent& node)const override;
		
	};

	class LightComponentSerialiser : public ComponentSerialiser
	{
	public:
		LightComponentSerialiser();		
		json visitLightComponent(const LightComponent& node)const override;		
	};

	class BoxColliderComponentSerialiser : public ComponentSerialiser
	{
	public:
		BoxColliderComponentSerialiser();
		json visitBoxColliderComponent(const BoxColliderComponent& node)const override;

	};

	class BillboardComponentSerialiser : public ComponentSerialiser
	{
	public:
		BillboardComponentSerialiser();
		json visitBillboardComponent(const BillboardComponent& node)const  override;
	};

	class AnimatorComponentSerialiser : public ComponentSerialiser
	{
	public:
		AnimatorComponentSerialiser();
		json visitAnimatorComponent(const AnimatorComponent& node)const override;
	};
}

#endif