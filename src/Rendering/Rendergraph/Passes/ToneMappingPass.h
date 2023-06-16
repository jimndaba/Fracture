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
		void Render(PostProcessInfo info);
		std::shared_ptr<Shader> shader;
	};

}

#endif