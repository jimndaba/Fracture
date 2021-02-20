#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <memory>

namespace Fracture
{

	class IndexBuffer
	{
	public:
		IndexBuffer(void* data, uint32_t size);
		~IndexBuffer();

		void SetData(void* buffer, uint32_t offset, uint32_t size);
		void bind() const;

		static std::shared_ptr<IndexBuffer> Create(void* data, uint32_t size);

	private:
		uint32_t m_RenderID;

	};

}

#endif