#pragma once
#ifndef UILOOP_H
#define UILOOP_H

#include "../UINode.h"

namespace Fracture
{
	class UISocket;
	class UIVec3;

	template<class T, typename ...Args>
	class UIFunction;

	template<typename ...Args>
	class UIForLoopNode : public UINode
	{
	public:
		UIForLoopNode(const std::string Name,const std::shared_ptr<UIFunction<Args...>>& func);

		void execute(Renderer& renderer) override;	
		void execute(Renderer& renderer, Args... params);

		UISocket InFunction;		

		uint32_t index = 0;
		uint32_t range = 10;

	private:

		std::shared_ptr<UIFunction<UIForLoopNode,Args...>> function;

	};

	template<typename ...Args>
	inline UIForLoopNode<Args...>::UIForLoopNode(const std::string Name, const std::shared_ptr<UIFunction<Args...>>& func):
		UINode(Name),
		InFunction(UISocket::Make("InFunction")),
		function(func)
	{
		AddInputSocket(InFunction);
		AddInputResource(InFunction, func);
	}	

	template<typename ...Args>
	inline void UIForLoopNode<Args...>::execute(Renderer& renderer)
	{
	}


	template<typename ...Args>
	inline void UIForLoopNode<Args...>::execute(Renderer& renderer, Args... params)
	{
		for (index; index < range; index++)
		{
			(*function->Value)(renderer,params...);
		}
	}

}

#endif