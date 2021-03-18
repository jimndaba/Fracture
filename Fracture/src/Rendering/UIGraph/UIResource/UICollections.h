#pragma once
#ifndef UICOLLECTIONS_H
#define UICOLLECTIONS_H

#include "UIResource.h"
#include <vector>


namespace Fracture
{
	template<class T>
	class UIVector :public UIResource
	{
	public:
		UIVector(const std::string& name):UIResource(name)
		{}
		void bind() override {};

		std::vector<T> Value;

	};

}

#endif