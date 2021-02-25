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
		BillboardComponent(UUID id);
		~BillboardComponent();

		BillboardComponent(const BillboardComponent& component, const UUID& entityID) :Component(entityID)
		{
			m_billboard = component.m_billboard;
		}

		virtual void onStart();
		std::shared_ptr<Billboard> GetBillboard();
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<BillboardComponent> clone(UUID id) const
		{
			return std::shared_ptr<BillboardComponent>(this->clone_impl(id));
		}

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

	private:

		virtual BillboardComponent* clone_impl(UUID id) const override
		{
			return new BillboardComponent(*this, id);
		}

		std::shared_ptr<Billboard> m_billboard;
	};


}

#endif