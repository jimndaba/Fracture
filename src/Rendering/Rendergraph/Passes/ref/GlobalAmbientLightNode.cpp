#include "pch.h"
#include "GlobalAmbientLightNode.h"
#include "rendering/Device.h"
#include "assets/TextureLoader.h"
#include "rendering/Pipeline.h"
#include "core/Application.h"
#include "assets/AssetManager.h"

Fracture::GlobalAmbientLightNode::GlobalAmbientLightNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :
	RenderNode(context, name, inputs, outputs)
{
}

Fracture::GlobalAmbientLightNode::~GlobalAmbientLightNode()
{
}

void Fracture::GlobalAmbientLightNode::Setup()
{
	IsDirty = true;
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\ibl\\CubeMap.vert";
		desc.FragmentPath = "Content\\shaders\\ibl\\EqToCubemap.frag";
		EqToCubemapShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\ibl\\Skybox.vert";
		desc.FragmentPath = "Content\\shaders\\ibl\\Skybox.frag";
		//desc.GeometryPath = "Content\\shaders\\ibl\\Skybox.geom";
		Skybox = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\ibl\\CubeMap.vert";
		desc.FragmentPath = "Content\\shaders\\ibl\\Irradiance.frag";		
		IrradianceShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\ibl\\Brdf.vert";
		desc.FragmentPath = "Content\\shaders\\ibl\\Brdf.frag";
		BrdfShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\ibl\\CubeMap.vert";
		desc.FragmentPath = "Content\\shaders\\ibl\\prefilter.frag";
		prefilterShader = Device::CreateShader(desc);
	}
	{
		hdrtexture = AssetManager::GetTextureByName("DefaultHDR")->Description.ID;
		IsDirty = true;
	}
	{
		TextureDescription desc;
		desc.Name = "BRDF";
		desc.AttachmentTrgt = AttachmentTarget::Color;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.format = Format::RG;
		desc.formatType = FormatType::Float;
		desc.internalFormat = InternalFormat::RG16F;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = 512;
		desc.Height = 512;
		
		BrdfTexture = std::make_unique<Texture>(desc);
		Device::CreateTexture(BrdfTexture.get());

		AddResource(std::move(BrdfTexture));
	}
	{
		RenderBufferDescription rdesc;
		rdesc.Width = 512;
		rdesc.Height = 512;
		rdesc.IsMultiSampled = false;
		rdesc.AttachmentTrgt = AttachmentTarget::Depth;
		rdesc.internalFormat = InternalFormat::DepthComponent24;	

		TextureDescription tdesc;
		tdesc.AttachmentTrgt = AttachmentTarget::Color;
		tdesc.TextureTarget = TextureTarget::TextureCubeMap;
		tdesc.format = Format::RGB;
		tdesc.IsResizable = false;
		tdesc.Width = 512;
		tdesc.Height = 512;
		tdesc.formatType = FormatType::Float;
		tdesc.internalFormat = InternalFormat::RGB16F;
		tdesc.minFilter = TextureMinFilter::LinearMipMapLinear;
		tdesc.magFilter = TextureMagFilter::Linear;
		tdesc.GenMinMaps = true;
		tdesc.MipLevels = 0;
		tdesc.Wrap = TextureWrap::ClampToEdge;


		RenderTargetDescription desc;
		desc.Name = "CaptureFBO";
		desc.HasRenderBuffer = true;
		desc.NoColorAttachments = 1;	
		desc.RenderBufferDescription = rdesc;
		desc.ColorAttachmentDescriptions = {tdesc};
		CaptureFBO =  Device::CreateRenderTarget_Complete(desc);
	}
	{
		ResourceDescription desc;
		desc.Name = "IrradianceMap";
		desc.Type = ResourceType::RenderTarget;
		desc.HasDepthStencil = false;
		desc.HasRenderBuffer = true;
		desc.IsResizable = false;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.magFilter = TextureMagFilter::Linear;
		desc.minFilter = TextureMinFilter::Linear;
		desc.format = Format::RGB;
		desc.formartType = FormatType::Float;
		desc.GenMinMaps = false;
		desc.Width = 32;
		desc.Height = 32;
		desc.TextureTarget = TextureTarget::TextureCubeMap;
		desc.TextureAttachmentTarget = AttachmentTarget::Color;
		desc.TextureInternalformat = InternalFormat::RGB16F;

		desc.ColorBufferInternalformats = { InternalFormat::RGB16F };
		desc.RenderbufferAttachmentTarget = AttachmentTarget::Depth;
		desc.RenderBufferInternalformat = InternalFormat::DepthComponent24;
			
		AddResource(desc);
	}
	{
		ResourceDescription desc;
		desc.Name = "preFilterMap";
		desc.Type = ResourceType::RenderTarget;
		desc.HasDepthStencil = false;
		desc.HasRenderBuffer = true;
		desc.IsResizable = false;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.magFilter = TextureMagFilter::Linear;
		desc.minFilter = TextureMinFilter::LinearMipMapLinear;
		desc.GenMinMaps = true;
		desc.MipLevels = 5;
		desc.format = Format::RGB;
		desc.formartType = FormatType::Float;
		desc.Width = 128;
		desc.Height = 128;
		desc.TextureTarget = TextureTarget::TextureCubeMap;
		desc.TextureAttachmentTarget = AttachmentTarget::Color;
		desc.TextureInternalformat = InternalFormat::RGB16F;

		desc.ColorBufferInternalformats = { InternalFormat::RGB16F };
		desc.RenderbufferAttachmentTarget = AttachmentTarget::Depth;
		desc.RenderBufferInternalformat = InternalFormat::DepthComponent24;

		AddResource(desc);
	}
}

