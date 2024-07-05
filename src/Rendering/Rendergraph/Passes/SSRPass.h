#pragma once
#ifndef SSRPASS_H
#define SSRPASS_H


#include "IPass.h"

namespace Fracture
{

	struct SSRPass : public IPass
	{
		SSRPass(const std::string& name, RenderContext* context);

		void Setup();
		void Execute();

		
		uint32_t Vao;
		std::shared_ptr<Shader>  mSSR_Shader;
	};



}
#endif