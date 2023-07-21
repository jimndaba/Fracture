#pragma once
#ifndef OUTLINEPASS_H
#define OUTLINEPASS_H

#include "IPass.h"


namespace Fracture
{
	struct VertexArray;

	struct OutlinePassDef
	{
	};


	struct OutlinePass : public IPass
	{
		OutlinePass(const std::string& name, RenderContext* context, const OutlinePassDef& info);

		void Setup();
		void Execute();

		OutlinePassDef definition;

	private:
		uint32_t Vao;
		std::shared_ptr<Shader> mZShader;
		std::shared_ptr<Shader> mOShader;
		
		
	};



}

#endif