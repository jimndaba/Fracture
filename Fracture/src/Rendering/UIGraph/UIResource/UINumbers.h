#pragma once
#ifndef UINUMBERS_H
#define UINUMBERS_H

#include "UIResource.h"

namespace Fracture
{
	class UIInt :public UIResource
	{
	public:
		UIInt(const std::string& name) :UIResource(name)
		{};

		int Value = 0;

		void bind() override {};

		static std::shared_ptr<UIInt> Make(const std::string& name)
		{
			return std::make_shared<UIInt>(name);
		}
	};

	class UIFloat :public UIResource
	{
	public:
		UIFloat(const std::string& name) :UIResource(name)
		{};

		float Value = 0.0f;

		void bind() override {};

		static std::shared_ptr<UIFloat> Make(const std::string& name)
		{
			return std::make_shared<UIFloat>(name);
		}
	};

	class UIDouble :public UIResource
	{
	public:
		UIDouble(const std::string& name) :UIResource(name)
		{};

		double Value = 0.0;

		void bind() override {};

		static std::shared_ptr<UIDouble> Make(const std::string& name)
		{
			return std::make_shared<UIDouble>(name);
		}
	};
}
#endif