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
		virtual unsigned int GetID() { return -1; };
		virtual void Resize(unsigned int width, unsigned int height) {};
		
		virtual void SetMultiSampled(bool value) { };
		virtual void SetResizable(bool value) {};
		virtual bool IsMultiSampled() { return false; };
		virtual bool IsResizable() { return true; };
		virtual void blit(unsigned int fbo){ };
		virtual std::shared_ptr<Texture> GetDepthStencilTexture() { return nullptr; };
		virtual std::shared_ptr<Texture> GetColorTexture(unsigned int index) { return nullptr; };
	private:
	};

}

#endif