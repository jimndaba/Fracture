#pragma once
#ifndef BINDPASS_H
#define BINDPASS_H

#include "RenderPass.h"
#include "Rendering/RenderTarget.h"

namespace Fracture
{

	class BindingPass : public RenderPass
	{

	public:
		BindingPass(std::string name);
		void Execute(Renderer& renderer) override;
		//void AddBind();
		void BindBufferResources(Renderer& renderer);
		void BindAll(Renderer& renderer);
		void Finalise() override;

		template<class T>
		void AddBindSink(std::string name)
		{
			
		}
		std::shared_ptr<RenderTarget> renderTarget;

	};

}

#endif