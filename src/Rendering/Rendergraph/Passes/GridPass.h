#pragma once
#ifndef GRIDPASS_H
#define GRIDPASS_H

#include "IPass.h"

namespace Fracture
{

	struct GridPassDef
	{
	};


	struct GridPass : public IPass
	{
		GridPass(const std::string& name, RenderContext* context, const GridPassDef& info);

		void Setup();
		void Execute();

		GridPassDef params;

		uint32_t Vao;
	};



}

#endif