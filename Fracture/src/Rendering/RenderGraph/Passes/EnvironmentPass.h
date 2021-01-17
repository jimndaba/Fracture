#pragma once
#ifndef ENVIRONMENTPASS_H
#define ENVIRONMENTPASS_H

#include "BindingPass.h"
#include "Rendering/Renderer.h"
#include "../DirectBufferSink.h"
#include "../RenderSource.h"

namespace Fracture
{

	class EnvironmentPass : public BindingPass
	{
	public:
		EnvironmentPass(std::string name) :BindingPass(name)
		{
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", renderTarget));
		}

		virtual void Execute(Renderer& renderer) override
		{
			renderer.RenderEnvironment();
		}

	};

}

#endif 