void Fracture::GlobalAmbientLightNode::Run(Scene& scene, CameraComponent* camera)
{
	const auto& eqShader = EqToCubemapShader.get();
	const auto& hdrtex = AssetManager::GetTextureByID(hdrtexture);
	const auto& capturefbo = CaptureFBO.get();
	const auto& cubeText = CaptureFBO->ColorAttachments[0]->RenderID;

	const auto& skyshader = Skybox.get();

	Context.SetUniforms([=]() {
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);		
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		});

	if (IsDirty)
	{	
		// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
		// ----------------------------------------------------------------------------------------------
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		{	
			Context.BindRenderTarget(capturefbo->RenderID);

			Context.BindShader(eqShader->Program);

			Context.SetUniforms([=]() { eqShader->SetTexture("equirectangularMap",hdrtex->RenderID ,0);});

			Context.SetUniforms([=](){ eqShader->SetUniform("projection", captureProjection);});

			Context.SetUniforms([=]()
				{
					eqShader->SetUniform("projection", captureProjection);								
				});

			Context.SetViewport(0, 0, 512, 512);
			
			for (unsigned int i = 0; i < 6; ++i)
			{
				Context.SetUniforms([=]() { eqShader->SetUniform("view", captureViews[i]);});

				Context.SetUniforms([=]() {
					glNamedFramebufferTextureLayer(capturefbo->RenderID, GL_COLOR_ATTACHMENT0, cubeText, 0, i);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					renderCube();
					});

				// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
												
			};			

			Context.BindRenderTarget(0);	

			Context.SetUniforms([=]() {
				glGenerateTextureMipmap(cubeText);		
			});

		}
		{
			const auto& IrradianceFBO = Context.GetRenderWriteTarget("IrradianceMap");
			Context.BindRenderTarget(IrradianceFBO->RenderID);
			Context.BindShader(IrradianceShader->Program);
			Context.SetUniforms([=]() { IrradianceShader->SetTexture("environmentMap", cubeText, 0); });
			Context.SetUniforms([=]() { IrradianceShader->SetUniform("projection", captureProjection); });
			Context.SetViewport(0, 0, 32,32);
			for (unsigned int i = 0; i < 6; ++i)
			{
				Context.SetUniforms([=]() { IrradianceShader->SetUniform("view", captureViews[i]); });

				Context.SetUniforms([=]() {
					glNamedFramebufferTextureLayer(IrradianceFBO->RenderID, GL_COLOR_ATTACHMENT0, IrradianceFBO->ColorAttachments[0]->RenderID, 0, i);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					renderCube();
					});
			}
		}	
		
		{
			const auto& prefilterFBO = Context.GetRenderWriteTarget("preFilterMap");
			Context.BindRenderTarget(prefilterFBO->RenderID);
			Context.BindShader(prefilterShader->Program);
			Context.SetUniforms([=]() { prefilterShader->SetTexture("environmentMap", cubeText, 0); });
			Context.SetUniforms([=]() { prefilterShader->SetUniform("projection", captureProjection); });
			
			unsigned int maxMipLevels = 5;
			for (int mip = 0; mip < maxMipLevels; mip++)
			{
				// reisze framebuffer according to mip-level size.
				unsigned int mipWidth = 128 * std::pow(0.5, mip);
				unsigned int mipHeight = 128 * std::pow(0.5, mip);			

				Context.SetViewport(0, 0, mipWidth, mipHeight);
				Context.SetUniforms([=]() {
					glBindRenderbuffer(GL_RENDERBUFFER, prefilterFBO->RenderBuffers[0]->Handle);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
					});

				float roughness = (float)mip / (float)(maxMipLevels - 1);			
				Context.SetUniforms([=]() { prefilterShader->SetUniform("roughness", roughness); });

				for (unsigned int i = 0; i < 6; ++i)
				{
					Context.SetUniforms([=]() { prefilterShader->SetUniform("view", captureViews[i]); });
					Context.SetUniforms([=]() {
						glNamedFramebufferTextureLayer(prefilterFBO->RenderID, GL_COLOR_ATTACHMENT0, prefilterFBO->ColorAttachments[0]->RenderID,mip,i);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						renderCube();
						});
				}
			}
		}

		{
			const auto& brdf = Context.GetRenderTexture("BRDF");
			Context.BindRenderTarget(CaptureFBO->RenderID);
			Context.SetViewport(0, 0, 512, 512);
			
			Context.SetUniforms([=]() {
				glBindRenderbuffer(GL_RENDERBUFFER, CaptureFBO->RenderBuffers[0]->Handle);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
				});

			Context.SetUniforms([=]() {
				glNamedFramebufferTexture(CaptureFBO->RenderID, GL_COLOR_ATTACHMENT0,brdf->RenderID, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				
				});

			Context.BindShader(BrdfShader->Program);
			Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
			Context.DrawArrays(DrawMode::Triangle, 0, 3);
		}
	}

	{
		const auto& backbuffer = Context.GetRenderWriteTarget("BackBufferColor");		
		const auto& prefil = Context.GetRenderReadTarget("preFilterMap")->ColorAttachments[0].get();
		Context.BindRenderTarget(backbuffer->RenderID);
		Context.SetViewport(0, 0, backbuffer->ColorAttachments[0]->Description.Width, backbuffer->ColorAttachments[0]->Description.Height);
		Context.BindShader(skyshader->Program);
		Context.SetUniforms([=]() { 			
		//	skyshader->SetTexture("environmentMap", cubeText, 0);
			skyshader->SetTexture("environmentMap", prefil->RenderID, 0);
			skyshader->SetUniform("Blur", Blur);
			renderCube();
			});
		Context.BindRenderTarget(0);

		Context.SetUniforms([=]() {
			for (const auto& texture : Context.GetRenderWriteTarget("BackBufferColor")->ColorAttachments)
				glGenerateTextureMipmap(texture->RenderID);

				glGenerateTextureMipmap(Context.GetRenderWriteTarget("BackBufferColor")->DepthStencil->RenderID);

			});

	}


	Context.SetUniforms([]() { glDepthFunc(GL_LESS); glDepthMask(GL_TRUE); 	glEnable(GL_CULL_FACE); });
	IsDirty = false;
}

void Fracture::GlobalAmbientLightNode::ReloadShader()
{
}

void Fracture::GlobalAmbientLightNode::AddHDRTexture(TextureRegistry& reg)
{
	hdrtexture = reg.ID;
	TextureLoader loader;
	auto texture = loader.LoadHDR(reg.Description);
	Device::CreateTexture(texture.get());
	AssetManager::AddTexture(std::move(texture)); 
	IsDirty = true;
}



void Fracture::GlobalAmbientLightNode::ChangeHDRTexture(const UUID& texture_ID)
{
	hdrtexture = texture_ID;
	IsDirty = true;
}

void Fracture::GlobalAmbientLightNode::renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
}
