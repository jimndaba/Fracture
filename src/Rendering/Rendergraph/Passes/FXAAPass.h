#pragma once
#ifndef FXAAPASS_H
#define FXAAPASS_H

#include "IPostProcess.h"

namespace Fracture
{
	struct FXAAPass : public IPostProcess
	{
		void Setup();
		void Render(PostProcessInfo info);
		std::shared_ptr<Shader> shader;
	};

}

#endif