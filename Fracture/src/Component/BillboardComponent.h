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

		BillboardComponent(const BillboardComponent& component, const uint32_t& entityID) :Component(entityID, ComponentType::None)
		{
			m_billboard = component.m_billboard;
		}

		virtual void onStart();
		std::shared_ptr<Billboard> GetBillboard();
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<BillboardComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<BillboardComponent>(this->clone_impl(entityID));
		}


	private:

		virtual BillboardComponent* clone_impl(uint32_t entityID) const override
		{
			return new BillboardComponent(*this, entityID);
		}

		std::shared_ptr<Billboard> m_billboard;
	};


}

#endif