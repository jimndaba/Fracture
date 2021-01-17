#pragma once
#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include <string>
#include <memory>
#include "Logging/Logger.h"
#include "Rendering/Bindable.h"

namespace Fracture
{
	class BufferResource;
	class Bindable;

	class RenderSource
	{

	public:
		RenderSource(std::string Name);
		~RenderSource();

		void PostLinkValidate();
		std::string& GetName();
		virtual std::shared_ptr<Bindable> YieldBindable();
		virtual std::shared_ptr<BufferResource> YieldBuffer();

	private:
		std::string name;
	};

	template<class T>
	class DirectBufferSource :public RenderSource
	{
	public:
		DirectBufferSource(std::string name, std::shared_ptr<T>& buffer)
			:
			RenderSource(std::move(name)),
			buffer(buffer)
		{}


		static std::unique_ptr<DirectBufferSource> Make(std::string name, std::shared_ptr<T>& buffer)
		{
			return std::make_unique<DirectBufferSource>(std::move(name), buffer);
		}
		
		void PostLinkValidate() const
		{}
		std::shared_ptr<BufferResource> YieldBuffer() override
		{
			if (linked)
			{
				FRACTURE_ERROR("Mutable output bound twice: " + GetName());
			}
			linked = true;
			return buffer;
		}
	private:
		std::shared_ptr<T>& buffer;
		bool linked = false;
	};

	template<class T>
	class DirectBindableSource : public RenderSource
	{
	public:
		static std::unique_ptr<DirectBindableSource> Make(std::string name, std::shared_ptr<T>& buffer)
		{
			return std::make_unique<DirectBindableSource>(std::move(name), buffer);
		}
		DirectBindableSource(std::string name, std::shared_ptr<T>& bind)
			:
			RenderSource(std::move(name)),
			bind(bind)
		{}
		void PostLinkValidate() const
		{}
		std::shared_ptr<Bindable> YieldBindable() override
		{
			return bind;
		}
	private:
		std::shared_ptr<T>& bind;
	};


}

#endif 