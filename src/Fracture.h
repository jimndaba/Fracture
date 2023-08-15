#pragma once

#define GLM_FORCE_NO_CTOR_INIT 
#include <glm/glm.hpp>
#include "glm/matrix.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include "glm/gtx/compatibility.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Core
#include "Common/Logger.h"
#include "Common/UUID.h"
#include "Common/AppWindow.h"
#include "Common/FractureApp.h"
#include "EventSystem/Eventbus.h"
#include "Physics/PhysicsEvents.h"

//Assets
#include "Assets/AssetManager.h"
#include "Assets/AssetRegistries.h"

//Graphics
#include "Rendering/GraphicsDevice.h"
#include "Rendering/Command.h"
#include "Rendering/IGraphicsResource.h"
#include "Rendering/RenderContext.h"
#include "Rendering/RenderCommands.h"
#include "Rendering/Color.h"
#include "Rendering/DebugRenderer.h"


//Input
#include "Input/Input.h"

//World
#include "World/Scene.h"
#include "World/SceneManager.h"
#include "World/Components.h"
#include "World/CameraSystem.h"
#include "World/TransformSystem.h"
#include "Assets/AssetManager.h"

#include "Serialisation/Serialiser.h"
#include "Serialisation/SceneSerialiser.h"

#include "Physics/PhysicsManager.h"

#include "Audio/AudioManager.h"