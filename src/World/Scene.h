#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "Components.h"
#include "Scenegraph.h"


namespace Fracture
{
	typedef std::unordered_map<UUID, std::shared_ptr<IComponent>> ComponentSet;

	struct ScenePrefab
	{
		UUID PrefabID;
		UUID SceneID;
		UUID ParentID;
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;

		inline bool operator == (const ScenePrefab& other) const {
			return SceneID == other.SceneID;
		}
	};
	
	struct EntityPrefabTemplate
	{
		EntityPrefabTemplate() {};

		EntityPrefabTemplate(const EntityPrefabTemplate& other)
		{
			TempalteID = other.TempalteID;
			SceneID = other.SceneID;
			ParentID = other.ParentID;

			HasTranformComponent = other.HasTranformComponent;
			HasTagComponent = other.HasTagComponent;
			HasMeshComponent = other.HasMeshComponent;
			HasHierachyComponent = other.HasHierachyComponent;
			HasCameraComponent = other.HasCameraComponent;
			HasSunlightComponent = other.HasSunlightComponent;
			HasPointlightComponent = other.HasPointlightComponent;
			HasSpotlightComponent = other.HasSpotlightComponent;
			HaslightProbeComponent = other.HaslightProbeComponent;
			HasReflectionProbeComponent = other.HasReflectionProbeComponent;
			HasRigidbodyComponent = other.HasRigidbodyComponent;
			HasColliderComponent = other.HasColliderComponent;
			HasScriptComponent = other.HasScriptComponent;
			HasAudioSourceComponent = other.HasAudioSourceComponent;
			HasSkyboxComponent = other.HasSkyboxComponent;
			HasAnimationComponent = other.HasAnimationComponent;

			scripts = other.scripts;

			if (other.animation)
				animation = other.animation;

			if (other.audiosource)
				audiosource = other.audiosource;

			if (other.camera)
				camera = other.camera;

			if (other.collider)
				collider = other.collider;

			if (other.hierachy)
				hierachy = other.hierachy;

			if (other.lightprobe)
				lightprobe = other.lightprobe;

			if (other.pointlight)
				pointlight = other.pointlight;

			if (other.reflectionprobe)
				reflectionprobe = other.reflectionprobe;

			if (other.mesh_component)
				mesh_component = other.mesh_component;

			if (other.spotlight)
				spotlight = other.spotlight;

			if (other.sunlight)
				sunlight = other.sunlight;

			if (other.rigidbody)
				rigidbody = other.rigidbody;

			if (other.skybox)
				skybox = other.skybox;

			if (other.transform)
				transform = other.transform;

			if (other.script)
				script = other.script;

			if (other.tag)
				tag = other.tag;
		}

		EntityPrefabTemplate operator = (const EntityPrefabTemplate& other)
		{
			TempalteID = other.TempalteID;
			SceneID = other.SceneID;
			ParentID = other.ParentID;

			HasTranformComponent = other.HasTranformComponent;
			HasTagComponent = other.HasTagComponent;
			HasMeshComponent = other.HasMeshComponent;
			HasHierachyComponent = other.HasHierachyComponent;
			HasCameraComponent = other.HasCameraComponent;
			HasSunlightComponent = other.HasSunlightComponent;
			HasPointlightComponent = other.HasPointlightComponent;
			HasSpotlightComponent = other.HasSpotlightComponent;
			HaslightProbeComponent = other.HaslightProbeComponent;
			HasReflectionProbeComponent = other.HasReflectionProbeComponent;
			HasRigidbodyComponent = other.HasRigidbodyComponent;
			HasColliderComponent = other.HasColliderComponent;
			HasScriptComponent = other.HasScriptComponent;
			HasAudioSourceComponent = other.HasAudioSourceComponent;
			HasSkyboxComponent = other.HasSkyboxComponent;
			HasAnimationComponent = other.HasAnimationComponent;

			scripts = other.scripts;

			if (other.animation)
				animation = other.animation;

			if (other.audiosource)
				audiosource = other.audiosource;

			if (other.camera)
				camera = other.camera;

			if (other.collider)
				collider = other.collider;

			if (other.hierachy)
				hierachy = other.hierachy;

			if (other.lightprobe)
				lightprobe = other.lightprobe;

			if (other.pointlight)
				pointlight = other.pointlight;

			if (other.reflectionprobe)
				reflectionprobe = other.reflectionprobe;

			if (other.mesh_component)
				mesh_component = other.mesh_component;

			if (other.spotlight)
				spotlight = other.spotlight;

			if (other.sunlight)
				sunlight = other.sunlight;

			if (other.rigidbody)
				rigidbody = other.rigidbody;

			if (other.skybox)
				skybox = other.skybox;

			if (other.transform)
				transform = other.transform;

			if (other.script)
				script = other.script;

			if (other.tag)
				tag = other.tag;
			return *this;
		}

	

