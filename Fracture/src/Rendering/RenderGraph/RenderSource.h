#pragma once
#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include <string>
#include <memory>

namespace Fracture
{
	class BufferSource;

	class RenderSource
	{

	public:
		RenderSource(std::string Name);
		~RenderSource();

		void PostLinkValidate();
		std::string& GetName();
		
		std::shared_ptr<BufferSource> YieldBuffer();

	private:
		std::string name;
	};

}

#endif 