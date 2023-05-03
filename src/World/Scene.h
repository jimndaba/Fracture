#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "Components.h"
#include "Scenegraph.h"


namespace Fracture
{
	typedef std::unordered_map<UUID, std::shared_ptr<IComponent>> ComponentSet;

	struct Scene
	{
		UUID ID;
		UUID RootID;
		UUID ActiveCameraID;
		
		std::vector<std::shared_ptr<Entity>> Entities;
		std::unordered_map<std::type_index, ComponentSet> ComponentReg;
		std::unordered_map<UUID, std::vector<std::shared_ptr<ScriptComponent>>> mScriptReg;
		std::string Name = "Untitled";
	};
}

#endif