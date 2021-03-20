#pragma once
#ifndef UITEXT_H
#define UITEXT_H

#include "UIResource.h"
#include <string>

namespace Fracture
{
	class UIText
		: public UIResource
	{
	public:
		UIText(const std::string& name):UIResource(name)
		{

		}

		void bind() override{};

		std::string Value;

		static std::shared_ptr<UIText> Make(const std::string& name)
		{
			return std::make_shared<UIText>(name);
		}

	};

}

#endif