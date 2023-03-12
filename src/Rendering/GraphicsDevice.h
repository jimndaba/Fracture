#pragma once
#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "IGraphicsResource.h"
#include "PipelineState.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderTarget.h"
#include "RenderCommands.h"
#include <map>

namespace Fracture
{
	enum class ClearBufferBit : uint32_t
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};

	enum class GraphicsCapability : uint32_t
	{
		CullFace = GL_CULL_FACE,
		DepthTest = GL_DEPTH_TEST,
		StencilTest = GL_STENCIL_TEST,
		TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
		MultiSample = GL_MULTISAMPLE,
		Blending = GL_BLEND,
		ClipDistance0 = GL_CLIP_DISTANCE0,
		Texture2D = GL_TEXTURE_2D
	};


	struct GlobalFrameData
	{
		glm::mat4 Camera_Projection;
		glm::mat4 Camera_View;
		glm::vec4 Camera_Position;
	};



	class GraphicsDevice
	{
		static std::unique_ptr<GraphicsDevice> _Instance;

		std::map<std::string, std::shared_ptr<IGraphicsResource>> mGlobalResources;

		std::shared_ptr<Buffer> mGFrameData;

	public:
		GraphicsDevice();

		static GraphicsDevice* Instance();


		void Startup();
		void UpdateGlobalFrameData(const GlobalFrameData& data);
		void Shutdown();

		void ClearBuffers(uint32_t bufferbit);
		void ClearColor(float r, float g, float b, float a);

		void CreateBuffer(Buffer* buffer, const BufferDescription& desc);
		void SetBufferData(const Buffer& buffer, const void* data);
		void SetBufferIndexRange(Buffer* buffer, uint32_t index, uint32_t offset);
		void UpdateBufferData(Buffer* buffer, uint32_t offset, uint32_t size, const void* data);

		void CreateVertexArray(uint32_t& vao, const VertexArrayCreationInfo& info);
		void VertexArray_BindVertexBuffer(const uint32_t& vao, const uint32_t& bindingIndex, const uint32_t& stride, const uint32_t& VBO);
		void VertexArray_BindIndexBuffers(const uint32_t& vao, const uint32_t& IBO);
		void VertexArray_BindAttributes(const uint32_t& vao, const VertexArrayCreationInfo& info);

		void CreateTexture(std::shared_ptr<Texture>& texture, const TextureCreationInfo& info);
		void CreateGlobalTexture(const std::string& Name, const TextureCreationInfo& info);

		RenderTarget* GetGlobalRenderTarget(const std::string& Name);

		std::shared_ptr<Fracture::Shader> CreateShader(const ShaderDescription& desc);
		void AttachShaderToProgram(const unsigned int& shader, const unsigned int& program);


		std::shared_ptr<RenderTarget> CreateRenderTarget(const RenderTargetCreationInfo info);
		void CreateGlobalRenderTarget(const std::string& Name, const RenderTargetCreationInfo& info);
		void SetDrawBuffers(std::shared_ptr<RenderTarget>& rt);

		void CHECKGLERRRORS();

		static uint16_t DRAWCALL_COUNT;

		unsigned int CompileShader(const std::string& name, const std::string& path, ShaderType shadertype);
		void checkCompileErrors(const std::string& name, unsigned int shader, const std::string& type);
		GLenum glCheckError_(const char* file, int line)
		{
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
				FRACTURE_ERROR("{} | {} (  {} )", error, file, line);
			}
			return errorCode;
		}
		#define glCheckError() glCheckError_(__FILE__, __LINE__) 
	};



}


#endif