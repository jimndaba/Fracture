#pragma once
#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <string>
#include "../UINode.h"
#include "UIDimension.h"
namespace Fracture
{

	template<class T, typename ...Args>
	class UIFunction;

	class UIWindow : public UINode
	{
	public:
		UIWindow(const std::string name);

		void execute(Renderer& renderer) override;

		UIDimension Dimensions;
		//UIMultiSocket Submit;
		//UIMultiSocket Submit2D;

	private:


	};


}

#endif