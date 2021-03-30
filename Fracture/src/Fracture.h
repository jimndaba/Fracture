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
#include "Rendering/Renderer2D/Renderer2D.h"
#include "AssetManager/AssetManager.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Entity/UUID.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"
#include "Math/Math.h"

//Renderings
#include "Rendering/DrawCommand.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/Model.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/OpenGL/Mesh.h"
#include "Rendering/OpenGL/Vertex.h"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/Texture.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/Texture2DMultiSample.h"
#include "Rendering/OpenGL/TextureCubeMap.h"
#include "Rendering/Material.h"
#include "Rendering/Environment.h"
#include "Rendering/Framegraph/FrameGraph.h"
#include "Rendering/UIGraph/UIGraph.h"

////// Input
#include "Input/MouseState.h"

////Animation
#include "Animation/AnimationManager.h"
#include "Animation/AnimatorProbe.h"
#include "Animation/AnimationClip.h"
#include "Animation/Skeleton.h"

//Scene
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

//Entitie
#include "Entity/Entity.h"
#include "Entity/ILight.h"
#include "Entity/SkyLight.h"
#include "Entity/SunLight.h"
#include "Entity/SpotLight.h"
#include "Entity/PointLight.h"
#include "Entity/Camera2D.h"

//Scripting
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"

//Components
#include "Component/Component.h"
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

//Physcis
#include "Physics/CollisionFilter.h"
#include "Physics/Collisions.h"
#include "Physics/RayHit.h"
#include "Physics/Ray.h"
#include "Physics/PhysicsManager.h"

//Serialisation
#include "Serialisation/SceneSerializer.h"
#include "Serialisation/ProjectSerializer.h"
#include "Serialisation/ProjectProperties.h"
#include "Serialisation/GameSerializer.h"
#include "Serialisation/FrameGraphSerialiser.h"
#include "Serialisation/GameSettings.h"
#include "Serialisation/ProjectProperties.h"

#endif
