#pragma once
#ifndef LAMBERTIANPASS_H
#define LAMBERTIANPASS_H

#include "RenderQueuePass.h"
#include "Rendering/RenderBucket.h"
#include "../DirectBufferSink.h"
#include "../RenderSource.h"

#include <string>

namespace Fracture
{

	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass(std::string Name, RenderBucket* opaque, RenderBucket* transparent):RenderQueuePass(Name)
		{
			AcceptBucket(opaque);
			AcceptBucket(transparent);
			
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", renderTarget));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", renderTarget));

			//AddBind(renderTarget);

		}

		void Execute(Renderer& renderer)
		{
			RenderQueuePass::Execute(renderer);
		}
	};


}

#endif