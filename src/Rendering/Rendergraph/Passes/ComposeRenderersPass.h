#pragma once
#ifndef COMPOSERENDERERSPASS_H
#define COMPOSERENDERERSPASS_H

#include "IPass.h"


namespace Fracture
{
	struct ComposeRenderersPassDef
	{

	};
	struct ComposeRenderersPass : public IPass
	{
		ComposeRenderersPass(const std::string& name, RenderContext* context, const ComposeRenderersPassDef& info);

		void Setup();
		void Execute();

		ComposeRenderersPassDef Properties;
		uint32_t Vao;
	};

}

#endif