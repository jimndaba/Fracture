#include "FracturePCH.h"
#include "PipelineState.h"

Fracture::PipelineState::PipelineState(const PipelineStateDesc& desc):
	Description(desc)
{
}

void Fracture::PipelineState::Enter()
{
	if (Description.EanbleStencilTest)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp((GLenum)Description.StFail, (GLenum)Description.StPass_DtFail, (GLenum)Description.StPass_DtPass);
		glStencilFunc((GLenum)Description.stencilFunction, 1, Description.StencilFunctionMask);
		glStencilMask(Description.StencilBitMask);
	}

	if (Description.EnableBlending)
	{
		glEnable(GL_BLEND);
		glBlendFunc((GLenum)Description.blendFunction_sFactor, (GLenum)Description.blendFunction_dFactor);
	}

	if (Description.EnableLineSmooth)
		glEnable(GL_LINE_SMOOTH);

	if (Description.EnableClipDistance0)
		glEnable(GL_CLIP_DISTANCE0);

	if (Description.EnableDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc((GLenum)Description.depthFunction);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glDepthMask(Description.EnableDepthMask);

	if (Description.EnableFaceCulling)
		glEnable(GL_CULL_FACE);

	switch (Description.cullMode)
	{
	case CullMode::None:
		glDisable(GL_CULL_FACE);
		break;
	case CullMode::Back:
		glCullFace(GL_BACK);
		break;
	case CullMode::Front:
		glCullFace(GL_FRONT);
		break;
	case CullMode::FrontAndBack:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	}

	switch (Description.fillMode)
	{
	case FillMode::Fill:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case FillMode::Line:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}

}

void Fracture::PipelineState::Exit()
{
	if (Description.EanbleStencilTest)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glDisable(GL_STENCIL_TEST);
	}

	if (Description.EnableBlending)
	{
		glBlendFunc((GLenum)BlendFunc::Zero, (GLenum)BlendFunc::Zero);
		glDisable(GL_BLEND);
	}

	if (Description.EnableLineSmooth)
		glDisable(GL_LINE_SMOOTH);

	if (Description.EnableClipDistance0)
		glDisable(GL_CLIP_DISTANCE0);

	if (Description.EnableDepthTest)
		glDisable(GL_DEPTH_TEST);

	if (Description.EnableFaceCulling)
		glDisable(GL_CULL_FACE);
}
