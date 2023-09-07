#include "FracturePCH.h"
#include "LuaMathBindings.h"

void LuaBindGLM::BindVec2(sol::state* lua)
{
	lua->new_usertype<glm::vec2>("vec2",
		// Constructor 
		sol::constructors <
		glm::vec2(),
		glm::vec2(float value),
		glm::vec2(float x, float y)
		>(),

		//Members
		"x", &glm::vec2::x,
		"y", &glm::vec2::y,

		//Methods
		"Length", &glm::vec2::length,
		"Normalize", [](glm::vec2& v) { v = glm::normalize(v); return v; },
		"Distance", [](glm::vec2& v1, glm::vec2& v2) { return glm::distance(v1, v2); },



		//Metamethods
		sol::meta_function::to_string, [](glm::vec2& v) {return fmt::format("vec2({}, {})", v.x, v.y); },
		sol::meta_function::index, [](glm::vec2& v, const int index) { if (index < 0 or index > 2) return 0.0f; return v[index]; },
		sol::meta_function::new_index, [](glm::vec2& v, const int index, double x) { if (index < 0 or index > 2) return; v[index] = x; },
		sol::meta_function::equal_to, [](glm::vec2& v1, glm::vec2& v2) { return glm::operator==<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::subtraction, [](glm::vec2& v1, glm::vec2& v2) { return glm::operator-<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::addition, [](glm::vec2& v1, glm::vec2& v2) { return glm::operator+<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::division, [](glm::vec2& v1, glm::vec2& v2) { return glm::operator/<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::multiplication, [](glm::vec2& v1, glm::vec2& v2) { return glm::operator*<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::unary_minus, [](glm::vec2& v1) { return glm::operator-<float, glm::packed_highp>(v1);  }
	);

	//Constructors
	//lua->set_function("vec2", []() {return glm::vec2(); });
	//lua->set_function("vec2", sol::overload([](float value) {return glm::vec2(value); }));
	//lua->set_function("vec2", sol::overload([](float x, float y) {return glm::vec2(x, y); }));
}

void LuaBindGLM::BindVec3(sol::state* lua)
{
	auto mult_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
		[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
		[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
	);

	lua->new_usertype<glm::vec3>("vec3",
		sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,

		"Length", &glm::vec3::length,
		"Normalize", [](glm::vec3& v) { v = glm::normalize(v); return v; },
		"Distance", [](glm::vec3& v1, glm::vec3& v2) { return glm::distance(v1, v2); },
		"Dot", [](glm::vec3& v1, glm::vec3& v2) { return glm::dot(v1, v2); },
		"Cross", [](glm::vec3& v1, glm::vec3& v2){return glm::cross(v1,v2);},

		"Forward", [](glm::vec3& rotation) {		

			glm::mat4 rotationMatrix(1.0f);

			// Rotate around the X-axis
			rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

			// Rotate around the Y-axis
			rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

			// Rotate around the Z-axis
			rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			// The forward vector in local space is the negative Z-axis
			glm::vec3 localForward = glm::vec3(0.0f, 0.0f, -1.0f);

			// Multiply the local forward vector by the rotation matrix to transform it
			glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(localForward, 0.0f));


			return  glm::normalize(rotatedForward);
		},

		sol::meta_function::equal_to, [](glm::vec3& v1, glm::vec3& v2) { return glm::operator==<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::subtraction, [](glm::vec3& v1, glm::vec3& v2) { return glm::operator-<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::addition, [](glm::vec3& v1, glm::vec3& v2) { return glm::operator+<float, glm::packed_highp>(v1, v2);  },
		//sol::meta_function::division, [](glm::quat& v1, glm::quat& v2) { return glm::operator/<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::multiplication, [](glm::vec3& v1, glm::vec3& v2) { return glm::operator*<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::multiplication, [](glm::vec3& v1, float v2) { return glm::operator*<float, glm::packed_highp>(v1, v2);  }
		);

}

void LuaBindGLM::BindVec4(sol::state* lua)
{
	lua->new_usertype<glm::vec4>("vec4",
		// Constructor 
		sol::constructors <
		glm::vec4(),
		glm::vec4(float value),
		glm::vec4(float x, float y, float z, float w)
		>(),

		//Members
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w
	);
}

void LuaBindGLM::BindQuat(sol::state* lua)
{
	glm::quat q; 
	
	lua->new_usertype<glm::quat>("quat",
		// Constructor 
		sol::constructors <
		glm::quat(),
		glm::quat(float x, float y,float z,float w),
		glm::quat(glm::vec3 v)
		>(),

		//Members
		"x", &glm::quat::x,
		"y", &glm::quat::y,
		"z", &glm::quat::z,
		"w", &glm::quat::w,

		//Methods
		"Length", &glm::quat::length,
		"Normalize", [](glm::quat& v) { v = glm::normalize(v); return v; },
		
		

		//Metamethods
		sol::meta_function::to_string, [](glm::quat& v) {return fmt::format("quat({}, {},{},{})", v.x, v.y, v.z, v.w); },
		sol::meta_function::index, [](glm::quat& v, const int index) { if (index < 0 or index > 2) return 0.0f; return v[index]; },
		sol::meta_function::new_index, [](glm::quat& v, const int index, double x) { if (index < 0 or index > 2) return; v[index] = x; },
		sol::meta_function::equal_to, [](glm::quat& v1, glm::quat& v2) { return glm::operator==<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::subtraction, [](glm::quat& v1, glm::quat& v2) { return glm::operator-<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::addition, [](glm::quat& v1, glm::quat& v2) { return glm::operator+<float, glm::packed_highp>(v1, v2);  },
		//sol::meta_function::division, [](glm::quat& v1, glm::quat& v2) { return glm::operator/<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::multiplication, [](glm::quat& v1, glm::quat& v2) { return glm::operator*<float, glm::packed_highp>(v1, v2);  },
		sol::meta_function::unary_minus, [](glm::quat& v1) { return glm::operator-<float, glm::packed_highp>(v1);  }
	);

	//Constructors
	//lua.set_function("quat", []() {return glm::vec2(); });
	//lua.set_function("quat", sol::overload([](glm::vec3 value) {return glm::quat(value); }));
	//lua.set_function("quat", sol::overload([](float x, float y,float z,float w) {return glm::quat(x, y,z,w); }));
}

void LuaBindGLM::BindMat3(sol::state* lua)
{
}

void LuaBindGLM::BindMat4(sol::state* lua)
{
}

