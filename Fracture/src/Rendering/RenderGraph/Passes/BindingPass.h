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
		BindingPass(std::string name, std::vector<std::shared_ptr<Bindable>> binds = {});
		void Execute(Renderer& renderer) override;
		void AddBind(std::shared_ptr<Bindable> bindable);		
		void BindAll(Renderer& renderer);
		void UnbindAll(Renderer& renderer);
		void Finalise() override;

		template<class T>
		void AddBindSink(std::string name)
		{
			const auto index = m_bindables.size();
			m_bindables.emplace_back();
			//RegisterSink(std::make_unique<ContainerBindableSink<T>>(std::move(name), binds, index));
		}
		std::shared_ptr<RenderTarget> renderTarget;
		

	private:
		void BindBufferResources(Renderer& renderer);
		void UnbindBufferResources(Renderer& renderer);
		std::vector<std::shared_ptr<Bindable>> m_bindables;
	};

}

#endif