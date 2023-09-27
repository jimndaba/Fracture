#include "FracturePCH.h"
#include "SceneRenderer.h"
#include "Rendering/GraphicsDevice.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Rendergraph/Passes/ClearTargetPass.h"

#include "Rendering/Rendergraph/Passes/PresentPass.h"
#include "Rendering/Rendergraph/Passes/GridPass.h"

#include "Assets/ImagerLoader.h"
#include "Rendering/PostProcessPipeline.h"
#include "World/WorldEvents.h"
#include "World/LightProbeSystem.h"
#include "Common/Math.h"
#include "World/SceneManager.h"

Fracture::SceneRenderer::SceneRenderer()
{
}

void Fracture::SceneRenderer::Init()
{
	
	{
		{
			Fracture::RenderTargetCreationInfo info;
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA16F;
				desc.formatType = Fracture::TextureFormatType::Float;
				desc.minFilter = TextureMinFilter::Linear;
				desc.magFilter = TextureMagFilter::Linear;
				desc.Name = "GlobalColorBuffer";
				info.ColorAttachments.push_back(desc);
			}
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA8;
				desc.formatType = Fracture::TextureFormatType::UByte;
				desc.minFilter = TextureMinFilter::Near;
				desc.magFilter = TextureMagFilter::Near;
				desc.Name = "PickingBuffer";
				info.ColorAttachments.push_back(desc);
			}
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA32F;
				desc.formatType = Fracture::TextureFormatType::Float;
				desc.minFilter = TextureMinFilter::Near;
				desc.magFilter = TextureMagFilter::Near;
				desc.Name = "Normals";
				info.ColorAttachments.push_back(desc);
			}
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA32F;
				desc.formatType = Fracture::TextureFormatType::Float;
				desc.minFilter = TextureMinFilter::Near;
				desc.magFilter = TextureMagFilter::Near;
				desc.Name = "Positions";
				info.ColorAttachments.push_back(desc);
			}
			{

				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::DepthStencil;
				desc.formatType = Fracture::TextureFormatType::Float32U248;
				desc.format = Fracture::TextureFormat::DepthStencil;
				desc.internalFormat = Fracture::InternalFormat::Depth32Stencil8;
				desc.Name = "GlobalDepthBuffer";
				info.DepthStencilAttachments.push_back(desc);

			}
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_ColorBuffer", info);
		}
		{
			Fracture::RenderTargetCreationInfo info;
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA16F;
				desc.formatType = Fracture::TextureFormatType::Float;
				desc.minFilter = TextureMinFilter::Linear;
				desc.magFilter = TextureMagFilter::Linear;
				desc.Name = "GlobalGrabBuffer";
				info.ColorAttachments.push_back(desc);
			}
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_GrabBuffers", info);
		}
		{
			Fracture::RenderTargetCreationInfo info;
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA16F;
				desc.formatType = Fracture::TextureFormatType::Float;
				desc.minFilter = TextureMinFilter::Linear;
				desc.magFilter = TextureMagFilter::Linear;
				desc.Name = "Global_Outline";
				info.ColorAttachments.push_back(desc);
			}
			{

				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::DepthStencil;
				desc.formatType = Fracture::TextureFormatType::Float32U248;
				desc.format = Fracture::TextureFormat::DepthStencil;
				desc.internalFormat = Fracture::InternalFormat::Depth32Stencil8;
				desc.Name = "GlobalDepthBuffer";
				info.DepthStencilAttachments.push_back(desc);

			}
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_Outline", info);
		}
		{
			Fracture::RenderTargetCreationInfo sky_info;
			sky_info.Width = 512;
			sky_info.Height = 512;
			sky_info.HasRenderBuffer = true;	
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_SkyCaptureBuffer", sky_info);
		}
		{
			Fracture::RenderTargetCreationInfo sky_info;
			sky_info.Width = 16;
			sky_info.Height = 16;
			sky_info.HasRenderBuffer = true;
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_IrradianceBuffer", sky_info);
		}
		{
			Fracture::RenderTargetCreationInfo sky_info;
			sky_info.Width = 32;
			sky_info.Height = 32;
			Fracture::TextureCreationInfo desc;
			desc.Width = 32;
			desc.Height = 32;
			desc.TextureTarget = TextureTarget::TextureCubeMapArray;
			desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
			desc.format = Fracture::TextureFormat::RGB;
			desc.internalFormat = Fracture::InternalFormat::RGB16F;
			desc.formatType = Fracture::TextureFormatType::Float;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			desc.Name = "LightProbeTexture";
			desc.TextureArrayLevels = 100 * 6;
			desc.MipLevels = 0;
			desc.GenMinMaps = false;
			sky_info.ColorAttachments.push_back(desc);
			sky_info.HasRenderBuffer = true;
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget(GlobalRenderTargets::GlobalIrradiance, sky_info);
		}
		{
			Fracture::TextureCreationInfo desc;
			desc.Width = 512;
			desc.Height = 512;
			desc.TextureTarget = TextureTarget::TextureCubeMap;
			desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
			desc.format = Fracture::TextureFormat::RGB;
			desc.internalFormat = Fracture::InternalFormat::RGB16F;
			desc.formatType = Fracture::TextureFormatType::Float;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			desc.Name = "Global_SkyMap";
			Fracture::GraphicsDevice::Instance()->CreateGlobalTexture("Global_SkyMap", desc);
		}
		{
			Fracture::RenderTargetCreationInfo Outinfo;
			{
				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
				desc.format = Fracture::TextureFormat::RGBA;
				desc.internalFormat = Fracture::InternalFormat::RGBA8;
				desc.formatType = Fracture::TextureFormatType::Int;
				desc.minFilter = TextureMinFilter::Near;
				desc.magFilter = TextureMagFilter::Near;
				desc.Name = "GlobalOutlineBuffer";
				Outinfo.ColorAttachments.push_back(desc);
			}			
			{

				Fracture::TextureCreationInfo desc;
				desc.Width = 1920;
				desc.Height = 1080;
				desc.AttachmentTrgt = Fracture::AttachmentTarget::DepthStencil;
				desc.formatType = Fracture::TextureFormatType::UInt24_8;
				desc.format = Fracture::TextureFormat::DepthStencil;
				desc.internalFormat = Fracture::InternalFormat::Depth24Stencil8;
				desc.Name = "OutlineDepthBuffer";
				Outinfo.DepthStencilAttachments.push_back(desc);
			}
			Fracture::GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalOutline, Outinfo);
		}
	}		
	{
		RenderContextFlags flags = RenderContextFlag_DrawRenderIDs;
		mContext = std::make_shared<Fracture::RenderContext>(flags);
		mContext->ContextViewport.Height = 1080;
		mContext->ContextViewport.Width = 1920;
		mContext->ContextScissor.Height = 1080;
		mContext->ContextScissor.Width = 1920;
	}
	{
		RenderContextFlags flags = RenderContextFlag_None;
		mFinalContext = std::make_shared<Fracture::RenderContext>(flags);
		mFinalContext->ContextViewport.Height = 1080;
		mFinalContext->ContextViewport.Width = 1920;
		mFinalContext->ContextScissor.Height = 1080;
		mFinalContext->ContextScissor.Width = 1920;
	}
	
	
	Fracture::ForwardPassDef forwardpassDef;
	f_pass = std::make_shared<Fracture::ForwardPass>("ForwardPass", mContext.get(), forwardpassDef);
	f_pass->Setup();

	Fracture::ZPrePassDef PrePassDef;
	zPrePass = std::make_shared<Fracture::ZPrePass>("ZPrePass", mContext.get(), PrePassDef);
	zPrePass->Setup();

	Fracture::GridPassDef griddef;
	gridpass = std::make_shared<Fracture::GridPass>("GridPass", mContext.get(), griddef);
	gridpass->Setup();

	ssaoPass = std::make_shared<Fracture::SSAOPass>("SSAO Pass", mContext.get());
	ssaoPass->Setup();

	ssrPass = std::make_shared<Fracture::SSRPass>("SSR Pass", mContext.get());
	ssrPass->Setup();

	Fracture::OutlinePassDef outdef;
	Outline_pass = std::make_shared<Fracture::OutlinePass>("outline pas", mContext.get(), outdef);
	Outline_pass->Setup();

	Fracture::LightCullPassDef lightculldef;
	lightcullPass = std::make_shared<Fracture::LightCullPass>("LightCullingPass", mContext.get(), lightculldef);
	lightcullPass->Setup();

	Fracture::ShadowCasterPassDef shadowDef;
	shadaowPass = std::make_shared<Fracture::ShadowCasterPass>("ShadowPass", mContext.get(), shadowDef);
	shadaowPass->Setup();

	Fracture::ComposeRenderersPassDef compdef;
	ComposePass = std::make_shared<Fracture::ComposeRenderersPass>("ComposePass", mFinalContext.get(), compdef);
	ComposePass->Setup();

	PresentPassDef presentInfo;
	presentPass = std::make_shared<Fracture::PresentPass>("Present",mFinalContext.get(), presentInfo);
	presentPass->Setup();


	particleSystem = std::make_unique<Fracture::ParticleSystem>();
	particleSystem->Init();
}

