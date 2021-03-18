#pragma once
#ifndef UIRESOURCE_H
#define UIRESOURCE_H

#include <string>

namespace Fracture
{
	class UIResource
	{
	public:
		UIResource(const std::string& Name): m_name(Name)
		{};

		virtual void bind() = 0;

		
		virtual void submit(Renderer& renderer) {};
		
		std::string GetName() const
		{
			return m_name;
		}

	private:
		std::string m_name;
	};

	
}

#endif