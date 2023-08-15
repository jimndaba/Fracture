#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <istream>
#include <sstream>
#include <ostream>
#include <functional>
#include <utility>
#include <algorithm>
#include <cstdint>
#include <random>
#include <chrono>
#include <type_traits>
#include <typeindex>
#include <stack>
#include <queue>



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

#include <Common/UUID.h>
#include "Common/Logger.h"



#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"


#include "glad/glad.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