void Fracture::SceneRenderer::Begin(float dt)
{

	OPTICK_EVENT();
	if (SceneManager::CurrentScene())
	{
		particleSystem->Update(dt);

		const auto& current_Camera = SceneManager::ActiveCamera();	
		//const auto& tranform = SceneManager::GetComponent<CameraComponent>(current_Camera->GetID());
		data.Camera_Position = glm::vec4(current_Camera->Position.x, current_Camera->Position.y, current_Camera->Position.z, 0);
		data.Camera_Projection = current_Camera->ProjectMatrix;
		data.Camera_View = current_Camera->ViewMatrix;
		data.Camera_InvProjection = glm::inverse(current_Camera->ProjectMatrix);
		data.Camera_InvView = glm::inverse(current_Camera->ViewMatrix);
		data.Near_Far_Width_Height = { current_Camera->Near,current_Camera->Far,1920,1080 };
		data.DeltaTimeX_PAD3.x = dt;
		

		std::vector<LightData> lightdata;
		bool hasSunLights = false;
		bool hasPointLights = false;
		bool hasSpotLights = false;

		{
			const auto& lights = SceneManager::GetAllComponents<SunlightComponent>();
			hasSunLights = !lights.empty();

			for (const auto& light : lights)
			{
				LightData D;
				D.Diffuse = glm::vec4(light->Diffuse, 1.0);


				//glm::vec3 angle = glm::normalize(glm::radians(glm::eulerAngles(SceneManager::GetComponent<TransformComponent>(light->GetID())->Rotation)));
				auto& rotation = SceneManager::GetComponent<TransformComponent>(light->GetID())->Rotation;
				glm::vec3 angle = glm::normalize(glm::vec3(rotation.x, rotation.y, rotation.z));

				D.DirectionXYZ_StrengthW = glm::vec4(angle, light->Strength);
				D.enabled = 1;
				D.type = 1;
				lightdata.push_back(D);
			}
		}
		{
			const auto& lights = SceneManager::GetAllComponents<PointlightComponent>();
			hasPointLights = !lights.empty();
			for (const auto& light : lights)
			{
				LightData D;
				D.Diffuse = glm::vec4(light->Diffuse, 1.0);
				D.DirectionXYZ_StrengthW = glm::vec4(0, 0, 0, light->Strength);

				// compute world position of light
				const auto& transform = SceneManager::GetComponent<TransformComponent>(light->GetID());
				auto& parent = SceneManager::GetComponent<HierachyComponent>(light->GetID())->Parent;
				auto& parent_transform = SceneManager::GetComponent<TransformComponent>(parent)->WorldTransform;

				glm::vec4 p = parent_transform * glm::vec4(transform->Position, 1.0);


				D.PositionXYZ_RadiusW = glm::vec4(p.x, p.y, p.z, light->Radius);
				D.ComprX_Y_Z_InnerW = glm::vec4(light->Compression, 0, 0, 0);
				D.enabled = 1;
				D.type = 2;
				lightdata.push_back(D);
			}
		}
		{
			const auto& lights = SceneManager::GetAllComponents<SpotlightComponent>();
			hasSpotLights = !lights.empty();
			for (const auto& light : lights)
			{
				LightData D;
				D.Diffuse = glm::vec4(light->Diffuse, 1.0);
				const auto& transform = SceneManager::GetComponent<TransformComponent>(light->GetID());


				// compute world position of light
				auto& parent = SceneManager::GetComponent<HierachyComponent>(light->GetID())->Parent;
				auto& parent_transform = SceneManager::GetComponent<TransformComponent>(parent)->WorldTransform;

				glm::vec4 position = parent_transform * glm::vec4(transform->Position, 1.0);			
				glm::vec3 angle = glm::normalize(glm::vec3(transform->Rotation.x, transform->Rotation.y, transform->Rotation.z));

				D.DirectionXYZ_StrengthW = glm::vec4(angle.x, angle.y, angle.z, light->Strength);
				D.PositionXYZ_RadiusW = position;
				D.ComprX_Y_Z_InnerW = glm::vec4(0, 0, 0, glm::cos(glm::radians(light->InnerCutoff)));
				D.CnstX_LnrY_QuadZ_OuterW = glm::vec4(light->Constant, light->Linear, light->Quadratic, glm::cos(glm::radians(light->OutCutoff)));
				D.enabled = 1;
				D.type = 3;
				lightdata.push_back(D);
			}
		}
		{
			int probelightIndex = 0;
			const auto& lights = SceneManager::GetAllComponents<LightProbeComponent>();			
			for (const auto& light : lights)
			{
				if (light->IsBaked && light->ProbeType == LightProbeComponent::LightProbeType::Local)
				{
					const auto& transform = SceneManager::GetComponent<TransformComponent>(light->GetID());
					for (int probe = 0; probe < light->ProbePositions.size(); probe++)
					{
						LightData D;
						D.PositionXYZ_RadiusW = light->ProbePositions[probe];
						D.enabled = 1;
						D.type = 4;
						D.ProbeID = probelightIndex;
						lightdata.push_back(D);
						probelightIndex++;
					}
				}
			}
		}

		for (const auto& probe : SceneManager::GetAllComponents<LightProbeComponent>())
		{
			if(!probe->IsBaked)
				mLightProbesToRender.push(probe->GetID());
		}

		Fracture::GraphicsDevice::Instance()->UpdateGlobalFrameData(data);
		Fracture::GraphicsDevice::Instance()->UpdateGlobalLightData(lightdata);
		Fracture::GraphicsDevice::Instance()->UpdateGlobalRenderSettings();

		mContext->deltaTime += dt;
	}



}

