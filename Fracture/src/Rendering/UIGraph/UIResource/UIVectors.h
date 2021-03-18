#pragma once
#ifndef UIVECTORS_H
#define UIVECTORS_H

#include "UIResource.h"
#include "glm/glm.hpp"

namespace Fracture
{
	class UIVec2 :public UIResource
	{
	public:
		UIVec2(const std::string& name) :UIResource(name)
		{};

		glm::vec2 Value = glm::vec2(0.0f);

		void bind() override {};

		static std::shared_ptr<UIVec2> Make(const std::string& name)
		{
			return std::make_shared<UIVec2>(name);
		}


	};

	class UIVec3 : public UIResource
	{
	public:
		UIVec3(const std::string& name) :UIResource(name)
		{};

		~ UIVec3() {};

		void bind() override {};

		glm::vec3 Value = glm::vec3(0.0f);

		static std::shared_ptr<UIVec3> Make(const std::string& name)
		{
			return std::make_shared<UIVec3>(name);
		}

	};

	class UIVec4 :public UIResource
	{
	public:
		UIVec4(const std::string& name) :UIResource(name)
		{};

		glm::vec4 Value = glm::vec4(0.0f);

		void bind() override {};

		static std::shared_ptr<UIVec4> Make(const std::string& name)
		{
			return std::make_shared<UIVec4>(name);
		}

	};

}

#endif