#include "FracturePCH.h"
#include "IPass.h"

Fracture::IPass::IPass(const std::string& name, RenderContext* context):
	Name(name),ID(UUID()),Context(context)
{
}

