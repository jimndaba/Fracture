#pragma once
#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include <glad/glad.h>

namespace Fracture
{
	
	enum class CullMode
	{
		None = GL_NONE,
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK
	};

	enum class FillMode
	{
		Line = GL_LINE,
		Fill = GL_FILL
	};

	enum class PrimitiveType
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineLoops = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN
	};

	enum class DepthFunc
	{
		Less = GL_LESS,
		Equal = GL_EQUAL,
		Never = GL_NEVER,
		LEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		GEqual = GL_GEQUAL,
		Always = GL_ALWAYS
	};

	enum class BlendFunc
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SrcColor = GL_SRC_COLOR,
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		DstColor = GL_DST_COLOR,
		OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
		SrcAlpha = GL_SRC_ALPHA,
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha = GL_DST_ALPHA,
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
		ConstantColor = GL_CONSTANT_COLOR,
		OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
		ConstantAlpha = GL_CONSTANT_ALPHA,
		OneMinusConstantAlpha =GL_ONE_MINUS_CONSTANT_ALPHA,
		AlphaSaturate = GL_SRC_ALPHA_SATURATE,
		Src1Color = GL_SRC1_COLOR,
		OneMinusSrc1Color = GL_ONE_MINUS_SRC1_COLOR,
		Src1Alpha = GL_SRC1_ALPHA,
		OneMinusSrc1Alpha = GL_ONE_MINUS_SRC1_ALPHA
	};

	enum class StencilOp_TestResult
	{
		Keep = GL_KEEP,
		Zero = GL_ZERO,
		Replace = GL_REPLACE,
		Increase = GL_INCR,
		IncreaseWrap = GL_INCR_WRAP,
		Decrease = GL_DECR,
		DecreaseWrap = GL_DECR_WRAP,
		Invert = GL_INVERT
	};

	enum class StencilFunc
	{
		Less = GL_LESS,
		Equal = GL_EQUAL,
		Never = GL_NEVER,
		LEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		GEqual = GL_GEQUAL,
		Always = GL_ALWAYS
	};

	struct PipelineStateDesc
	{
		std::string Name = "Default";
		CullMode cullMode = CullMode::Back;
		FillMode fillMode = FillMode::Fill;
		DepthFunc depthFunction = DepthFunc::Less;	
		BlendFunc blendFunction_sFactor = BlendFunc::Zero;
		BlendFunc blendFunction_dFactor = BlendFunc::Zero;
		StencilOp_TestResult StFail = StencilOp_TestResult::Keep;
		StencilOp_TestResult StPass_DtFail = StencilOp_TestResult::Keep;
		StencilOp_TestResult StPass_DtPass = StencilOp_TestResult::Keep;
		StencilFunc stencilFunction = StencilFunc::Always;
		uint32_t StencilBitMask = 0xFF;
		uint32_t StencilFunctionMask = 0xFF;

		bool EnableFaceCulling = true;
		bool EnableBlending = false;
		bool EnableDepthTest = true;
		bool EnableDepthMask = true;
		bool EanbleStencilTest = false;
		bool TextureCubeMapSeamless = true;
		bool EnableMultiSampling = false;
		bool EnableClipDistance0 = false;
		bool EnableLineSmooth = false;
	};

	class PipelineState
	{
	public:
		PipelineState(const PipelineStateDesc& desc);
		void Enter();
		void Exit();
		PipelineStateDesc Description;
	};
}

#endif