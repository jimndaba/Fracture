#pragma once
#ifndef PRESENTPASS_H
#define PRESENTPASS_H

#include "IPass.h"

namespace Fracture
{
	struct VertexArray;

	struct PresentPassDef
	{
	};


	struct PresentPass : public IPass
	{
		PresentPass(const std::string& name,RenderContext* context, const PresentPassDef& info);

		void Setup();
		void Execute();

		PresentPassDef definition;

	private:
		uint32_t Vao;
	};



}

#endif