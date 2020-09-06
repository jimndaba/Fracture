#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <vector>
#include <memory>

namespace Fracture
{

	class IndexBuffer
	{

	public:
		IndexBuffer(unsigned int* indicies, unsigned int count);
		IndexBuffer(std::vector<unsigned int> indicies, unsigned int count);
		~IndexBuffer();

		void bind();
		void unbind();

		static std::shared_ptr<IndexBuffer> create(std::vector<unsigned int> indicies, unsigned int count);

		unsigned int GetCount();

	private:
		unsigned int m_Id;
		unsigned int m_Count;
	};

}

#endif
