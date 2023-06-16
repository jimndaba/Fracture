#include "FracturePCH.h"
#include "RenderCommands.h"

#include "Viewport.h"
#include "Scissor.h"

#include "RenderContext.h"
#include "RenderTarget.h"

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

void Fracture::RenderCommands::SetRenderTarget(Fracture::RenderContext* cntxt, uint32_t rt)
{
	Fracture::Command cmd;
	cmd.fnc = [rt]() {
		glBindFramebuffer(GL_FRAMEBUFFER, rt);
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

void Fracture::RenderCommands::DepthMask(Fracture::RenderContext* cntxt, bool mask)
{
	Fracture::Command cmd;
	cmd.fnc = [mask]() {
		glDepthMask(mask);
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

void Fracture::RenderCommands::SetColorMask(Fracture::RenderContext* cntxt, bool r, bool g, bool b, bool a)
{
	Fracture::Command cmd;
	cmd.fnc = [r,g,b,a]() {
		glColorMask(r,g,b,a);
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

void Fracture::RenderCommands::DispatchComputeShader(Fracture::RenderContext* cntxt, uint16_t x, uint16_t y, uint16_t z)
{
	Fracture::Command cmd;
	cmd.fnc = [x,y,z]() {
		glDispatchCompute(x,y,z);
		mErroCallback("Dispatch Compute");

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		mErroCallback("Memory Barrier");
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::Barrier(Fracture::RenderContext* cntxt)
{
	Fracture::Command cmd;
	cmd.fnc = []() {
		//TODO glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawElementsArray(Fracture::RenderContext* cntxt, const Fracture::DrawElementsArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawElements((GLenum)render_cmd.mode,render_cmd.count,GL_UNSIGNED_INT,0);
		mErroCallback("Draw Elements");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawArray(Fracture::RenderContext* cntxt, const Fracture::DrawArray& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArrays((GLenum)render_cmd.mode, render_cmd.first, render_cmd.count);
		mErroCallback("Draw Arrays");
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
		mErroCallback("Draw ELements Instanced");
		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
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
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::DrawArraysInstancedBaseInstance(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstancedBaseInstance& render_cmd)
{
	Fracture::Command cmd;
	cmd.fnc = [render_cmd]() {
		glDrawArraysInstancedBaseInstance((GLenum)render_cmd.mode, render_cmd.firstIndex , render_cmd.count,render_cmd.instanceCount,render_cmd.baseInstance);


		mErroCallback("DrawArrays Instance Base Vert");

		GraphicsDevice::DRAWCALL_COUNT++;
	};
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::UnMapbuffer(Fracture::RenderContext* cntxt, uint32_t buffer)
{
	Fracture::Command cmd;
	cmd.fnc = [buffer]() {
		glUnmapNamedBuffer(buffer);

		mErroCallback("UnMap");
	};
	cntxt->Push(cmd);
	
}

void Fracture::RenderCommands::ClearImage(Fracture::RenderContext* cntxt, uint32_t image, int level, int* clearValue)
{
	Fracture::Command cmd;
	cmd.fnc = [image, level,clearValue]() {
		glClearTexImage(image, level, GL_RGBA, GL_UNSIGNED_BYTE,clearValue);
		mErroCallback("ClearImage");
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
	

		mErroCallback("UseProgram");
	};
	cntxt->Push(cmd);
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
	cntxt->Push(cmd);	
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
	cntxt->Push(cmd);

	
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
	cntxt->Push(cmd);

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
	cntxt->Push(cmd);
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
	cntxt->Push(cmd);
	
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
	cntxt->Push(cmd);
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
	cntxt->Push(cmd);
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
	cntxt->Push(cmd);

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
	cntxt->Push(cmd);
}

void Fracture::RenderCommands::SetTexture(Fracture::RenderContext* cntxt, Fracture::Shader* shader, const std::string& name, const uint32_t& RenderID, unsigned int unit)
{
	Fracture::Command cmd;
	auto location = shader->getUniformLocation(name);
	cmd.fnc = [location, unit, RenderID, name]() {
		glBindTextureUnit(unit, RenderID);
		mErroCallback("Bind Unit");
		glUniform1i(location, unit);
		mErroCallback(name);
	};
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
	cntxt->ActiveTextureUnits++;

}

void Fracture::RenderCommands::BindMaterial(Fracture::RenderContext* cntxt, Fracture::Shader* shader, Fracture::Material* material)
{
	if (material)
	{
		SetUniform(cntxt, shader, "TextureSpace",(int)material->TextureSpace);
		SetUniform(cntxt, shader, "Tiling",material->TextureTiling);
		SetUniform(cntxt, shader, "pDiffuse",material->AlbedoColour);
		SetUniform(cntxt, shader, "pEmission",material->EmissionColour);
		SetUniform(cntxt, shader, "pAO",material->AOLevel);
		SetUniform(cntxt, shader, "pMetalness",material->MetalicLevel);
		SetUniform(cntxt, shader, "pRoughness",material->RoughnessLevel);
		SetUniform(cntxt, shader, "EmissionStrength",material->EmmisionStrength);
		SetUniform(cntxt, shader, "SpecularLevel",material->SpecularLevel);
		SetUniform(cntxt, shader, "SpecularIntensity",material->SpecularIntensity);

		SetUniform(cntxt, shader, "_AlbedoFlag", material->HasAlbedoTexture);
		SetUniform(cntxt, shader, "_SpecularFlag", material->HasSpecularTexture);
		SetUniform(cntxt, shader, "_NormalFlag", material->HasNormalTexture);
		SetUniform(cntxt, shader, "_RoughnessFlag", material->HasRoughnessTexture);
		SetUniform(cntxt, shader, "_MetalnessFlag", material->HasMetalTexture);
		SetUniform(cntxt, shader, "_AOFlag", material->HasAOTexture); 
		SetUniform(cntxt, shader, "_EmissionFlag", material->HasEmissionTexture);

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
			}
		}
	}
}


void Fracture::RenderCommands::ResetTextureUnits(Fracture::RenderContext* cntxt,Fracture::Shader* shader)
{
	Fracture::Command cmd;	
	for (int i = 0; i < cntxt->ActiveTextureUnits; i++)
	{
		cmd.fnc = [i]() {
			glBindTextureUnit(i, 0);
			mErroCallback("Reset Texture Unit");		
		};
	}
	
	cmd.Key.ShaderIndex = shader->Handle;
	cntxt->Push(cmd);
	cntxt->ActiveTextureUnits = 0;
}


#pragma endregion
