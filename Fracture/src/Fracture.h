#pragma once
#ifndef FRACTURE_H
#define FRACTURE_H


#include <memory>
#include <vector>
#include <math.h>

#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"
#include "Event/Event.h"
#include "Event/WindowEvents.h"

#include "Component/ComponentManager.h"
#include "Component/ComponentSet.h"
#include "Game/GameWindow.h"
#include "Game/Game.h"
#include "Rendering/Renderer.h"
#include "AssetManager/AssetManager.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Entity/IDManager.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"
#include "Math/Math.h"

#include "Rendering/DrawCommand.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/OpenGL/Vertex.h"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/Texture.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/Texture2DMultiSample.h"
#include "Rendering/OpenGL/TextureCubeMap.h"
#include "Rendering/Material.h"
#include "Rendering/Environment.h"
#include "Rendering/Framegraph/FrameGraph.h"

////// Input
#include "Input/MouseState.h"

////Animation
#include "Animation/AnimationManager.h"
#include "Animation/AnimatorProbe.h"
#include "Animation/AnimationClip.h"
#include "Animation/Skeleton.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


#include "Entity/Entity.h"
#include "Entity/IDManager.h"
#include "Entity/ILight.h"
#include "Entity/SkyLight.h"
#include "Entity/SunLight.h"
#include "Entity/SpotLight.h"
#include "Entity/PointLight.h"
#include "Scripting/GameLogic.h"

#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/ICamera.h"
#include "Component/ScriptComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/LightComponent.h"
#include "Component/AnimatorComponent.h"

#include "Physics/CollisionFilter.h"
#include "Physics/Collisions.h"
#include "Physics/RayHit.h"
#include "Physics/Ray.h"
#include "Physics/PhysicsManager.h"

#include "Scripting/ScriptManager.h"
#include "Scripting/GameLogic.h"

#include "Serialisation/SceneSerializer.h"
#include "Serialisation/ProjectSerializer.h"
#include "Serialisation/ProjectProperties.h"
#include "Serialisation/GameSerializer.h"
#include "Serialisation/GameSettings.h"



#endif
