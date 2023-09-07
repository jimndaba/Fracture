#pragma once
#ifndef PREFABFACTORY_H
#define PREFABFACTORY_H

#include "Scene.h"

namespace Fracture
{


	class PrefabFactory
	{
	public:
		PrefabFactory();

		void Instance(ScenePrefab prefab, glm::vec3 position = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::vec3 rotation = glm::vec3(0),bool isRootOfNested = false);

		void LoadPrefabTemplate(const std::string& path);
		//void CreatePrefab();

		bool IsTemplateAvailable(Fracture::UUID sceneID);

		std::map<Fracture::UUID, ScenePrefabTemplate> mPrefabTemplates;
		std::map<Fracture::UUID, bool> mPrefabTracker;
		std::vector<Fracture::UUID> MeshesToLoad;
	};








}

#endif