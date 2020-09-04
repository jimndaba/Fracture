#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

namespace Fracture
{

	class IndexBuffer
	{

	public:
		IndexBuffer(unsigned int* indicies, unsigned int count);
		~IndexBuffer();

		void bind();
		void unbind();

		unsigned int GetCount();

	private:
		unsigned int m_Id;
		unsigned int m_Count;
	};

}

#endif
