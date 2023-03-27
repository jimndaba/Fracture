#include "FracturePCH.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Mesh.h"

void Fracture::RenderContext::BeginState(const SortKey& key)
{
	KeyStack.push(key);
}

void Fracture::RenderContext::EndState()
{
	KeyStack.pop();
}

void Fracture::RenderContext::Begin()
{
	Renderable_batch.clear();
	const auto& components = SceneManager::GetAllComponents<MeshComponent>();
	for (const auto& meshcomponent : components)
	{
		if (AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		{
			const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());
			// [Shader][Mesh]
			Renderable_batch[meshcomponent->Shader][meshcomponent->Mesh].push_back(transform->WorldTransform);
		}
	}

	for (auto& batch : Renderable_batch)
	{
		if (batch.second.empty())
			continue;
		for (auto entity : batch.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(entity.first);		
			Fracture::RenderCommands::MapDataTobuffer<glm::mat4>(this, mesh->Matrix_Buffer->RenderID, entity.second, entity.second.size() * sizeof(glm::mat4));	
		}
	}
}

void Fracture::RenderContext::Push(Fracture::Command& cmd)
{
	if (KeyStack.empty())
		return;

	cmd.Key = KeyStack.top();
	Commands.push_back(cmd);
}

void Fracture::RenderContext::Sort(DepthSortOrder order)
{
	switch (order)
	{
		case DepthSortOrder::Front_To_Back:
		{
			std::sort(Commands.begin(), Commands.end(), [](Command a, Command b) {return a.Key < b.Key; });
			break;
		}
		case DepthSortOrder::Back_To_Front:
		{
			std::sort(Commands.begin(), Commands.end(), [](Command a, Command b) {return a.Key > b.Key; });
			break;
		}
	}
}

void Fracture::RenderContext::Render()
{
	for (const auto& cmd : Commands)
	{
		cmd.fnc();
		GraphicsDevice::Instance()->CHECKGLERRRORS();
	}

	Commands.clear();
}
