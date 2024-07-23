#pragma once
#ifndef RENDERCOMMANDS_H
#define RENDERCOMMANDS_H

#include "Command.h"
#include "Color.h"
#include "RenderTarget.h"
#include <glad/glad.h>

namespace Fracture
{
	struct Scissor;
	struct Viewport;
	struct Shader;
	struct Material;
	struct RenderContext;


	enum class GLCapability;

	enum class DepthSortOrder
	{
		Front_To_Back,
		Back_To_Front
	};

	enum class DrawMode : uint32_t
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineLoops = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Patches = GL_PATCHES
	};

	enum class DrawCommandType
	{
		DrawArrys,
		DrawElements,
		DrawElementsInstancedBaseVertex,
		MultiDrawElementsIndirect
	};


	enum ClearFlags : uint32_t
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};
	
	enum BufferAccess
	{
		WriteOnly = GL_WRITE_ONLY,
		ReadOnly = GL_READ_ONLY,
		ReadWrite = GL_READ_WRITE
	};

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

	enum class BlendEq
	{
		Func_ADD = GL_FUNC_ADD,
		Func_Subtract = GL_FUNC_SUBTRACT,
		Func_Rev_Subtract = GL_FUNC_REVERSE_SUBTRACT
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
		OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
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
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
		Float4Instanced
	};

	enum class BufferUsage
	{
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream = GL_STREAM_DRAW,
		StaticCopy = GL_STATIC_COPY
	};

	enum class BufferType
	{
		ArrayBuffer = GL_ARRAY_BUFFER,
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
		UniformBuffer = GL_UNIFORM_BUFFER,
		TextureBuffer = GL_TEXTURE_BUFFER,
		FrameBuffer = GL_FRAMEBUFFER,
		ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
		DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER
	};

	
	struct DrawArray
	{
		DrawMode mode;
		uint32_t first;
		uint32_t count;
	};

	struct DrawArraysInstanced
	{
		DrawMode mode;
		uint32_t first;
		uint32_t count;
		uint32_t instance_count;
	};

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
		GLuint  count;
		GLuint  instanceCount;
		GLuint  firstIndex;
		GLuint  baseVertex;
		GLuint  baseInstance;
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
		void SetViewport(Fracture::RenderContext* cntxt, float width, float height, float x = 0, float y = 0 );
		void SetScissor(Fracture::RenderContext* cntxt, float width, float height, float x = 0, float y = 0);
		void SetRenderTarget(Fracture::RenderContext* cntxt, RenderTarget* rt);
		void SetRenderTarget(Fracture::RenderContext* cntxt, uint32_t rt);
		void SetRenderBuffer(Fracture::RenderContext* cntxt, uint32_t rb);
		void ReleaseRenderTarget(Fracture::RenderContext* cntxt);
		void SetCullMode(Fracture::RenderContext* cntxt, CullMode mode);

		void DepthFunction(Fracture::RenderContext* cntxt, DepthFunc fnc);
		void DepthMask(Fracture::RenderContext* cntxt,bool mask);
		void StencilMask(Fracture::RenderContext* cntxt, uint32_t mask);
		void BlendFunction(Fracture::RenderContext* cntxt, BlendFunc sfactor, BlendFunc dfactor);
		void BlendEquation(Fracture::RenderContext* cntxt, BlendEq eq);
		void StencilFunction(Fracture::RenderContext* cntxt, StencilFunc fnc, int ref, uint32_t mask);
		void StencilOperation(Fracture::RenderContext* cntxt, StencilOp_TestResult sfail, StencilOp_TestResult dfail, StencilOp_TestResult sdpass);
		void SetColorMask(Fracture::RenderContext* cntxt, bool r, bool g, bool b, bool a);

		void BindVertexArrayObject(Fracture::RenderContext* cntxt, uint32_t vao);
		void BindVertexArrayVertexBuffer(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t index, uint32_t stride, uint32_t buffer, uint32_t offset = 0);
		void BindVertexArrayIndexBuffer(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t buffer);
		void BindVertexArraySetDivisor(Fracture::RenderContext* cntxt, uint32_t vao,uint32_t AttributeIndex, uint32_t divisor);
		void BindBuffer(Fracture::RenderContext* cntxt, Fracture::BufferType target, uint32_t buffer);
		void ClearBufferData(Fracture::RenderContext* cntxt, uint32_t buffer);
		
		void DispatchComputeShader(Fracture::RenderContext* cntxt, uint16_t x, uint16_t y, uint16_t z);
		void Barrier(Fracture::RenderContext* cntxt);

		void FramBufferTexture(Fracture::RenderContext* cntxt, uint32_t attachment_index, uint32_t attachment_target, uint32_t texture, uint32_t level);
		void FrameBufferTextureTarget(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t attachment_layer, uint32_t texture, uint32_t level);

		void FrameBufferDepthTextureTarget(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t attachment_layer, uint32_t texture, uint32_t level);

		void FrameBufferAttachTexture(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t texture, uint32_t level);
		void FrameBufferAttachTexture3D(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t attachment_index, uint32_t face, uint32_t texture, uint32_t level, uint32_t layer);		
		void FrameBufferSetDrawBuffers(Fracture::RenderContext* cntxt, uint32_t fb, uint32_t NoBuffers);
		void RenderBufferTextureStorage(Fracture::RenderContext* cntxt, uint32_t rb, InternalFormat internal, int x, int y);
		void CheckFrameBuffer(Fracture::RenderContext* cntxt, uint32_t fb);

		void BlitFramebuffer(Fracture::RenderContext* cntxt,const std::string& From, const std::string& to);
		void BlitDepthFramebuffer(Fracture::RenderContext* cntxt,const std::string& From, const std::string& to);
		void BlitFramebuffer(Fracture::RenderContext* cntxt, uint32_t fb_from, uint32_t fb_to);
		
		void DrawArray(Fracture::RenderContext* cntxt, const Fracture::DrawArray& cmd);
		void DrawArrayInstanced(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstanced& cmd);
		void DrawElementsArray(Fracture::RenderContext* cntxt,const Fracture::DrawElementsArray& cmd);
		void DrawElementsArrayInstanced(Fracture::RenderContext* cntxt,const Fracture::DrawElementsArraysInstanced& cmd);
		void DrawElementsBaseVertex(Fracture::RenderContext* cntxt,const Fracture::DrawElementsBaseVertex& cmd);
		void DrawElementsInstancedBaseVertex(Fracture::RenderContext* cntxt, const Fracture::DrawElementsInstancedBaseVertex& cmd);
		void DrawElementsIndirect(Fracture::RenderContext* cntxt, void* indirect, int count  = 0, int stride = 0);
		void DrawArraysInstancedBaseInstance(Fracture::RenderContext* cntxt, const Fracture::DrawArraysInstancedBaseInstance& cmd);

	    void MapDataFrombuffer(void* ptr,uint32_t buffer,BufferAccess access = BufferAccess::WriteOnly);

		template<class T>
		void UploadDataTobuffer(void* ptr, std::vector<T>& data, uint32_t size);

		template<class T>
	    void BufferSubData(Fracture::RenderContext* cntxt,uint32_t buffer, std::vector<T>& data, uint32_t size, uint32_t offset,BufferAccess access = BufferAccess::WriteOnly);


		void UnMapbuffer(uint32_t buffer);

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
		void UploadDataTobuffer(void* ptr, std::vector<T>& data, uint32_t size)
		{
			Fracture::Command cmd;
			cmd.fnc = [ptr, data, size]() {
				
				memcpy(ptr, data.data(), size);
			};
			cmd.fnc();
		}

		template<class T>
		void BufferSubData(Fracture::RenderContext* cntxt, uint32_t buffer, std::vector<T>& data, uint32_t size, uint32_t offset, BufferAccess access)
		{
			Fracture::Command cmd;
			cmd.fnc = [cntxt, buffer, data, size, access,offset]() {
				glNamedBufferSubData(buffer, offset, size, data.data());
			};
			cmd.fnc();
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