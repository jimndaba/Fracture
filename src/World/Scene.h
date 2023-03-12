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
		std::vector<std::shared_ptr<Entity>> Entities;
		std::unordered_map<std::type_index, ComponentSet> ComponentReg;
		std::shared_ptr<SceneNode> RootNode;
		std::string Name = "Untitled";
	};
}

#endif