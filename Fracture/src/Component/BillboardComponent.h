#pragma once
#ifndef BILLBOARDCOMPONENT_H
#define BILLBOARDCOMPONENT_H

#include "Component/Component.h"
#include <memory>
#include "glm/common.hpp"

namespace Fracture
{

	class Billboard;

	class BillboardComponent : public Component
	{

	public:
		BillboardComponent(uint32_t id);
		~BillboardComponent();

		virtual void onStart();
		std::shared_ptr<Billboard> GetBillboard();

	private:

		std::shared_ptr<Billboard> m_billboard;
	};


}

#endif