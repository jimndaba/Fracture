#pragma once
#ifndef CHROMATICPASS_H
#define CHROMATICPASS_H

#include "IPostProcess.h"

namespace Fracture
{
	struct ChromaticAberrationPass : public IPostProcess
	{
		void Setup();
		bool Render(PostProcessInfo info);
		std::shared_ptr<Shader> shader;
	};

}

#endif