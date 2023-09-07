#pragma once
#ifndef TONEMAPPASS_H
#define TONEMAPPASS_H

#include "IPostProcess.h"

namespace Fracture
{
	struct Shader;

	struct ToneMappingPass : public IPostProcess
	{
		void Setup();
		bool Render(PostProcessInfo info);
		std::shared_ptr<Shader> shader;
	};

}

#endif