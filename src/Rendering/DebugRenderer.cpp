#include "FracturePCH.h"
#include "DebugRenderer.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "RenderCommands.h"

std::vector<Fracture::Line> Fracture::DebugRenderer::mLines;
std::vector<glm::vec4> Fracture::DebugRenderer::mLinesColors;
std::map<uint32_t, Fracture::BillboardData> Fracture::DebugRenderer::mBillboardDrawCalls;

Fracture::DebugRenderer::DebugRenderer()
{
}

void Fracture::DebugRenderer::OnInit()
{
	mLines.reserve(MaxLines);

    VertexArrayCreationInfo info;
    info.Layout =
    {
        { ShaderDataType::Float3,"aPos" }
    };
    GraphicsDevice::Instance()->CreateVertexArray(LineVAO, info);
    {
        BufferDescription desc;
        desc.bufferType = BufferType::UniformBuffer;
        desc.size = MaxLines * sizeof(Line);
        desc.data = mLines.data();
        desc.usage = BufferUsage::Dynamic;
        mlineBufferSSBO = std::make_shared<Buffer>();
        mlineBufferSSBO->Description = desc;
        GraphicsDevice::Instance()->CreateBuffer(mlineBufferSSBO.get(), mlineBufferSSBO->Description);
        GraphicsDevice::Instance()->SetBufferIndexRange(mlineBufferSSBO.get(), 1, 0);
    }
    GraphicsDevice::Instance()->VertexArray_BindAttributes(LineVAO, info);
    {
        RenderTargetCreationInfo info;
      
        Fracture::TextureCreationInfo desc;
        desc.Width = 1920;
        desc.Height = 1080;
        desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
        desc.format = Fracture::TextureFormat::RGBA;
        desc.internalFormat = Fracture::InternalFormat::RGBA8;
        desc.formatType = Fracture::TextureFormatType::UByte;
        desc.Name = "ColorBuffer";
        info.ColorAttachments.push_back(desc);

        GraphicsDevice::Instance()->CreateGlobalRenderTarget("Debug", info);
    }

    mContext = std::make_unique<RenderContext>();
}



void Fracture::DebugRenderer::OnRender()
{

    GraphicsDevice::Instance()->UpdateBufferData(mlineBufferSSBO.get(), 0, sizeof(Line) * MaxLines, mLines.data());


    mContext->BeginState(SortKey());
    mContext->Begin();

    const auto& target =  GraphicsDevice::Instance()->GetGlobalRenderTarget("Debug");
    if (target)
    {

        const auto& mShader = AssetManager::GetShader("Debug");
        RenderCommands::SetRenderTarget(mContext.get(),target);
        RenderCommands::SetViewport(mContext.get(),1920, 1080,0,0);
        RenderCommands::ClearColor(mContext.get(), glm::vec4(0, 0, 0, 0));
        RenderCommands::ClearTarget(mContext.get(), ClearFlags::Color);
        RenderCommands::UseProgram(mContext.get(), mShader->Handle);
       
        if (mLines.size() > 0)
        {
            RenderCommands::BindVertexArrayObject(mContext.get(), LineVAO);
         
            glLineWidth(3.0f);
            DrawArray cmd;
            cmd.count = mLines.size();
            cmd.first = 0;
            cmd.mode = DrawMode::Lines;
            RenderCommands::DrawArray(mContext.get(), cmd);


            RenderCommands::UseProgram(mContext.get(), 0);
            RenderCommands::BindVertexArrayObject(mContext.get(), 0);

        }

        if (mBillboardDrawCalls.size() > 0)
        {

        }


        mLines.clear();
        mBillboardDrawCalls.clear();
    }
    RenderCommands::ReleaseRenderTarget(mContext.get());
    mContext->EndState();
    mContext->Sort(Fracture::DepthSortOrder::Front_To_Back);
    mContext->Render();
}

void Fracture::DebugRenderer::OnShutdown()
{
}

void Fracture::DebugRenderer::OnReloadShaders()
{
}

void Fracture::DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
    Line line;
    line.Point = glm::vec4(start, 1.0f);
    line.Color = color;
    mLines.push_back(line);

    Line lineEnd;
    lineEnd.Point = glm::vec4(end, 1.0f);
    lineEnd.Color = color;
    mLines.push_back(lineEnd);
}

