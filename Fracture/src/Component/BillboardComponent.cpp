#include "BillboardComponent.h"
#include "Rendering/Billboard.h"

Fracture::BillboardComponent::BillboardComponent(uint32_t id):Component(id,ComponentType::Render)
{
	m_billboard = std::make_shared<Billboard>();
}

Fracture::BillboardComponent::~BillboardComponent()
{
}

void Fracture::BillboardComponent::onStart()
{
}

std::shared_ptr<Fracture::Billboard> Fracture::BillboardComponent::GetBillboard()
{
	return m_billboard;
}

void Fracture::BillboardComponent::Accept(ISceneProbe* visitor)
{
	visitor->VisitBillboardComponent(this);
}
