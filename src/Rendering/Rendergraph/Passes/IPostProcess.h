#pragma once
#ifndef IPOSTPROCESS_H
#define IPOSTPROCESS_H

#include "IPass.h"

namespace Fracture
{
	struct RenderContext;
	struct Shader;

	struct PostProcessInfo
	{
		RenderContext* cntxt;
		uint32_t write_to;
		uint32_t input_texture;
		uint32_t VAO;
	};

	struct IPostProcess
	{
		virtual void Setup() = 0;
		virtual bool Render(PostProcessInfo info) = 0;
	};


}

#endif