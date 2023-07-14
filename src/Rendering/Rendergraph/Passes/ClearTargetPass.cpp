#include "FracturePCH.h"
#include "ClearTargetPass.h"
#include "ForwardPass.h"

std::string Fracture::ClearTargetSockets::InBuffer = "InBuffer";
std::string Fracture::ClearTargetSockets::OutBuffer = "OutBuffer";


Fracture::ClearTargetPass::ClearTargetPass(const std::string& name, RenderContext* context, const ClearTargetPassDef& info):IPass(name,context), Properties(info)
{
}


void Fracture::ClearTargetPass::Setup()
{
}

void Fracture::ClearTargetPass::Execute()
{

	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::ClearColor(Context, Properties.ClearColor);	
	RenderCommands::ClearTarget(Context,Properties.BufferTarget);


}