void Fracture::SceneRenderer::QueueLightProbesToBake(UUID id)
{
	OPTICK_EVENT();
	mLightProbesToRender.push(id);
}

void Fracture::SceneRenderer::End()
{
	OPTICK_EVENT();
	Fracture::SortKey key;
	mContext->BeginState(key);
	mContext->BeginScene();

	Fracture::ClearTargetPassDef passDef;
	passDef.ClearColor = Fracture::Colour::Black;
	passDef.BufferTarget = ((uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth | (uint32_t)Fracture::ClearFlags::Stencil);
	Fracture::ClearTargetPass cleartarget("ClearTarget", mContext.get(), passDef);
	cleartarget.Setup();



	for (const auto& skybox : SceneManager::GetAllComponents<SkyboxComponent>())
	{
		if (skybox->IsDirty)
		{
			Fracture::GraphicsDevice::Instance()->UpdateSkybox(mContext.get(), skybox.get());
			for (const auto& probe : SceneManager::GetAllComponents<LightProbeComponent>())
			{
				if (probe->AutoBaked)
					mLightProbesToRender.push(probe->GetID());
			}
		}
	}

	while (!mLightProbesToRender.empty())
	{
		LightProbeSystem s;
		s.Bake(mContext.get(), mLightProbesToRender.front());
		mLightProbesToRender.pop();
	}

	{
		cleartarget.Execute();

		if (Outline_pass)
			Outline_pass->Execute();

		if (zPrePass)
			zPrePass->Execute();		

		if (ssaoPass)
			ssaoPass->Execute();

		if (shadaowPass)
			shadaowPass->Execute();

		if (lightcullPass)
			lightcullPass->Execute();

		if (f_pass)
			f_pass->Execute();

		if (gridpass && DrawGrid)
			gridpass->Execute();

		particleSystem->Render(mContext.get());

		if (ssrPass)
			ssrPass->Execute();


		
	
	}

	mContext->EndState();

	mContext->Render();
	GraphicsDevice::Instance()->PostProcessStack()->OnRender();
	GraphicsDevice::Instance()->PostProcessStack()->Submit();

	

	Fracture::SortKey nkey;
	mFinalContext->BeginState(nkey);

	if (ComposePass)
		ComposePass->Execute();

	if (presentPass)
	   presentPass->Execute();

	mFinalContext->EndState();
	mFinalContext->Render();
	mContext->EndScene();
}

void Fracture::SceneRenderer::OnSave()
{
	particleSystem->OnSave();
}

void Fracture::SceneRenderer::OnLoad()
{
	particleSystem->OnLoad();
}
