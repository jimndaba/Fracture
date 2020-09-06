#pragma once
#ifndef RENDERPASS_H
#define RENDERPASS_H

namespace Fracture {

	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

		virtual void bind()=0;
		virtual void unbind()=0;
		virtual void render()=0;

	};

}

#endif