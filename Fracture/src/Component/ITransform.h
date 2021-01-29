#pragma once
#ifndef ITRANSFORM_H
#define ITRANSFORM_H

#include "glm/glm.hpp"

namespace Fracture
{

	class ITransform
	{
	public:
		virtual glm::mat4 GetLocalTranform() = 0;
		virtual glm::mat4 GetWorldTransform() = 0;
	};


}

#endif 