		Fracture::UUID TempalteID;
		Fracture::UUID SceneID;
		Fracture::UUID ParentID;

		bool HasTranformComponent = false;
		bool HasTagComponent = false;
		bool HasMeshComponent = false;
		bool HasHierachyComponent = false;
		bool HasCameraComponent = false;
		bool HasSunlightComponent = false;
		bool HasPointlightComponent = false;
		bool HasSpotlightComponent = false;
		bool HaslightProbeComponent = false;
		bool HasReflectionProbeComponent = false;
		bool HasRigidbodyComponent = false;
		bool HasColliderComponent = false;
		bool HasScriptComponent = false;
		bool HasAudioSourceComponent = false;
		bool HasSkyboxComponent = false;
		bool HasAnimationComponent = false;

		std::shared_ptr<TagComponent> tag = nullptr;
		std::shared_ptr<TransformComponent> transform = nullptr;
		std::shared_ptr<PrefabInstanceComponent> mesh_component = nullptr;
		std::shared_ptr<HierachyComponent> hierachy = nullptr;
		std::shared_ptr<CameraComponent> camera = nullptr;
		std::shared_ptr<PointlightComponent> pointlight = nullptr;
		std::shared_ptr<SpotlightComponent> spotlight = nullptr;
		std::shared_ptr<SunlightComponent> sunlight = nullptr;
		std::shared_ptr<LightProbeComponent> lightprobe = nullptr;
		std::shared_ptr<ReflectionProbeComponent> reflectionprobe = nullptr;
		std::shared_ptr<ColliderComponent> collider = nullptr;
		std::shared_ptr<RigidbodyComponent> rigidbody = nullptr;
		std::shared_ptr<ScriptComponent> script = nullptr;
		std::shared_ptr<AudioSourceComponent> audiosource = nullptr;
		std::shared_ptr<SkyboxComponent> skybox = nullptr;
		std::shared_ptr<AnimationComponent> animation = nullptr;

		std::vector<Fracture::UUID> scripts;


	};

	struct ScenePrefabTemplate
	{
		Fracture::UUID TempalteID;
		Fracture::UUID SceneID;
		Fracture::UUID ParentID;
		Fracture::UUID RootID;

		std::vector<int> mEntityIndex;
		std::map<int, EntityPrefabTemplate> mEntities;
		std::vector<ScenePrefab> mPrefablist;
	};

	struct Scene
	{
		UUID ID;
		UUID RootID;
		UUID ActiveCameraID;		
		std::vector<std::shared_ptr<Entity>> Entities;
		std::unordered_map<std::type_index, ComponentSet> ComponentReg;
		std::unordered_map<Fracture::UUID, std::vector<std::shared_ptr<ScriptComponent>>> mScriptReg;
		std::vector<Fracture::ScenePrefab> mPrefabs;
		std::string Name = "Untitled";				
	};
}

#endif
