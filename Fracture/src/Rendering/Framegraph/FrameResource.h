#pragma once
#ifndef FRAMERESOURCE_H
#define FRAMERESOURCE_H

namespace Fracture
{
	class Texture;

	class FrameResource
	{

	public:
		FrameResource() {};
		virtual void bind() = 0;
		virtual void Unbind() {};
		virtual void Resize(unsigned int width, unsigned int height) {};
		virtual std::shared_ptr<Texture> GetDepthStencilTexture() { return nullptr; };
		virtual std::shared_ptr<Texture> GetColorTexture(unsigned int index) { return nullptr; };
	private:
	};

}

#endif