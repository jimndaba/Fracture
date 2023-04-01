#pragma once
#ifndef SCENESERIALISER
#define SCENESERIALISER

#include "Serialiser.h"
#include "World/SceneManager.h"

namespace Fracture
{
	struct Scene;
	struct TagComponent;
	struct TransformComponent;
	struct HierachyComponent;
	struct MeshComponent;
	struct CameraComponent;
	struct PointlightComponent;
	struct SpotlightComponent;
	struct SunlightComponent;
	struct ShadowCasterComponent;
	struct RigidbodyComponent;
	struct ColliderComponent;
	struct ScriptComponent;

	struct SceneSerialiser : public ISerialiser
	{
		SceneSerialiser(IOMode mode, SerialiseFormat format);

		void SerialiseComponent(Fracture::TagComponent* component);
		void SerialiseComponent(Fracture::TransformComponent* component);
		void SerialiseComponent(Fracture::HierachyComponent* component);
		void SerialiseComponent(Fracture::MeshComponent* component);
		void SerialiseComponent(Fracture::CameraComponent* component);
		void SerialiseComponent(Fracture::PointlightComponent* component);
		void SerialiseComponent(Fracture::SpotlightComponent* component);
		void SerialiseComponent(Fracture::SunlightComponent* component);
		void SerialiseComponent(Fracture::ShadowCasterComponent* component);
		void SerialiseComponent(Fracture::RigidbodyComponent* component);
		void SerialiseComponent(Fracture::ColliderComponent* component);
		void SerialiseComponent(Fracture::ScriptComponent* component);

		void ReadTagComponentIfExists(Fracture::UUID entity_id);
		void ReadTransformComponentIfExists(Fracture::UUID entity_id);
		void ReadHierachyComponentIfExists(Fracture::UUID entity_id);
		void ReadMeshComponentIfExists(Fracture::UUID entity_id);
		void ReadSpotlightComponentIfExists(Fracture::UUID entity_id);
		void ReadPointlightComponentIfExists(Fracture::UUID entity_id);
		void ReadSunlightComponentIfExists(Fracture::UUID entity_id);
		void ReadRigidbodyComponentIfExists(Fracture::UUID entity_id);
		void ReadColliderComponentIfExists(Fracture::UUID entity_id);
		void ReadScriptComponentIfExists(Fracture::UUID entity_id);

		void WriteScene(Scene* scene);
		std::shared_ptr<Scene> ReadScene();

		std::map<Fracture::UUID,int> MeshesToLoad;

		template <class T>
		void WriteEntityComponentOfType(const UUID& entity);
	};
		
	template<class T>
	inline void SceneSerialiser::WriteEntityComponentOfType(const UUID& entity)
	{
		if (SceneManager::HasComponent<T>(entity))
		{
			const auto& component = SceneManager::GetComponent<T>(entity);
			SerialiseComponent(component.get());
		}
	}

}

#endif