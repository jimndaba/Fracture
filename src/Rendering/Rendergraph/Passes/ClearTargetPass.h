#pragma once
#ifndef CLEARTARGETPASS_H
#define CLEARTARGETPASS_H

#include "IPass.h"


namespace Fracture
{
	enum class ClearTargetType
	{
		Color,
		Depth,
		Stencil,
		All
	};

	struct ClearTargetSockets
	{
		static std::string InBuffer;
		static std::string OutBuffer;
	};


	struct ClearTargetPassDef
	{
		uint32_t BufferTarget;
		Fracture::Colour ClearColor = Fracture::Colour::Red;
	};


	struct ClearTargetPass : public IPass
	{
		ClearTargetPass(const std::string& name,RenderContext* context, const ClearTargetPassDef& info);
		
		void Setup();
		void Execute();

		ClearTargetPassDef Properties;
	};



}

#endif