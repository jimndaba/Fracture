#pragma once
#ifndef RENDERCOMMANDS_H
#define RENDERCOMMANDS_H

#include "Command.h"
#include "Color.h"
#include "RenderContext.h"
#include "RenderTarget.h"
#include "GraphicsDevice.h"
#include "PipelineState.h"

namespace Fracture
{
	struct Scissor;
	struct Viewport;

	struct RenderTarget;
	struct Shader;
	struct Material;

	enum ClearFlags : uint32_t
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};

	enum class DrawMode : uint32_t
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineLoops = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN
	};

	enum BufferAccess
	{
		WriteOnly = GL_WRITE_ONLY,
		ReadOnly = GL_READ_ONLY,
		ReadWrite = GL_READ_WRITE
	};
	
	struct DrawArray
	{
		DrawMode mode;
		uint32_t first;
		uint32_t count;
	};

	struct DrawArraysInstanced
	{};

	struct DrawElementsArray
	{
		DrawMode mode = DrawMode::Triangles;
		uint32_t count = 0;
		uint32_t Offset = 0;
	};
	
	struct DrawElementsArraysInstanced
	{
		DrawMode mode = DrawMode::Triangles;
		uint32_t element_count;
		uint32_t instance_count;
	};

	struct DrawElementsBaseVertex
	{
		DrawMode mode = DrawMode::Triangles;
		uint32_t baseindex; 
		uint32_t basevertex; 
		uint32_t count;
	};

	struct DrawElementsInstancedBaseVertex
	{
		DrawMode mode = DrawMode::Triangles;
		uint32_t count;
		void* indices;
		uint32_t instancecount;
		uint32_t basevertex;	
	};

	struct DrawElementsIndirectCommand {
		uint32_t  count;
		uint32_t  instanceCount;
		uint32_t  firstIndex;
		int  baseVertex;
		uint32_t  baseInstance;
	};

	struct MultiDrawElementsIndirect
	{
		std::vector<DrawElementsIndirectCommand> indirect;
	};

	struct DrawArraysInstancedBaseInstance
	{
		DrawMode mode = DrawMode::Triangles;
		uint32_t  firstIndex;
		uint32_t  count;
		uint32_t  instanceCount;		
		uint32_t  baseInstance;
	};

	enum class RenderTargetType
	{
		ColorBuffer,
		StencilBuffer,
		DepthBuffer,
		All
	};

	enum class GLCapability
	{
		DepthTest = GL_DEPTH_TEST,
		StencilTest = GL_STENCIL_TEST,
		ScissorTest = GL_SCISSOR_TEST,
		Blending = GL_BLEND,
		SmoothLines = GL_LINE_SMOOTH,
		FaceCulling = GL_CULL_FACE,
		TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
		MultiSample = GL_MULTISAMPLE,
		ClipDistance0 = GL_CLIP_DISTANCE0,
		Texture2D = GL_TEXTURE_2D
	};

	namespace RenderCommands
	{
		
		void Enable(Fracture::RenderContext* cntxt, GLCapability abiltiy);
		void Disable(Fracture::RenderContext* cntxt, GLCapability abiltiy);
		void ClearTarget(Fracture::RenderContext* cntxt, uint32_t flags);
		void ClearColor(Fracture::RenderContext* cntxt, const Fracture::Colour& color);
		void SetViewport(Fracture::RenderContext* cntxt, float width, float height, float x, float y);
		void SetScissor(Fracture::RenderContext* cntxt, float width, float height, float x, float y);
		void SetRenderTarget(Fracture::RenderContext* cntxt, RenderTarget* rt);
		void SetRenderTarget(Fracture::RenderContext* cntxt, uint32_t rt);
		void SetRenderBuffer(Fracture::RenderContext* cntxt, uint32_t rb);
		void ReleaseRenderTarget(Fracture::RenderContext* cntxt);
		void SetCullMode(Fracture::RenderContext* cntxt, CullMode mode);

		void DepthFunction(Fracture::RenderContext* cntxt, DepthFunc fnc);
		void DepthMask(Fracture::RenderContext* cntxt,bool mask);
		void BlendFunction(Fracture::RenderContext* cntxt, BlendFunc sfactor, BlendFunc dfactor);
		void StencilFunction(Fracture::RenderContext* cntxt, StencilFunc fnc, int ref, uint32_t mask);
		void SetColorMask(Fracture::RenderContext* cntxt, bool r, bool g, bool b, bool a);

		void BindVertexArrayObject(Fracture::RenderContext* cntxt, uint32_t vao);
		void BindVertexArrayVertexBuffer(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t index, uint32_t stride, uint32_t buffer, uint32_t offset = 0);
		void BindVertexArrayIndexBuffer(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t buffer);
		void BindVertexArraySetDivisor(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t AttributeIndex, uint32_t divisor);
		void ClearBufferData(Fracture::RenderContext* cntxt, uint32_t buffer);
		
		void DispatchComputeShader(Fracture::RenderContext* cntxt, uint16_t x, uint16_t y, uint16_t z);
		void Barrier(Fracture::RenderContext* cntxt);

		void FrameBufferTextureTarget(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t attachment_target, uint32_t texture, uint32_t level);
		void FrameBufferAttachTexture(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t texture, uint32_t level);
		void FrameBufferSetDrawBuffers(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t NoBuffers);
		void RenderBufferTextureStorage(Fracture::RenderContext* cntxt, uint32_t rb, InternalFormat internal, int x, int y);
		
		void DrawArray(Fracture::RenderContext* cntxt, const Fracture::DrawArray& cmd);
		void DrawArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstanced& cmd);
		void DrawElementsArray(Fracture::RenderContext* cntxt,const Fracture::DrawElementsArray& cmd);
		void DrawElementsArrayInstanced(Fracture::RenderContext* cntxt,const Fracture::DrawElementsArraysInstanced& cmd);
		void DrawElementsBaseVertex(Fracture::RenderContext* cntxt,const Fracture::DrawElementsBaseVertex& cmd);
		void DrawElementsInstancedBaseVertex(Fracture::RenderContext* cntxt, const Fracture::DrawElementsInstancedBaseVertex& cmd);
		void DrawArraysInstancedBaseInstance(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstancedBaseInstance& cmd);

		template<class T>
	    void MapDataTobuffer(Fracture::RenderContext* cntxt,uint32_t buffer, std::vector<T>& data, uint32_t size,BufferAccess access = BufferAccess::WriteOnly);

		template<class T>
	    void BufferSubData(Fracture::RenderContext* cntxt,uint32_t buffer, std::vector<T>& data, uint32_t size, uint32_t offset,BufferAccess access = BufferAccess::WriteOnly);


		void UnMapbuffer(Fracture::RenderContext* cntxt,uint32_t buffer);

		template<RenderTargetType E>
		void CopyRenderTarget(RenderTarget* from_buffer, RenderTarget* to_buffer, uint32_t attachment_index);

		void ClearImage(Fracture::RenderContext* cntxt, uint32_t image, int level,int* clearValue);

		void UseProgram(Fracture::RenderContext* cntxt, uint32_t program);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const int& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const float& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const bool& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const glm::vec2& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const glm::vec3& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const glm::vec4& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const glm::mat3& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader,const std::string name, const glm::mat4& value);
		void SetUniform(Fracture::RenderContext* cntxt,Fracture::Shader* shader, const std::string name, const Fracture::Colour& value);
		void SetTexture(Fracture::RenderContext* cntxt,Fracture::Shader* shader, const std::string& name, const uint32_t& RenderID, unsigned int unit);		

		void BindMaterial(Fracture::RenderContext* cntxt, Fracture::Shader* shader, Fracture::Material* material);
		void ResetTextureUnits(Fracture::RenderContext* cntxt, Fracture::Shader* shader);

		template<class T>
		void MapDataTobuffer(Fracture::RenderContext* cntxt, uint32_t buffer, std::vector<T>& data, uint32_t size, BufferAccess access)
		{
			Fracture::Command cmd;
			cmd.fnc = [cntxt, buffer, data, size,access]() {
				auto ptr = glMapNamedBuffer(buffer, (GLenum)access);
				memcpy(ptr, data.data(), size);				
			};
			cntxt->Push(cmd);	
			UnMapbuffer(cntxt, buffer);
		}

		template<class T>
		void BufferSubData(Fracture::RenderContext* cntxt, uint32_t buffer, std::vector<T>& data, uint32_t size, uint32_t offset, BufferAccess access)
		{
			Fracture::Command cmd;
			cmd.fnc = [cntxt, buffer, data, size, access,offset]() {
				glNamedBufferSubData(buffer, offset, size, data.data());
			};
			cntxt->Push(cmd);
		}
		
		template<RenderTargetType E>
		void CopyRenderTarget(RenderTarget* from_buffer, RenderTarget* to_buffer,uint32_t attachment_index)
		{
			switch (E)
			{
				case RenderTargetType::ColorBuffer:
				{					
					auto srcWidth = from_buffer->Info.ColorAttachments[attachment_index].Width;
					auto srcHeight = from_buffer->Info.ColorAttachments[attachment_index].Height;

					//auto dstWidth = to->Description.ColorTextureDesc.Width;
					//auto dstHeight = to->Description.ColorTextureDesc.Height;

					/*
					glBindFramebuffer(GL_READ_FRAMEBUFFER, from->RenderID);


					glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
				

					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to->RenderID);
				

					glDrawBuffer(GL_COLOR_ATTACHMENT0);
			

					glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
				
					glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
					*/
					break;
				}
				case RenderTargetType::DepthBuffer:
				{				
					break;
				}
				case RenderTargetType::StencilBuffer:
				{					
					break;
				}
			}
		}			
	}

}



#endif