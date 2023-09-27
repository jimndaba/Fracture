#pragma once

#include "EventSystem/Event.h"

#include "Scene.h"

namespace Fracture
{
	struct InstanceScenePrefabEvent : public Fracture::Event
	{
		InstanceScenePrefabEvent(ScenePrefab Prefab) : Event(), prefab(Prefab) {};

		ScenePrefab prefab;
		const char* Name() { return "InstanceScenePrefabEvent"; };
	};

	struct InstantiatePrefabEvent : public Fracture::Event
	{
		InstantiatePrefabEvent(Fracture::UUID id, glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0)) : Event(), SceneID(id), Position(pos), Rotation(rot) {};

		Fracture::UUID SceneID;
		glm::vec3 Position;
		glm::vec3 Rotation;
		const char* Name() { return "InstanceScenePrefabEvent"; };
	};

	struct DestroyEntityEvent : public Fracture::Event
	{
		DestroyEntityEvent(Fracture::UUID id) : Event(), ID(id){}
		Fracture::UUID ID;
		const char* Name() { return "DestroyEntityEvent"; };
	};

}