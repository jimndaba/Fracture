#include "FracturePCH.h"
#include "RenderCommands.h"
#include "Viewport.h"
#include "Scissor.h"
#include "RenderContext.h"
#include "Shader.h"
#include "Material.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"



std::function mErroCallback = [](std::string fnc) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::printf("GL Error %s at %s \n",fnc.c_str(), error.c_str());
	}
};


void Fracture::RenderCommands::Enable(Fracture::RenderContext* cntxt, GLCapability ability)
{	
	if (cntxt->mContextState[ability])
		return;

	Command cmd;
	cmd.fnc = [ability]() {
		glEnable((GLenum)ability);
		
	};
	cmd.fnc();
	cntxt->mContextState[ability] = true;
	////cntxt->Push(cmd);  
}

void Fracture::RenderCommands::Disable(Fracture::RenderContext* cntxt, GLCapability ability)
{
	if (!cntxt->mContextState[ability])
		return;

	Command cmd;
	cmd.fnc = [ability]() {
		glDisable((GLenum)ability);
	};
	cmd.fnc();
	cntxt->mContextState[ability] = false;
	////cntxt->Push(cmd);  
	
}

void Fracture::RenderCommands::ClearTarget(Fracture::RenderContext* cntxt, uint32_t flags)
{
	Command cmd;
	cmd.fnc = [flags]() {
		glClear(flags);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::ClearColor(Fracture::RenderContext* cntxt, const Fracture::Colour& color)
{
	Fracture::Command cmd;
	cmd.fnc = [color]() {
		glClearColor(color.R, color.G, color.B, color.A);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 


}

void Fracture::RenderCommands::CheckFrameBuffer(Fracture::RenderContext* cntxt,uint32_t fb)
{
	Fracture::Command cmd;
	cmd.fnc = [fb]() {

		auto fboState = glCheckNamedFramebufferStatus(fb, GL_FRAMEBUFFER);
		if (fboState != GL_FRAMEBUFFER_COMPLETE)
		{
			if (fboState == GL_FRAMEBUFFER_UNDEFINED)
			{
				FRACTURE_ERROR("GL_FRAMEBUFFER_UNDEFINED");
			}
			if (fboState == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			{
				FRACTURE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			}
			if (fboState == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			{
				FRACTURE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ");
			}
			if (fboState == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			{
				FRACTURE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			}
			if (fboState == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
			{
				FRACTURE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			}
			
		}
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BlitFramebuffer(Fracture::RenderContext* cntxt, const std::string& From, const std::string& to)
{
	const auto& from_fb = GraphicsDevice::Instance()->GetGlobalRenderTarget(From);
	const auto& to_fb = GraphicsDevice::Instance()->GetGlobalRenderTarget(to);

	Fracture::Command cmd;
	cmd.fnc = [from_fb,to_fb]() {
		glBlitNamedFramebuffer(from_fb->Handle, to_fb->Handle, 0, 0, 1920,1080, 0, 0, 1920, 1080,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBlitNamedFramebuffer(from_fb->Handle, to_fb->Handle, 0, 0, 1920, 1080, 0, 0, 1920, 1080,
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BlitDepthFramebuffer(Fracture::RenderContext* cntxt, const std::string& From, const std::string& to)
{
}

void Fracture::RenderCommands::BlitFramebuffer(Fracture::RenderContext* cntxt, uint32_t fb_from, uint32_t fb_to)
{	
}

void Fracture::RenderCommands::SetViewport(Fracture::RenderContext* cntxt, float width, float height,float x, float y)
{
	if (cntxt->ContextViewport.Width == width && cntxt->ContextViewport.Height == height)
		return;

	cntxt->ContextViewport.Width = width;
	cntxt->ContextViewport.Height = height;

	Fracture::Command cmd;
	cmd.fnc = [width,height,x,y]() {
		glViewport(x, y, width, height);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetScissor(Fracture::RenderContext* cntxt, float width, float height, float x, float y)
{
	if (cntxt->ContextScissor.Width == width && cntxt->ContextScissor.Height == height)
		return;

	cntxt->ContextScissor.Width = width;
	cntxt->ContextScissor.Height = height;

	Fracture::Command cmd;
	cmd.fnc = [x,y,width,height]() {
		glScissor(x, y, width, height);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetRenderTarget(Fracture::RenderContext* cntxt, Fracture::RenderTarget* rt)
{
	if (cntxt->CurrentRenderTarget == rt->Handle)
		return;
	cntxt->CurrentRenderTarget = rt->Handle;

	Fracture::Command cmd;
	cmd.fnc = [rt]() {
		glBindFramebuffer(GL_FRAMEBUFFER,rt->Handle);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetRenderTarget(Fracture::RenderContext* cntxt, uint32_t rt)
{
	if (cntxt->CurrentRenderTarget == rt)
		return;
	cntxt->CurrentRenderTarget = rt;

	Fracture::Command cmd;
	cmd.fnc = [rt]() {
		glBindFramebuffer(GL_FRAMEBUFFER, rt);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void  Fracture::RenderCommands::SetRenderBuffer(Fracture::RenderContext* cntxt, uint32_t rb)
{
	Fracture::Command cmd;
	cmd.fnc = [rb]() {
		glBindRenderbuffer(GL_RENDERBUFFER, rb);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::ReleaseRenderTarget(Fracture::RenderContext* cntxt)
{
	Fracture::Command cmd;
	cmd.fnc = []() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};
	cmd.fnc();
	cntxt->CurrentRenderTarget = 0;
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetCullMode(Fracture::RenderContext* cntxt, CullMode mode)
{

	if (mode == CullMode::None && cntxt->CullFaceEnabled)
	{
		Fracture::Command cmd;
		cntxt->CullFaceEnabled = false;
		cmd.fnc = [mode]() {
			glDisable(GL_CULL_FACE);
		};
		cmd.DEBUG = "Set Cull Mode Disabled";
		cmd.fnc();
		//cntxt->Push(cmd); 
	}
	else
	{
		if (mode == CullMode::None)
			return;

		if (!cntxt->CullFaceEnabled)
		{
			Fracture::Command cmd;
			cmd.DEBUG = "Set Enable Cull Mode Enabled";
			cntxt->CullFaceEnabled = true;
			cmd.fnc = [mode]() {
				glEnable(GL_CULL_FACE);
			};
			cmd.fnc();
			//cntxt->Push(cmd); 
		}

		Fracture::Command cmd;
		cmd.fnc = [mode]() {
			glCullFace((GLenum)mode);
		};
		cmd.DEBUG = "Set Cull Mode";
		cmd.fnc();
		//cntxt->Push(cmd); 
	}
	

}

void Fracture::RenderCommands::DepthFunction(Fracture::RenderContext* cntxt, DepthFunc fnc)
{
	Fracture::Command cmd;
	cmd.fnc = [fnc]() {
		glDepthFunc((GLenum)fnc);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DepthMask(Fracture::RenderContext* cntxt, bool mask)
{
	Fracture::Command cmd;
	cmd.fnc = [mask]() {
		glDepthMask(mask);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::StencilMask(Fracture::RenderContext* cntxt, uint32_t mask)
{
	Fracture::Command cmd;
	cmd.fnc = [mask]() {
		glStencilMask(mask);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BlendFunction(Fracture::RenderContext* cntxt, BlendFunc sfactor, BlendFunc dfactor)
{
	Fracture::Command cmd;
	cmd.fnc = [sfactor, dfactor]() {
		glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BlendEquation(Fracture::RenderContext* cntxt, BlendEq eq)
{
	Fracture::Command cmd;
	cmd.fnc = [eq]() {
		glBlendEquation((GLenum)eq);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
	
}

void Fracture::RenderCommands::StencilFunction(Fracture::RenderContext* cntxt, StencilFunc fnc,int ref,uint32_t mask)
{
	Fracture::Command cmd;
	cmd.fnc = [fnc, ref,mask]() {
		glStencilFunc((GLenum)fnc,ref,mask);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::StencilOperation(Fracture::RenderContext* cntxt, StencilOp_TestResult sfail, StencilOp_TestResult dfail, StencilOp_TestResult sdpass)
{
	Fracture::Command cmd;
	cmd.fnc = [sfail,dfail, sdpass]() {
		glStencilOp((GLenum)sfail, (GLenum)dfail, (GLenum)sdpass);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetColorMask(Fracture::RenderContext* cntxt, bool r, bool g, bool b, bool a)
{
	Fracture::Command cmd;
	cmd.fnc = [r,g,b,a]() {
		glColorMask(r,g,b,a);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BindVertexArrayObject(Fracture::RenderContext* cntxt, uint32_t vao)
{
	if (cntxt->CurrentRenderVAO == vao)
		return;

	cntxt->CurrentRenderVAO = vao;

	Fracture::Command cmd;
	cmd.fnc = [vao]() {
		glBindVertexArray(vao);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BindVertexArrayVertexBuffer(Fracture::RenderContext* cntxt, uint32_t vao, uint32_t index, uint32_t stride, uint32_t buffer, uint32_t offset)
{
	Fracture::Command cmd;
	cmd.fnc = [vao,index,stride,buffer,offset]() {
		glVertexArrayVertexBuffer(vao, index, buffer, offset, stride);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BindVertexArrayIndexBuffer(Fracture::RenderContext* cntxt, uint32_t vao, uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [vao, buffer]() {
		glVertexArrayElementBuffer(vao, buffer);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BindVertexArraySetDivisor(Fracture::RenderContext* cntxt, uint32_t vao, uint32_t AttributeIndex, uint32_t divisor)
{
	Fracture::Command cmd;
	cmd.fnc = [vao, AttributeIndex,divisor]() {
		glVertexArrayBindingDivisor(vao,AttributeIndex ,divisor);	
		mErroCallback("Set Divisor");
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::BindBuffer(Fracture::RenderContext* cntxt, Fracture::BufferType target, uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [target,buffer]() {
		GLubyte val = 0;
		glBindBuffer((GLenum)target, buffer);
		mErroCallback("Buffer Bind");
	};
	cmd.fnc();
}

void Fracture::RenderCommands::ClearBufferData(Fracture::RenderContext* cntxt, uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [buffer]() {
		GLubyte val = 0;
		glClearNamedBufferData(buffer, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &val);
		mErroCallback("Clear Buffer");
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DispatchComputeShader(Fracture::RenderContext* cntxt, uint16_t x, uint16_t y, uint16_t z)
{
	Fracture::Command cmd;
	cmd.fnc = [x,y,z]() {
		glDispatchCompute(x,y,z);
		mErroCallback("Dispatch Compute");

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		mErroCallback("Memory Barrier");
	};
	cmd.fnc();
}

void Fracture::RenderCommands::Barrier(Fracture::RenderContext* cntxt)
{
	Fracture::Command cmd;
	cmd.fnc = []() {
		//TODO glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::FramBufferTexture(Fracture::RenderContext* cntxt, uint32_t attachment_index, uint32_t attachment_target, uint32_t texture, uint32_t level)
{
	Fracture::Command cmd;
	cmd.fnc = [attachment_index, attachment_target, texture, level]() {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_index,GL_TEXTURE_CUBE_MAP_POSITIVE_X + attachment_target , texture, level);		
		mErroCallback("glNamedFramebufferTextureLayer");

	};
	cmd.fnc();
}

void Fracture::RenderCommands::FrameBufferTextureTarget(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t attachment_layer, uint32_t texture, uint32_t level)
{
	Fracture::Command cmd;
	cmd.fnc = [fb, attachment_index, attachment_layer,texture,level]() {
		glNamedFramebufferTextureLayer(fb, GL_COLOR_ATTACHMENT0 + attachment_index,texture,level, attachment_layer);

		//glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_index, (GLuint)texture, level, attachment_layer);
		mErroCallback("glNamedFramebufferTextureLayer");
		
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::FrameBufferDepthTextureTarget(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t attachment_layer, uint32_t texture, uint32_t level)
{
	Fracture::Command cmd;
	cmd.fnc = [fb, attachment_index, attachment_layer, texture, level]() {
		glNamedFramebufferTextureLayer(fb, GL_DEPTH_ATTACHMENT, texture, level, attachment_layer);

		//glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_index, (GLuint)texture, level, attachment_layer);
		mErroCallback("glNamedFramebufferTextureLayer");

	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}


void Fracture::RenderCommands::FrameBufferAttachTexture(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t texture, uint32_t level)
{
	Fracture::Command cmd;
	cmd.fnc = [fb, attachment_index, texture, level]() {
		glNamedFramebufferTexture(fb, GL_COLOR_ATTACHMENT0 + attachment_index, texture, level);
		mErroCallback("glNamedFramebufferTexture");
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
	
}

void Fracture::RenderCommands::FrameBufferAttachTexture3D(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t face, uint32_t texture, uint32_t level, uint32_t layer)
{
	Fracture::Command cmd;
	cmd.fnc = [fb, attachment_index, texture, level,layer,face]() {
		glFramebufferTexture3D(fb, GL_COLOR_ATTACHMENT0 + attachment_index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture, level,layer);
		mErroCallback("AttachTexture");

	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}


void Fracture::RenderCommands::FrameBufferSetDrawBuffers(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t NoBuffers)
{
	unsigned int attachments[10];
	for (int i = 0; i <NoBuffers; i++)
	{
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	//glDrawBuffers(this->Description.NoColorAttachments, attachments);

	Fracture::Command cmd;
	cmd.fnc = [fb, attachments,NoBuffers]() {
		glNamedFramebufferDrawBuffers(fb, NoBuffers, attachments);
		mErroCallback("SetDrawBuffers");

	};
	cmd.fnc();
	//cntxt->Push(cmd); 
	
}

void Fracture::RenderCommands::RenderBufferTextureStorage(Fracture::RenderContext* cntxt, uint32_t rb, InternalFormat internlforamt, int x, int y)
{
	Fracture::Command cmd;
	cmd.fnc = [rb, internlforamt,x,y]() {
		glNamedRenderbufferStorage(rb,(GLenum)internlforamt,x,y);
		mErroCallback("RenderBufferTextureStorage");
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DrawElementsArray(Fracture::RenderContext* cntxt, const Fracture::DrawElementsArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElements((GLenum)render_cmd.mode,render_cmd.count,GL_UNSIGNED_INT,0);
		mErroCallback("Draw Elements");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DrawArray(Fracture::RenderContext* cntxt, const Fracture::DrawArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArrays((GLenum)render_cmd.mode, render_cmd.first, render_cmd.count);
		mErroCallback("Draw Arrays");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DrawArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstanced& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArraysInstanced((GLenum)render_cmd.mode, render_cmd.first, render_cmd.count,render_cmd.instance_count);
		mErroCallback("Draw Arrays Instanced");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
}

void Fracture::RenderCommands::DrawElementsArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawElementsArraysInstanced& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElementsInstanced((GLenum)render_cmd.mode, render_cmd.element_count, GL_UNSIGNED_INT, 0, render_cmd.instance_count);
		mErroCallback("Draw ELements Instanced");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DrawElementsBaseVertex(Fracture::RenderContext* cntxt, const Fracture::DrawElementsBaseVertex& render_cmd)
{
}

void Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Fracture::RenderContext* cntxt, const Fracture::DrawElementsInstancedBaseVertex& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElementsInstancedBaseVertex((GLenum)render_cmd.mode, render_cmd.count, GL_UNSIGNED_INT,render_cmd.indices, render_cmd.instancecount, render_cmd.basevertex);

		mErroCallback("DrawElementsBaseInstance");

		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::DrawElementsIndirect(Fracture::RenderContext* cntxt, void* indirect,int count, int stride)
{
	glMultiDrawElementsIndirect((GLenum)GL_TRIANGLE_STRIP, GL_UNSIGNED_INT,indirect,count,stride);
	mErroCallback("Draw Elements Indirect");
	GraphicsDevice::DRAWCALL_COUNT++;
}

void Fracture::RenderCommands::DrawArraysInstancedBaseInstance(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstancedBaseInstance& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArraysInstancedBaseInstance((GLenum)render_cmd.mode, render_cmd.firstIndex , render_cmd.count,render_cmd.instanceCount,render_cmd.baseInstance);


		mErroCallback("DrawArrays Instance Base Vert");

		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::MapDataFrombuffer(void* ptr, uint32_t buffer, BufferAccess access)
{	
	Fracture::Command cmd;
	cmd.fnc = [&ptr, buffer, access]() {};
	ptr = glMapNamedBuffer(buffer, (GLenum)access);
	mErroCallback("MapNamedBuffer");
	cmd.fnc();	
}

void Fracture::RenderCommands::UnMapbuffer(uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [buffer]() {
	
	};
	glUnmapNamedBuffer(buffer);
	mErroCallback("UnMap");
	cmd.fnc();
	//cntxt->Push(cmd); 
	
}

void Fracture::RenderCommands::ClearImage(Fracture::RenderContext* cntxt, uint32_t image, int level, int* clearValue)
{
	Fracture::Command cmd;
	cmd.fnc = [image, level,clearValue]() {
		glClearTexImage(image, level, GL_RGBA, GL_UNSIGNED_BYTE,clearValue);
		mErroCallback("ClearImage");
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::UseProgram(Fracture::RenderContext* cntxt, uint32_t program)
{
	if (cntxt->CurrentProgram == program)
		return;
	
	cntxt->CurrentProgram = program;

	Fracture::Command cmd;
	cmd.Key.ShaderIndex = program;
	cmd.fnc = [program,cntxt]() {
		glUseProgram(program);
		mErroCallback("UseProgram: " + std::to_string(program));
	};
	cmd.fnc();
	//cntxt->Push(cmd); 
}


#pragma region Shader Commands



void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const int& value)
{
	Fracture::Command cmd;
	auto location =  shader->getUniformLocation(name);	
	cmd.fnc = [location, value, name]() {
		glUniform1i(location, value);


		mErroCallback("SetUnfirom: " + name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const float& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform1f(location, value);


		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 

	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const bool& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform1i(location, value);

		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 

}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec2& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform2f(location, value.x,value.y);


		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec3& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform3f(location, value.x, value.y,value.z);

		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 
	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec4& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform4f(location, value.x, value.y, value.z,value.w);
		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::mat3& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));

		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::mat4& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value,name]() {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	//cntxt->Push(cmd); 

}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const Fracture::Colour& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value, name]() {
		glUniform4f(location, value.R, value.G, value.B, value.A);


		mErroCallback(name);

	};
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
}

void Fracture::RenderCommands::SetTexture(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string& name, const uint32_t& RenderID, unsigned int unit)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, unit, RenderID, name,shader] () {
		glBindTextureUnit(unit, RenderID);
		mErroCallback("Shader Bind Unit: " + shader->Description.Name);
		glUniform1i(location, unit);
		mErroCallback(name);
	};
	cmd.fnc();
	cntxt->ActiveTextureUnits++;

}

void Fracture::RenderCommands::BindMaterial(Fracture::RenderContext* cntxt, Fracture::Shader* shader, Fracture::Material* material)
{
	if (material)
	{
		SetUniform(cntxt, shader, "IsAffectedByWind", material->IsAffectedByWind);

		if (material->HasAlbedoTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->AlbedoTexture);
			if (texture)
			{			
				SetTexture(cntxt, shader, "aAlbedo", texture->Handle, (int)TEXTURESLOT::Albedo);
			}
		}
		if (material->HasSpecularTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->SpecularTexture);
			if (texture)
			{			
				SetTexture(cntxt, shader, "aSpecular", texture->Handle, (int)TEXTURESLOT::Specular);
			}
		}
		if (material->HasNormalTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->NormalTexture);
			if (texture)
			{				
				SetTexture(cntxt, shader, "aNormal", texture->Handle, (int)TEXTURESLOT::Normal);
			}
		}
		if (material->HasRoughnessTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->RoughnessTexture);
			if (texture)
			{				
				SetTexture(cntxt, shader, "aRoughness", texture->Handle, (int)TEXTURESLOT::Roughness);
			}
		}
		if (material->HasMetalTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->MetallicTexture);
			if (texture)
			{			
				SetTexture(cntxt, shader, "aMetalness", texture->Handle, (int)TEXTURESLOT::Metalness);
			}
		}
		if (material->HasAOTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->AOTexture);
			if (texture)
			{			
				SetTexture(cntxt, shader, "aAO", texture->Handle, (int)TEXTURESLOT::AO);
			}
		}
		if (material->HasEmissionTexture)
		{
			const auto& texture = AssetManager::GetTextureByID(material->EmmissionTexture);
			if (texture)
			{				
				SetTexture(cntxt, shader, "aEmission", texture->Handle, (int)TEXTURESLOT::Emmission);
			}
		}		
		for (const auto& uniform : material->Uniforms)
		{
			switch (uniform.type)
			{
				case UniformType::INT:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.INT);

				}
				case UniformType::BOOL:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.BOOL);
					break;
				}
				case UniformType::FLOAT:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.FLOAT);
					break;
				}
				case UniformType::VEC2:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.VEC2);
					break;
				}
				case UniformType::VEC3:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.VEC3);
					break;
				}
				case UniformType::VEC4:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.VEC4);
					break;
				}
				case UniformType::COLOR3:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.COLOR3);
					break;
				}
				case UniformType::COLOR4:
				{
					SetUniform(cntxt, shader, uniform.Name, uniform.data.COLOR4);
					break;
				}
				case UniformType::SAMPLE2D:
				{

					const auto& texture = AssetManager::GetTextureByID(uniform.TextureID);
					if (texture)
						SetTexture(cntxt, shader, uniform.Name, texture->Handle,material->TextureUnits[uniform.Name]);
					break;
				}
				case UniformType::SAMPLECUBE:
				{
					const auto& texture = AssetManager::GetTextureByID(uniform.TextureID);
					if (texture)
						SetTexture(cntxt, shader, uniform.Name, texture->Handle, material->TextureUnits[uniform.Name]);
					break;
				}
				case UniformType::GLOBALDEPTH:
				{
					material->GlobalDepth(cntxt, shader, 0);
					break;
				}
				case UniformType::GLOBALGRAB:
				{
					material->GlobalGrab(cntxt, shader, 0);
					break;
				}
				case UniformType::GLOBALNORMAL:
				{
					material->GlobalNormal(cntxt, shader, 0);
					break;
				}
				case UniformType::GLOBALPOSITION:
				{
					material->GlobalPosition(cntxt, shader, 0);
					break;
				}
				case UniformType::GLOBALDELTATIME:
				{
					material->GlobalDeltaTime(cntxt, shader, cntxt->deltaTime);
					break;
				}
			}
		}
	}
}


void Fracture::RenderCommands::ResetTextureUnits(Fracture::RenderContext* cntxt,Fracture::Shader* shader)
{
	if (cntxt->ActiveTextureUnits <= 0)
		return;

	Fracture::Command cmd;	
	for (int i = 0; i < cntxt->ActiveTextureUnits; i++)
	{
		cmd.fnc = [i]() {
			glBindTextureUnit(i, 0);
			mErroCallback("Reset Texture Unit");		
		};
	}	
	cmd.Key.ShaderIndex = shader->Handle;
	cmd.fnc();
	cntxt->ActiveTextureUnits = 0;
}


#pragma endregion
