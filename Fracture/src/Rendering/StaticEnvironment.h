#pragma once
#ifndef STATICENVIRONMENT_H
#define STATICENVIRONMENT_H

#include "Environment.h"


namespace Fracture
{

	class RenderTarget;
	class FrameBuffer;
	class Texture2D;
	class TextureCubeMap;
	class Model;
	class Shader;

	class StaticEnvironment : public Environment
	{

	public: 
		StaticEnvironment(const std::string& name,std::shared_ptr<Texture2D> texture, const std::shared_ptr<Shader>& rendershader);
		~StaticEnvironment();
				
		static std::shared_ptr<StaticEnvironment> Create(const std::string& name,std::shared_ptr<Texture2D> texture, std::shared_ptr<Shader> shader);
	};
}

#endif