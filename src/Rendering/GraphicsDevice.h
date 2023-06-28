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
#include "PostProcessingParams.h"
#include <map>

namespace Fracture
{
	struct GlobalRenderTargets
	{
		static std::string GlobalColour;
		static std::string GlobalSSAO;
		static std::string GlobalSSR;
		static std::string GlobalDebug;
		static std::string GlobalDirectShadows;
		static std::string GlobalFinalOut;
	};

	enum class ShaderUniformIndex
	{
		GlobalFrameData,		
		GlobalRenderSettings,
		SSAO_Kernel,
		ShadowMatrix,
		ShadowPlanes
	};

	enum class ShaderStorageBufferIndex
	{
		LightList,
		ClusterAABB,
		ScreenToView,
		LightIndexSSBO,
		LightGridSSBO,
		GlobalIndexCountSSBO,
		Debuglines
	};

	enum class GlobalColorAttachments
	{
		Color,
		Picking,
		Normal,
		Position
	};


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
		glm::mat4 Camera_InvProjection;
		glm::mat4 Camera_View;
		glm::mat4 Camera_InvView;
		glm::vec4 Camera_Position;
		glm::vec4 Near_Far_Width_Height;
	};

	struct LightData
	{
		glm::vec4 DirectionXYZ_StrengthW = glm::vec4(0);
		glm::vec4 PositionXYZ_RadiusW = glm::vec4(0); //Radius for Point lights 
		glm::vec4 ComprX_Y_Z_InnerW = glm::vec4(0);
		glm::vec4 CnstX_LnrY_QuadZ_OuterW = glm::vec4(0);
		glm::vec4 Diffuse = glm::vec4(0);
		int enabled = 0;//0_ OFF, 1 _ ON
		int type = 0; // 0 _ invalid ,1 _ Direc, 2 _ Point , 3 _ Spot
		glm::vec2 _pad;
	};

	struct PostProcessPipeline;
	struct SkyboxComponent;

	class GraphicsDevice
	{
		static std::unique_ptr<GraphicsDevice> _Instance;

		std::map<std::string, std::shared_ptr<IGraphicsResource>> mGlobalResources;

		std::shared_ptr<Buffer> mGFrameData;
		std::shared_ptr<Buffer> mGLightBuffer;
		std::shared_ptr<Buffer> mPostProcessingBuffer;
		std::shared_ptr<PostProcessPipeline> mPostProcessPipeline;

		const int MAX_LIGHTS = 1024;
		std::vector<LightData> mLightData;
		uint32_t cubeVAO;
		uint32_t cubeVBO;

	public:
		GraphicsDevice();

		static GraphicsDevice* Instance();


		void Startup();
		void UpdateGlobalFrameData(const GlobalFrameData& data);
		void UpdateGlobalRenderSettings();
		void UpdateGlobalLightData(const std::vector<LightData>& data);
		void Shutdown();

		void ClearBuffers(uint32_t bufferbit);
		void ClearColor(float r, float g, float b, float a);

		void CreateBuffer(Buffer* buffer, const BufferDescription& desc);
		void SetBufferData(const Buffer& buffer, const void* data);
		void SetBufferIndexRange(Buffer* buffer, uint32_t index, uint32_t offset);
		void UpdateBufferData(Buffer* buffer, uint32_t offset, uint32_t size, const void* data);
		void ClearBufferData(Buffer* buffer);
	

		void CreateVertexArray(uint32_t& vao, const VertexArrayCreationInfo& info);
		void VertexArray_BindVertexBuffer(const uint32_t& vao, const uint32_t& bindingIndex, const uint32_t& stride, const uint32_t& VBO ,const uint32_t offset = 0);
		void VertexArray_BindIndexBuffers(const uint32_t& vao, const uint32_t& IBO);
		void VertexArray_SetDivisor(const uint32_t& vao, const uint32_t& attributeindex,const uint32_t divisor);
		void VertexArray_BindAttributes(const uint32_t& vao, const VertexArrayCreationInfo& info);

		void CreateTexture(std::shared_ptr<Texture>& texture, const TextureCreationInfo& info);
		void CreateGlobalTexture(const std::string& Name, const TextureCreationInfo& info);

		void UpdateSkybox(RenderContext* Context, SkyboxComponent* component);
		void RenderCaptureCube(RenderContext* Context);

		RenderTarget* GetGlobalRenderTarget(const std::string& Name);
		Texture* GetGlobalTexture(const std::string& Name);

		std::shared_ptr<Fracture::Shader> CreateShader(const ShaderDescription& desc);
		void AttachShaderToProgram(const unsigned int& shader, const unsigned int& program);
		void RecompileShader(Fracture::Shader* shader);

		std::shared_ptr<RenderTarget> CreateRenderTarget(const RenderTargetCreationInfo info);
		void CreateGlobalRenderTarget(const std::string& Name, const RenderTargetCreationInfo& info);
		void SetDrawBuffers(std::shared_ptr<RenderTarget>& rt);

		void CHECKGLERRRORS();

		static uint16_t DRAWCALL_COUNT;

		static GlobalPostProcessParams RenderSettings;

		PostProcessPipeline* PostProcessStack();
		
		int Viewport_Width = 1920;
		int Viewport_Height = 1080;

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