void Fracture::DebugRenderer::DrawCircle(const glm::vec3& center, const float& radius, const glm::vec4& color)
{
    float twicePi = 2.0f * 3.14159265359f;
    for (int i = 0; i < 50; i++) {
        Line p;
        p.Point = glm::vec4(center.x + (radius * cos(i * twicePi / 50)), center.y + (radius * sin(i * twicePi / 50)), center.z, 1.0f);
        p.Color = color;
        mLines.push_back(p);
    }
}

void Fracture::DebugRenderer::DrawBillboard(const uint32_t& texture, const glm::vec3& position, const glm::vec4& color)
{
    if (mBillboardDrawCalls.find(texture) != mBillboardDrawCalls.end())
    {
        mBillboardDrawCalls[texture].Positions.push_back(glm::vec4(position, 1.0f));
        mBillboardDrawCalls[texture].Colors.push_back(color);
    }
    else
    {
        BillboardData data;
        data.Image = texture;
        data.Positions.push_back(position);
        data.Colors.push_back(color);
        mBillboardDrawCalls[texture] = data;
    }
}

void Fracture::DebugRenderer::DrawSphere(const glm::vec3& center, const float& radius, const glm::vec4& color)
{
    float twicePi = 2.0f * 3.14159265359f;

    for (int i = 0; i < 50; i++) {
        Line p;
        p.Color = color;
        p.Point = glm::vec4(center.x, center.y + (radius * sin(i * twicePi / 50)), center.z + (radius * cos(i * twicePi / 50)), 1.0f);
        mLines.push_back(p);
    }
    for (int i = 0; i < 50; i++) {
        Line p;
        p.Color = color;
        p.Point = glm::vec4(center.x + (radius * cos(i * twicePi / 50)), center.y, center.z + (radius * sin(i * twicePi / 50)), 1.0f);
        mLines.push_back(p);
    }
    for (int i = 0; i < 50; i++) {
        Line p;
        p.Color = color;
        p.Point = glm::vec4(center.x + (radius * cos(i * twicePi / 50)), center.y + (radius * sin(i * twicePi / 50)), center.z, 1.0f);
        mLines.push_back(p);
    }
}

void Fracture::DebugRenderer::DrawArrow(const glm::vec3& pos, const float& angle, const glm::vec4& color, const float& length, const float& tipSize, const float& width)
{
    auto angleRot = glm::angleAxis(angle, glm::vec3(0, 1.0, 0));
    auto dir = angleRot * glm::vec3(0, 0, -1.0);
    DrawArrow(pos, dir, color, length, tipSize, width);
}

void Fracture::DebugRenderer::DrawArrow(const glm::vec3& pos, const glm::vec2& direction, const glm::vec4& color, const float& length, const float& tipSize, const float& width)
{
    auto dir = glm::vec3(direction.x, 0.0f, direction.y);
    DrawArrow(pos, dir, color, length, tipSize, width);
}

void Fracture::DebugRenderer::DrawArrow(const glm::vec3& pos, const glm::vec3& dir, const glm::vec4& color, const float& length, const float& tipSize, const float& width)
{
    auto direction = glm::normalize(dir);

    float sideLen = length - length * tipSize;
    auto widthOffset = glm::cross(direction, glm::vec3(0, 1.0, 0)) * width;

    auto baseLeft = pos + widthOffset * 0.3f;
    auto baseRight = pos - widthOffset * 0.3f;
    auto tip = pos + direction * length;
    auto upCornerInRight = pos - widthOffset * 0.3f + direction * sideLen;
    auto upCornerInLeft = pos + widthOffset * 0.3f + direction * sideLen;
    auto upCornerOutRight = pos - widthOffset * 0.5f + direction * sideLen;
    auto upCornerOutLeft = pos + widthOffset * 0.5f + direction * sideLen;

    DrawLine(baseLeft, baseRight, color);
    DrawLine(baseRight, upCornerInRight, color);
    DrawLine(upCornerInRight, upCornerOutRight, color);
    DrawLine(upCornerOutRight, tip, color);
    DrawLine(tip, upCornerOutLeft, color);
    DrawLine(upCornerOutLeft, upCornerInLeft, color);
    DrawLine(upCornerInLeft, baseLeft, color);
}
