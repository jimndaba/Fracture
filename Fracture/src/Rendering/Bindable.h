#pragma once
#ifndef BINDABLE_H
#define BINDABLE_H

namespace Fracture
{

	class Bindable
	{

	public:
		virtual void bind() = 0;
		virtual void Unbind() {};
	};


}

#endif