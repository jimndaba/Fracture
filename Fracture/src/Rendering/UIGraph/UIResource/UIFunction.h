#pragma once
#ifndef UIFUNCTION_H
#define  UIFUNCTION_H

#include "UIResource.h"
#include "Logging/Logger.h"

namespace Fracture
{
	class Renderer;		
	class Renderer2D;

	template<class T,typename ...Args>
	class UIFunction :public UIResource
	{
	public:		
		UIFunction(T* instance, const std::string& name,void(T::*func)(Renderer& renderer, Args... params)) :UIResource(name),Value(func),m_instance(instance)
		{}
		
		UIFunction(T* instance, const std::string& name, void(T::* func)(Renderer2D& renderer, Args... params)) :UIResource(name), Value2D(func), m_instance(instance)
		{}

		~UIFunction() {};

		void bind() override {};
		void submit(Renderer& renderer, Args... params) override {			
			(m_instance->*Value)(renderer, params...);
		};
		void submit2D(Renderer2D& renderer, Args... params) override {
			(m_instance->*Value2D)(renderer, params...);
		};
		//Function Value;
		void(T::*Value)(Renderer& renderer,Args... params);		
		void(T::* Value2D)(Renderer2D& renderer, Args... params);

		static std::shared_ptr<UIFunction>Make(T* instance, const std::string& name , void(T::*func)(Renderer& renderer,Args... params)) {
			return std::make_shared<UIFunction>(instance,name,func);
		}

		static std::shared_ptr<UIFunction>Make2D(T* instance, const std::string& name, void(T::* func)(Renderer2D& renderer, Args... params)) {
			return std::make_shared<UIFunction>(instance, name, func);
		}

		T* m_instance;
	};
}

#endif