#pragma once
#ifndef EDITORNODE_H
#define EDITORNODE_H

#include "Component/Component.h"
#include "glm/glm.hpp"

namespace Fracture
{

	class EditorNode : public Component
	{

	public:
		EditorNode(uint32_t id);
		~EditorNode();

		virtual void onStart();

		void SetPosition(glm::vec3 value);
		void SetRotation(glm::vec3 value);
		void SetScale(glm::vec3 value);

		glm::vec3 GetPosition();
		glm::vec3 GetRotation();
		glm::vec3 GetScale();

	private:

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}

#endif