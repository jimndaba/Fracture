#pragma once
#ifndef SCENESERIALISER
#define SCENESERIALISER

#include "Serialiser.h"
#include "World/SceneManager.h"

namespace Fracture
{
	struct Scene;
	struct ScenePrefab;
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
	struct AudioSourceComponent;
	struct PrefabInstanceComponent;

	struct PrefabCreationInfo
	{
		std::vector<Fracture::UUID> Entities;
		std::vector<Fracture::UUID> Prefabs;
		std::string Name;
		UUID SceneID;
	};

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
		void SerialiseComponent(Fracture::AudioSourceComponent* component);
		void SerialiseComponent(Fracture::SkyboxComponent* component);
		void SerialiseComponent(Fracture::LightProbeComponent* component);
		void SerialiseComponent(Fracture::AnimationComponent* component);
		void SerialiseComponent(Fracture::CharacterControllerComponent* component);

		void ReadTagComponentIfExists(Fracture::UUID entity_id);
		void ReadTransformComponentIfExists(Fracture::UUID entity_id);
		void ReadTransformComponentIfExists(Fracture::UUID entity_id,glm::vec3 Position, glm::vec3 scale,glm::quat Rotation);
		void ReadHierachyComponentIfExists(Fracture::UUID entity_id);
		void ReadHierachyComponentIfExists(Fracture::UUID entity_id, Fracture::UUID new_parent);
		void ReadMeshComponentIfExists(Fracture::UUID entity_id, bool isPrefab = false, Fracture::UUID prefab_id = uint32_t(0));
		void InstanceMeshComponentIfExists(Fracture::UUID entity_id, Fracture::UUID prefab_id = uint32_t(0), Fracture::UUID scene_id = uint32_t(0));
		void ReadSpotlightComponentIfExists(Fracture::UUID entity_id);
		void ReadPointlightComponentIfExists(Fracture::UUID entity_id);
		void ReadSunlightComponentIfExists(Fracture::UUID entity_id);
		void ReadRigidbodyComponentIfExists(Fracture::UUID entity_id);
		void ReadColliderComponentIfExists(Fracture::UUID entity_id);
		void ReadScriptComponentIfExists(Fracture::UUID entity_id);
		void ReadCameraComponentIfExists(Fracture::UUID entity_id);
		void ReadAudioSourceComponentIfExists(Fracture::UUID entity_id);
		void ReadSkyboxComponentIfExists(Fracture::UUID entity_id);
		void ReadLightProbeComponentIfExists(Fracture::UUID entity_id);
		void ReadAnimationComponentIfExists(Fracture::UUID entity_id);
		void ReadCharacterControllerComponentIfExists(Fracture::UUID entity_id);

		void WriteScene(Scene* scene);
		void EntitiesToPrefab(PrefabCreationInfo info);
		void WriteEntityToPrefab(Fracture::UUID& parent,Fracture::UUID& entity);

		std::shared_ptr<Scene> ReadScene();
		std::shared_ptr<Scene> ReadSceneWithoutLoad();
		void ReadScenePrefab(ScenePrefab prefab_id);
		
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