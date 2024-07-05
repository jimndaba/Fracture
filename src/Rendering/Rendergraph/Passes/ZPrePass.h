#pragma once
#ifndef ZPREPASS_H
#define ZPREPASS_H

#include "IPass.h"


namespace Fracture
{
	struct ZPrePassDef
	{
	};


	struct ZPrePass : public IPass
	{
		ZPrePass(const std::string& name, RenderContext* context, const ZPrePassDef& info);

		void Setup();
		void Execute();

		ZPrePassDef Properties;
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Shader> mShaderInstanced;
		std::shared_ptr<Shader> mShaderSkinned;
	};



}

#endif