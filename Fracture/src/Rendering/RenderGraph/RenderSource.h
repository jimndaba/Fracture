#pragma once
#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include <string>
#include <memory>
#include "Logging/Logger.h"

namespace Fracture
{
	class BufferResource;

	class RenderSource
	{

	public:
		RenderSource(std::string Name);
		~RenderSource();

		void PostLinkValidate();
		std::string& GetName();
		
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


}

#endif 