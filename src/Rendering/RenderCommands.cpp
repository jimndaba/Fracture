#include "FracturePCH.h"
#include "RenderCommands.h"

#include "Viewport.h"
#include "Scissor.h"

#include "RenderContext.h"
#include "RenderTarget.h"

#include "Shader.h"

#include "GraphicsDevice.h"

void Fracture::RenderCommands::Enable(Fracture::RenderContext* cntxt, GLCapability ability)
{	
	Command cmd;
	cmd.fnc = [ability]() {
		glEnable((GLenum)ability);
	};
	cntxt->Push(cmd); 
}

void Fracture::RenderCommands::Disable(Fracture::RenderContext* cntxt, GLCapability ability)
{
	Command cmd;
	cmd.fnc = [ability]() {
		glDisable((GLenum)ability);
	};
	cntxt->Push(cmd);
	
}

void Fracture::RenderCommands::ClearTarget(Fracture::RenderContext* cntxt, uint32_t flags)
{
	Command cmd;
	cmd.fnc = [flags]() {
		glClear(flags);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::ClearColor(Fracture::RenderContext* cntxt, const Fracture::Colour& color)
{
	Fracture::Command cmd;
	cmd.fnc = [color]() {
		glClearColor(color.R, color.G, color.B, color.A);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetViewport(Fracture::RenderContext* cntxt, float width, float height,float x, float y)
{
	Fracture::Command cmd;
	cmd.fnc = [width,height,x,y]() {
		glViewport(x, y, width, height);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetScissor(Fracture::RenderContext* cntxt, float width, float height, float x, float y)
{
	Fracture::Command cmd;
	cmd.fnc = [x,y,width,height]() {
		glScissor(x, y, width, height);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetRenderTarget(Fracture::RenderContext* cntxt, Fracture::RenderTarget* rt)
{
	Fracture::Command cmd;
	cmd.fnc = [rt]() {
		glBindFramebuffer(GL_FRAMEBUFFER,rt->Handle);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::ReleaseRenderTarget(Fracture::RenderContext* cntxt)
{
	Fracture::Command cmd;
	cmd.fnc = []() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};
	cntxt->Push(cmd);
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
		cntxt->Push(cmd);
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
			cntxt->Push(cmd);
		}

		Fracture::Command cmd;
		cmd.fnc = [mode]() {
			glCullFace((GLenum)mode);
		};
		cmd.DEBUG = "Set Cull Mode";
		cntxt->Push(cmd);
	}
	

}

void Fracture::RenderCommands::DepthFunction(Fracture::RenderContext* cntxt, DepthFunc fnc)
{
	Fracture::Command cmd;
	cmd.fnc = [fnc]() {
		glDepthFunc((GLenum)fnc);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::BlendFunction(Fracture::RenderContext* cntxt, BlendFunc sfactor, BlendFunc dfactor)
{
	Fracture::Command cmd;
	cmd.fnc = [sfactor, dfactor]() {
		glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::StencilFunction(Fracture::RenderContext* cntxt, StencilFunc fnc,int ref,uint32_t mask)
{
	Fracture::Command cmd;
	cmd.fnc = [fnc, ref,mask]() {
		glStencilFunc((GLenum)fnc,ref,mask);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::BindVertexArrayObject(Fracture::RenderContext* cntxt, uint32_t vao)
{
	Fracture::Command cmd;
	cmd.fnc = [vao]() {
		glBindVertexArray(vao);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawElementsArray(Fracture::RenderContext* cntxt, const Fracture::DrawElementsArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElements((GLenum)render_cmd.mode,render_cmd.count,GL_UNSIGNED_INT,0);
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawArray(Fracture::RenderContext* cntxt, const Fracture::DrawArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArrays((GLenum)render_cmd.mode, render_cmd.first, render_cmd.count);
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstanced& render_cmd)
{
}

void Fracture::RenderCommands::DrawElementsArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawElementsArraysInstanced& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElementsInstanced((GLenum)render_cmd.mode, render_cmd.element_count, GL_UNSIGNED_INT, 0, render_cmd.instance_count);
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawElementsBaseVertex(Fracture::RenderContext* cntxt, const Fracture::DrawElementsBaseVertex& render_cmd)
{
}

void Fracture::RenderCommands::DrawArraysInstancedBaseInstance(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstancedBaseInstance& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArraysInstancedBaseInstance((GLenum)render_cmd.mode, render_cmd.firstIndex , render_cmd.count,render_cmd.instanceCount,render_cmd.baseInstance);
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::UnMapbuffer(Fracture::RenderContext* cntxt, uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [buffer]() {
		glUnmapNamedBuffer(buffer);
	};
	cntxt->Push(cmd);
	
}

void Fracture::RenderCommands::UseProgram(Fracture::RenderContext* cntxt, uint32_t program)
{
	Fracture::Command cmd;
	cmd.Key.ShaderIndex = program;
	cmd.fnc = [program,cntxt]() {
		glUseProgram(program);
		cntxt->CurrentProgram = program;
	};
	cntxt->Push(cmd);
}


#pragma region Shader Commands



void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const int& value)
{
	Fracture::Command cmd;
	auto location =  shader->getUniformLocation(name);	
	cmd.fnc = [location, value]() {
		glUniform1i(location, value);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const float& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform1f(location, value);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);

	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const bool& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform1i(location, value);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);

}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec2& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform2f(location, value.x,value.y);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec3& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform3f(location, value.x, value.y,value.z);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
	
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::vec4& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform4f(location, value.x, value.y, value.z,value.w);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::mat3& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const glm::mat4& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);

}

void Fracture::RenderCommands::SetUniform(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string name, const Fracture::Colour& value)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, value]() {
		glUniform4f(location, value.R, value.G, value.B, value.A);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetTexture(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string& name, const uint32_t& RenderID, unsigned int unit)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, unit, RenderID]() {
		glBindTextureUnit(unit, RenderID);
		glUniform1i(location, unit);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);

}

#pragma endregion
