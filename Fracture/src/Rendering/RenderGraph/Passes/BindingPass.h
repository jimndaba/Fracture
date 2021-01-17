#pragma once
#ifndef BINDPASS_H
#define BINDPASS_H

#include "RenderPass.h"
#include "Rendering/RenderTarget.h"

namespace Fracture
{
	class Bindable;
	class BindingPass : public RenderPass
	{

	public:
		BindingPass(std::string name);
		void Execute(Renderer& renderer) override;
		void AddBind(std::shared_ptr<Bindable> bindable);
		void BindBufferResources(Renderer& renderer);
		void BindAll(Renderer& renderer);
		void Finalise() override;

		template<class T>
		void AddBindSink(std::string name)
		{
			
		}
		std::shared_ptr<RenderTarget> renderTarget;
		std::vector<std::shared_ptr<Bindable>> m_bindables;
	};

}

#endif