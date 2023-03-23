#pragma once
#ifndef FORWARDPASS_H
#define FORWARDPASS_H

#include "IPass.h"

namespace Fracture
{

	struct ForwardPassDef
	{		
	};


	struct ForwardPass : public IPass
	{
		ForwardPass(const std::string& name,RenderContext* context,const ForwardPassDef& info);

		void Setup();

		void Execute();

		void PickPass();

		float SpecularLevel = 16.0f;

		ForwardPassDef definition;

		std::map<Fracture::UUID, std::map<Fracture::UUID, std::vector<glm::mat4>>> Renderable_batch;
	};



}

#endif