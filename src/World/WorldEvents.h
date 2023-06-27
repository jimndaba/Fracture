#pragma once

#include "EventSystem/Event.h"


namespace Fracture
{
	struct InstanceScenePrefabEvent : public Fracture::Event
	{
		InstanceScenePrefabEvent(Fracture::UUID id, glm::vec3 pos = glm::vec3(0)) : Event(), SceneID(id),Position(pos) {};

		Fracture::UUID SceneID;
		glm::vec3 Position;
		const char* Name() { return "InstanceScenePrefabEvent"; };
	};


}