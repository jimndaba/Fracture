#include "FracturePCH.h"
#include "TransformSystem.h"
#include "Components.h"
#include "SceneManager.h"

Fracture::TransformSystem::TransformSystem()
{
}

void Fracture::TransformSystem::Update(Fracture::UUID root)
{
	const auto& entity = SceneManager::GetComponent<HierachyComponent>(root);
	const auto& entity_transform = SceneManager::GetComponent<TransformComponent>(root);

	if (!entity_transform || !entity) return; 

	if(entity_transform->IsDirty)
	{ 
		glm::mat4 mTransform = glm::mat4(1.0f);
		glm::mat4 m_rotation, m_translation, m_scale;
		m_translation = glm::translate(entity_transform->Position);
		m_scale = glm::scale(entity_transform->Scale);
		m_rotation = glm::toMat4(glm::normalize(entity_transform->Rotation));
		entity_transform->LocalTransform = m_translation * m_rotation * m_scale;
	}

	if (entity->HasParent)
	{
		const auto& parent_transform = SceneManager::GetComponent<TransformComponent>(entity->Parent);
		entity_transform->WorldTransform = parent_transform->WorldTransform * entity_transform->LocalTransform;
	}
	else
	{
		entity_transform->WorldTransform = entity_transform->LocalTransform;
	}

	for (auto child : entity->Children)
		Update(child);

}

void  Fracture::TransformSystem::UpdatePrefabs(Fracture::UUID root)
{
	const auto& entity_transform = SceneManager::GetComponent<TransformComponent>(root);
	const auto& entity = SceneManager::GetComponent<HierachyComponent>(root);

	if (!entity_transform || !entity) return;

	if (entity_transform->IsDirty)
	{
		glm::mat4 mTransform = glm::mat4(1.0f);
		glm::mat4 m_rotation, m_translation, m_scale;
		m_translation = glm::translate(entity_transform->Position);
		m_scale = glm::scale(entity_transform->Scale);
		m_rotation = glm::toMat4(glm::normalize(entity_transform->Rotation));
		entity_transform->LocalTransform = m_translation * m_rotation * m_scale;
	}

	const auto& parent_transform = SceneManager::GetComponent<TransformComponent>(SceneManager::CurrentScene()->RootID);
	entity_transform->WorldTransform = parent_transform->WorldTransform * entity_transform->LocalTransform;


	for (auto child : entity->Prefabs)
		UpdatePrefab(child);
}

void Fracture::TransformSystem::UpdatePrefab(Fracture::UUID root)
{
	const auto& entity = SceneManager::GetComponent<HierachyComponent>(root);
	const auto& entity_transform = SceneManager::GetComponent<TransformComponent>(root);

	if (!entity_transform || !entity) return;

	if (entity_transform->IsDirty)
	{
		glm::mat4 mTransform = glm::mat4(1.0f);
		glm::mat4 m_rotation, m_translation, m_scale;
		m_translation = glm::translate(entity_transform->Position);
		m_scale = glm::scale(entity_transform->Scale);
		m_rotation = glm::toMat4(glm::normalize(entity_transform->Rotation));
		entity_transform->LocalTransform = m_translation * m_rotation * m_scale;
	}

	if (entity->HasParent)
	{
		const auto& parent_transform = SceneManager::GetComponent<TransformComponent>(entity->Parent);
		entity_transform->WorldTransform = parent_transform->WorldTransform * entity_transform->LocalTransform;
	}
	else
	{
		entity_transform->WorldTransform = entity_transform->LocalTransform;
	}

	for (auto child : entity->Prefabs)
		UpdatePrefab(child);

}

void Fracture::TransformSystem::Translate(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value)
{
	transform->Position += value;
	transform->IsDirty = true;
}

void Fracture::TransformSystem::LookAt(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value, const glm::vec3& up)
{
	if ((value - transform->Position) == glm::vec3(0, 0, 0)) return;

	glm::vec3  direction = value - transform->Position;
	float      directionLength = glm::length(direction);

	// Normalize direction
	direction /= directionLength;

	// Is the normal up (nearly) parallel to direction?
	if (glm::abs(glm::dot(direction, up)) > .9999f) {
		// Use alternative up
		transform->Rotation = glm::quatLookAt(direction, up);
	}
	else {
		transform->Rotation = glm::quatLookAt(direction, up);
	}

	transform->IsDirty = true;
}

void Fracture::TransformSystem::SetPosition(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value)
{
	transform->Position = value;
	transform->IsDirty = true;
}
void Fracture::TransformSystem::SetScale(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value)
{
	transform->Scale = value;
	transform->IsDirty = true;
}
void Fracture::TransformSystem::SetRotation(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value)
{
	transform->Rotation = glm::quat(value);
	transform->IsDirty = true;
}


