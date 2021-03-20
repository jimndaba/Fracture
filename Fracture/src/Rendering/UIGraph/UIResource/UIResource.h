#pragma once
#ifndef UIRESOURCE_H
#define UIRESOURCE_H

#include <string>
#include <memory>


namespace Fracture
{
	class Renderer2D;

	class UIResource
	{
	public:
		UIResource(const std::string& Name): m_name(Name)
		{};

		virtual void bind() = 0;

		
		virtual void submit(Renderer& renderer) {};
		virtual void submit2D(Renderer2D& renderer) {};
		
		std::string GetName() const
		{
			return m_name;
		}

	private:
		std::string m_name;
	};

	
}

#endif