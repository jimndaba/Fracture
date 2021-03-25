#pragma once
#ifndef PROCEDURALSKY_H
#define PROCEDURALSKY_H

#include "Environment.h"

namespace Fracture
{
	class RenderTarget;
	class FrameBuffer;
	class Texture2D;
	class TextureCubeMap;
	class Model;
	class Shader;
	

	class DynamicEnvironment :public Environment
	{

	public:
		DynamicEnvironment(const std::string& name, std::shared_ptr<Shader> mshader);
		~DynamicEnvironment() = default;

		static std::shared_ptr<DynamicEnvironment> Create(const std::string& name, std::shared_ptr<Shader> mshader);

	private:		
		std::shared_ptr<Shader> mshader;		
	};


}

#endif