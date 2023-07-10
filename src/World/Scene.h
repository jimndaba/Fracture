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
