#pragma once
#ifndef DIRECTBUFFERSINK_H
#define DIRECTBUFFERSINK_H

#include"Logging/Logger.h"

#include "RenderSink.h"
#include "RenderSource.h"
#include <memory>
#include <type_traits>
#include <sstream>
#include <cctype>
#include <typeinfo>
#include "../RenderTarget.h"

namespace Fracture
{

	template<class T>
	class DirectBufferSink : public RenderSink
	{
	public:
		DirectBufferSink(std::string registeredName, std::shared_ptr<T>& bind):RenderSink(std::move(registeredName)),
			target(bind)
		{}

		void PostLinkValidate() override
		{
			if (!linked)
			{
				FRACTURE_ERROR("Unlinked input: " + GetRegisteredName());
			}
		}

		static std::unique_ptr<RenderSink> Make(std::string registeredName, std::shared_ptr<T>& target)
		{
			return std::make_unique<DirectBufferSink>(std::move(registeredName), target);
		}

		virtual void Bind(RenderSource& source)
		{
			auto p = std::dynamic_pointer_cast<T>(source.YieldBuffer());
			if (!p)
			{
				std::ostringstream oss;
				oss << "Binding input [" << GetRegisteredName() << "] to output [" << GetTargetPassName() << "." << GetSourceName() << "] "
					<< " { " << typeid(T).name() << " } not compatible with { " << typeid(*source.YieldBuffer().get()).name() << " }";			
			}
			target = std::move(p);
			linked = true;

		}
	private:
		std::shared_ptr<T>& target;
		bool linked = false;
	};

}

#endif