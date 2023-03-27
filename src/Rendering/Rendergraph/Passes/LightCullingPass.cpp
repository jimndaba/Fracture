#include "FracturePCH.h"
#include "LightCullingPass.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"

Fracture::LightCullPass::LightCullPass(const std::string& name, RenderContext* context, const LightCullPassDef& info):IPass(name,context),Properties(info)
{
}

void Fracture::LightCullPass::Setup()
{
	{
        BufferDescription desc;
        desc.Name = "Volume Grid SSBO";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = NULL;
        desc.size = sizeof(VolumeTileAABB) * Properties.numClusters;
        desc.usage = BufferUsage::StaticCopy;
        VolumeGridSSBO = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(VolumeGridSSBO.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(VolumeGridSSBO.get(), 1, 0);
	}
    {
        Properties.sizeX = (unsigned int)std::ceilf(1920 / (float)Properties.gridSizeX);
        Properties.screen2View.screenWidth = 1920;
        Properties.screen2View.screenHeight = 1080;
        Properties.screen2View.tileSizes[0] = Properties.gridSizeX;
        Properties.screen2View.tileSizes[1] = Properties.gridSizeY;
        Properties.screen2View.tileSizes[2] = Properties.gridSizeZ;
        Properties.screen2View.tileSizes[3] = Properties.sizeX;
        Properties.screen2View.sliceScalingFactor = (float)Properties.gridSizeZ / std::log2f(SceneManager::ActiveCamera()->Far / SceneManager::ActiveCamera()->Near);
        Properties.screen2View.sliceBiasFactor = -((float)Properties.gridSizeZ * std::log2f(SceneManager::ActiveCamera()->Near) / std::log2f(SceneManager::ActiveCamera()->Far / SceneManager::ActiveCamera()->Near));


        BufferDescription desc;
        desc.Name = "ScreenToView SSBO";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = &Properties.screen2View;
        desc.size = sizeof(ScreenToView);
        desc.usage = BufferUsage::StaticCopy;
        ScreenToViewSSBO = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(ScreenToViewSSBO.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(ScreenToViewSSBO.get(), 2, 0);
    }
    {
        unsigned int totalNumLights = Properties.numClusters * Properties.maxLightsPerTile; //50 lights per tile max
        BufferDescription desc;
        desc.Name = "Light Index SSBO";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = NULL;
        desc.size = sizeof(unsigned int) * totalNumLights;
        desc.usage = BufferUsage::StaticCopy;
        lightIndexListSSBO = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(lightIndexListSSBO.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(lightIndexListSSBO.get(), 3, 0);
    }
    {       
        BufferDescription desc;
        desc.Name = "Light Grid SSBO";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = NULL;
        desc.size = Properties.numClusters * 2 * sizeof(unsigned int);
        desc.usage = BufferUsage::StaticCopy;
        lightGridSSBO = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(lightGridSSBO.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(lightGridSSBO.get(), 4, 0);
    }
    {     
        BufferDescription desc;
        desc.Name = "lightIndexGlobal Count SSBO";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = NULL;
        desc.size = sizeof(unsigned int);
        desc.usage = BufferUsage::StaticCopy;
        lightIndexGlobalCountSSBO = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(lightIndexGlobalCountSSBO.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(lightIndexGlobalCountSSBO.get(), 5, 0);
    }
    {
        mClusterComputeShader = AssetManager::GetShader("ClusterCompute");
        mLightCullComputeShader = AssetManager::GetShader("LightCullCompute");
    }

}

void Fracture::LightCullPass::Execute()
{
    if (dirty)
    {
        RenderCommands::UseProgram(Context, mClusterComputeShader->Handle);
        RenderCommands::DispatchComputeShader(Context, (float)Properties.gridSizeX, (float)Properties.gridSizeY, (float)Properties.gridSizeZ);
        dirty = false;
    }

    RenderCommands::UseProgram(Context, mLightCullComputeShader->Handle);
    RenderCommands::DispatchComputeShader(Context,1,1, 6);

}
