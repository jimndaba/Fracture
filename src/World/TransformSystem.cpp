#include "FracturePCH.h"
#include "TransformSystem.h"
#include "Components.h"

Fracture::TransformSystem::TransformSystem()
{
}

void Fracture::TransformSystem::Update(Fracture::SceneManager* manager, Fracture::UUID root)
{
	const auto& entity = manager->GetComponent<HierachyComponent>(root);
	const auto& entity_transform = manager->GetComponent<TransformComponent>(root);

	if (!entity_transform || !entity) return; 

	if(entity_transform->IsDirty)
	{ 
		glm::mat4 mTransform = glm::mat4(1.0f);
		glm::mat4 m_rotation, m_translation, m_scale;
		m_translation = glm::translate(entity_transform->Position);
		m_scale = glm::scale(entity_transform->Scale);
		m_rotation = glm::toMat4(entity_transform->Rotation);
		entity_transform->LocalTransform = m_translation * m_rotation * m_scale;
	}

	if (entity->HasParent)
	{
		const auto& parent_transform = manager->GetComponent<TransformComponent>(entity->Parent);
		entity_transform->WorldTransform = parent_transform->WorldTransform * entity_transform->LocalTransform;
	}
	else
	{
		entity_transform->WorldTransform = entity_transform->LocalTransform;
	}

	for (auto child : entity->Children)
		Update(manager, child);

}


