#pragma once
#ifndef FRACTURE_H
#define FRACTURE_H


#include <memory>
#include <vector>
#include <math.h>
#include "SDL2/SDL.h"
#include "GLAD/glad.h"

#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"
#include "Event/Event.h"
#include "Event/WindowEvents.h"

#include "Game/Game.h"
#include "Game/GameWindow.h"
#include "Rendering/Renderer.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Entity/IDManager.h"

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"

#include "Rendering/RenderCommand.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Vertex.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Entity/Entity.h"
#include "Entity/IDManager.h"
#include "Scripting/GameLogic.h"

#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/ScriptComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/LightComponent.h"
#include "Component/EditorNodeComponent.h"

#include "Physics/CollisionFilter.h"
#include "Physics/Collisions.h"
#include "Physics/RayHit.h"
#include "Physics/Ray.h"

#include "Scripting/GameLogic.h"

#include "Serialisation/SceneSerializer.h"

#endif
