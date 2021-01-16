#pragma once
#ifndef BUFFERRESOURCE_H
#define BUFFERRESOURCE_H

namespace Fracture
{
	class Renderer;
	class BufferResource
	{
	public:
		virtual ~BufferResource() = default;
		virtual void BindAsBuffer(Renderer&) = 0;
		virtual void BindAsBuffer(Renderer&, BufferResource*) = 0;
		virtual void Clear(Renderer&) = 0;
	};
}

#endif