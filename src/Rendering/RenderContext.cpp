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
	EntityIndeces.clear();

	const auto& components = SceneManager::GetAllComponents<MeshComponent>();

	for (const auto& meshcomponent : components)
	{
		if (AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		{
			const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());
			// [Shader][Mesh]
			Renderable_batch[meshcomponent->Shader][meshcomponent->Mesh].push_back(transform->WorldTransform);

			glm::vec4 color(0);

			//color.r = (meshcomponent->GetID() >> 0)  / 255.0f;
			//color.g = (meshcomponent->GetID() >> 8) / 255.0f;
			//color.b = (meshcomponent->GetID() >> 16) / 255.0f;

			uint32_t id = meshcomponent->GetID();
			//r = (id & 0x000000FF) >> 0;
			//g = (id & 0x0000FF00) >> 8;
			//b = (id & 0x00FF0000) >> 16;
			//a = 255;

			uint8_t r = (id >> 24) & 0xFF; // Red component
			uint8_t g = (id >> 16) & 0xFF; // Green component
			uint8_t b = (id >> 8) & 0xFF;  // Blue component
			uint8_t a = id & 0xFF;         // Alpha component


			color.r = (float)r / 255.0f;
			color.g = (float)g / 255.0f;
			color.b = (float)b / 255.0f;
			color.a = (float)a / 255.0f;

			EntityIndeces[meshcomponent->Mesh].push_back(color);
		
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

			Fracture::RenderCommands::MapDataTobuffer<glm::vec4>(this, mesh->EntityID_Buffer->RenderID, EntityIndeces[entity.first], EntityIndeces[entity.first].size() * sizeof(glm::vec4));

		
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
