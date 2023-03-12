#include "FracturePCH.h"
#include "RenderTarget.h"

Fracture::RenderTarget::RenderTarget():IGraphicsResource()
{
}

Fracture::RenderTargetCreationInfo::RenderTargetCreationInfo()
{
}

Fracture::RenderTargetCreationInfo::RenderTargetCreationInfo(std::initializer_list<TextureCreationInfo> c_attachments):
	ColorAttachments(c_attachments)
{
}
