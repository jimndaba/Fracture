#pragma once
#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "IGraphicsResource.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderTarget.h"
#include "RenderContext.h"
#include "PostProcessingParams.h"
#include "World/WindSystem.h"

#include <map>

namespace Fracture
{
	struct RenderContext;

	struct GlobalRenderTargets
	{
		static std::string GlobalColour;
		static std::string GlobalSSAO;
		static std::string GlobalSSR;
		static std::string GlobalDebug;
		static std::string GlobalDirectShadows;
		static std::string GlobalSpotShadows;
		static std::string GlobalPointShadows;
		static std::string GlobalFinalOut;
		static std::string GlobalOutline;
		static std::string GlobalIrradiance;
		static std::string GlobalGrabs;
	};

	enum class ShaderUniformIndex
	{
		GlobalFrameData,		
		GlobalRenderSettings,
		SSAO_Kernel,
		ShadowMatrix,
		ShadowPlanes,
		GlobalWindData,
		GlobalGPUMaterials,
		IndirectBuffer
	};

	enum class ShaderStorageBufferIndex
	{
		LightList,
		ClusterAABB,
		ScreenToView,
		LightIndexSSBO,
		LightGridSSBO,
		GlobalIndexCountSSBO,
		Debuglines,
		AnimationData,
		TerrainHeightMap
	};

	enum class GlobalColorAttachments
	{
		Color,
		Picking,
		Normal,
		Position,
		Emissive
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
		glm::vec4 DeltaTimeX_PAD3;		
	};

	struct LightData
	{
		glm::vec4 DirectionXYZ_StrengthW = glm::vec4(0);
		glm::vec4 PositionXYZ_RadiusW = glm::vec4(0); //Radius for Point lights 
		glm::vec4 ComprX_Y_Z_InnerW = glm::vec4(0);
		glm::vec4 CnstX_LnrY_QuadZ_OuterW = glm::vec4(0);
		glm::vec4 Diffuse = glm::vec4(0);
		//0_ OFF, 1 _ ON
		int enabled = 0;
		// 0 _ invalid ,1 _ Direc, 2 _ Point , 3 _ Spot , 4 _LocalProbe
		int type = 0; 
		int ProbeID = -1;
		int _pad;
	};

	struct PostProcessPipeline;
	struct SkyboxComponent;

	class GraphicsDevice
	{
		static std::unique_ptr<GraphicsDevice> _Instance;

		std::map<std::string, std::shared_ptr<IGraphicsResource>> mGlobalResources;

		std::unordered_map<UUID, std::shared_ptr<Texture>> mIrradianceMaps;
		std::unordered_map<UUID, std::shared_ptr<Texture>> mBRDFMaps;
		std::shared_ptr<Texture> BRDFLUT;
		bool IsBRDFCreated = false;

		std::shared_ptr<Buffer> mGFrameData;
		std::shared_ptr<Buffer> mGWindData;
		std::shared_ptr<Buffer> mIndirectBuffer;
		std::shared_ptr<Buffer> mGLightBuffer;
		std::shared_ptr<Buffer> mAnimationData;
		std::shared_ptr<Buffer> mPostProcessingBuffer;
		std::shared_ptr<Buffer> mGPUMaterialBuffer;
		std::shared_ptr<PostProcessPipeline> mPostProcessPipeline;
		std::shared_ptr<Texture> mLightProbeArray;
		std::shared_ptr<Texture> mLightProbeIrradianceArray;

		const int MAX_LIGHTS = 1024;
		const int MAX_BONES_PER_MESH = 120;
		std::vector<LightData> mLightData;
		uint32_t cubeVAO;
		uint32_t QuadVAO;
		uint32_t cubeVBO;

	public:
		GraphicsDevice();

		static GraphicsDevice* Instance();


		void Startup();
		void UpdateGlobalFrameData(GlobalFrameData data);
		void UpdateGlobalRenderSettings();
		void UpdateGlobalLightData(const std::vector<LightData>& data);
		void UpdateAnimationData(const std::vector<glm::mat4>& data);
		void UpdateMaterialData(const std::vector<GPUMaterial>& data);
		
		template<typename T>
		void UpdateTexture(Fracture::UUID id,const std::vector<T>& data,float x = 0,float y = 0, int width = 1, int height = 1);


		void UpdateFloatTexture(Fracture::UUID id);

		void UpdateIndirectBuffer(const std::vector<DrawElementsIndirectCommand>& data);
		uint32_t GetIndirectBuffer();
		void UpdateGlobalWindData();
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
		UUID CreateIrradianceMap(const TextureCreationInfo& info);
		UUID CreateBRDFMap(const TextureCreationInfo& info);
		UUID CreateBRDFLUT(const TextureCreationInfo& info);
		void CreateLightprobeMap(int Resolution);
		uint32_t GetIrradianceMap(UUID entity_id);
		uint32_t GetSpecularBRDFMap(UUID entity_id);
		uint32_t GetBRDFLUTMap(UUID entity_id);
		uint32_t GetQUADVAO(); 
		uint32_t GetLightProbeMap(); 
		uint32_t GetLightProbeIrradiance(); 

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
		static uint16_t RENDERBATCH_COUNT;

		static GlobalPostProcessParams RenderSettings;

		static WindSystemData WindSettings;

		static void SaveScreenShot(uint32_t fb,uint32_t attachment_index, int width, int height, const std::string& path);

		static void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar* message, const void* userParam);

		PostProcessPipeline* PostProcessStack();
		
		int Viewport_Width = 1920;
		int Viewport_Height = 1080;
		const int MAX_POINTLIGHTS = 30;
		const int MAX_LIGHTPROBES = 30;
		bool EnablePostProsessing = true;

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