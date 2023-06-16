#include "FracturePCH.h"
#include "DebugRenderer.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "RenderCommands.h"
#include "AABB.h"

std::vector<Fracture::Line> Fracture::DebugRenderer::mLines;
std::vector<glm::vec4> Fracture::DebugRenderer::mLinesColors;
std::map<uint32_t, Fracture::BillboardData> Fracture::DebugRenderer::mBillboardDrawCalls;
int Fracture::DebugRenderer::g_numCircleVertices = 50;

Fracture::DebugRenderer::DebugRenderer()
{
}

void Fracture::DebugRenderer::OnInit()
{
	mLines.reserve(MaxLines);

    VertexArrayCreationInfo info;
    info.Layout =
    {
    };

    GraphicsDevice::Instance()->CreateVertexArray(LineVAO, info);
    {
        BufferDescription desc;
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.size = MaxLines * sizeof(Line);
        desc.data = mLines.data();
        desc.usage = BufferUsage::Dynamic;
        mlineBufferSSBO = std::make_shared<Buffer>();
        mlineBufferSSBO->Description = desc;
        GraphicsDevice::Instance()->CreateBuffer(mlineBufferSSBO.get(), mlineBufferSSBO->Description);
        GraphicsDevice::Instance()->SetBufferIndexRange(mlineBufferSSBO.get(), (int)ShaderStorageBufferIndex::Debuglines, 0);
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
        desc.Name = "DebugBuffer";
        info.ColorAttachments.push_back(desc);

        GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDebug, info);
    }

    mContext = std::make_unique<RenderContext>();
}



void Fracture::DebugRenderer::OnRender()
{

    GraphicsDevice::Instance()->UpdateBufferData(mlineBufferSSBO.get(), 0, sizeof(Line) * MaxLines, mLines.data());


    mContext->BeginState(SortKey());
    mContext->Begin();

    const auto& target =  GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDebug);
    if (target)
    {

        
        RenderCommands::SetRenderTarget(mContext.get(),target);
        RenderCommands::SetViewport(mContext.get(),1920, 1080,0,0);
        RenderCommands::ClearColor(mContext.get(), glm::vec4(0, 0, 0, 0));
        RenderCommands::ClearTarget(mContext.get(), ClearFlags::Color);
      
       
        if (mLines.size() > 0)
        {
            const auto& mShader = AssetManager::GetShader("Debug");
            RenderCommands::UseProgram(mContext.get(), mShader->Handle);
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

void  Fracture::DebugRenderer::DrawAABB(const AABB& aabb, const glm::vec4& color)
{
    DrawAABB(aabb.min, aabb.max);
}

void Fracture::DebugRenderer::DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color)
{
    glm::vec3 boundPoint1 = min;
    glm::vec3 boundPoint2 = max;
    glm::vec3 boundPoint3 = glm::vec3(boundPoint1.x, boundPoint1.y, boundPoint2.z);
    glm::vec3 boundPoint4 = glm::vec3(boundPoint1.x, boundPoint2.y, boundPoint1.z);
    glm::vec3 boundPoint5 = glm::vec3(boundPoint2.x, boundPoint1.y, boundPoint1.z);
    glm::vec3 boundPoint6 = glm::vec3(boundPoint1.x, boundPoint2.y, boundPoint2.z);
    glm::vec3 boundPoint7 = glm::vec3(boundPoint2.x, boundPoint1.y, boundPoint2.z);
    glm::vec3 boundPoint8 = glm::vec3(boundPoint2.x, boundPoint2.y, boundPoint1.z);



    DrawLine(boundPoint6, boundPoint2, color);
    DrawLine(boundPoint2, boundPoint8, color);
    DrawLine(boundPoint8, boundPoint4, color);
    DrawLine(boundPoint4, boundPoint6, color);

    DrawLine(boundPoint3, boundPoint7, color);
    DrawLine(boundPoint7, boundPoint5, color);
    DrawLine(boundPoint5, boundPoint1, color);
    DrawLine(boundPoint1, boundPoint3, color);

    DrawLine(boundPoint6, boundPoint3, color);
    DrawLine(boundPoint2, boundPoint7, color);
    DrawLine(boundPoint8, boundPoint5, color);
    DrawLine(boundPoint4, boundPoint1, color);
}

void Fracture::DebugRenderer::DrawCircle(const glm::vec3& center, const float& radius, const glm::vec3& rotation, const glm::vec4& color)
{
    float twicePi = 2.0f * 3.14159265359f;
   

    for (int i = 0; i < g_numCircleVertices; i++) {

        float angle = twicePi * i / g_numCircleVertices;
        float rotatedAngleX = angle + rotation.x;
        float rotatedAngleY = angle + rotation.y;
        float rotatedAngleZ = angle + rotation.z;

        float x = center.x + radius * std::cos(rotatedAngleX);
        float y = center.y + radius * std::sin(rotatedAngleY);
        float z = center.z;

        Line p;
       // p.Point = glm::vec4(center.x + (radius * cos(i * twicePi / 50)), center.y + (radius * sin(i * twicePi / 50)), center.z, 1.0f);
        p.Point = glm::vec4(x, y, z, 1.0f);
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


void Fracture::DebugRenderer::DrawCone(const glm::vec3& pos, const glm::quat& rotation, float length, float radius, const glm::vec4& color)
{
    int detail = 16;
    std::vector<glm::vec3> vertices;
    vertices.reserve(detail);
    float twicePi = 2.0f * 3.14159265359f;
    

    glm::vec3 topVertex = pos + (rotation * glm::vec3(0, length, 0));

    for (std::size_t i = 0; i <= detail; ++i)
    {
        float r = twicePi * (i / (float)detail);
        glm::vec3 vertex(radius * cos(r), 0.0f, radius * sin(r));
        vertices.push_back(pos + (rotation * vertex));
    }

    for (std::size_t i = 0; i < detail; ++i)
    {
        DrawLine(vertices[i], vertices[i + 1], color);
        DrawLine(vertices[i], topVertex, color);
    }    



}
/*
void DrawContext::DrawCone(Transform const& transform, Radians coneAngle, float length, Float4 const& color, float thickness, DepthTestState depthTestState, Seconds TTL)
{
    Vector const capOffset = (transform.GetForwardVector() * length);
    float const coneCapRadius = Math::Tan(coneAngle.ToFloat()) * length;

    Transform capTransform = transform;
    capTransform.SetScale(coneCapRadius);
    capTransform.AddTranslation(capOffset);

    // Draw cone cap
    //-------------------------------------------------------------------------

    auto verts = EE_STACK_ARRAY_ALLOC(Vector, g_numCircleVertices);
    for (auto i = 0; i < g_numCircleVertices; i++)
    {
        verts[i] = capTransform.TransformPoint(g_circleVerticesYUp[i]);
    }

    // Register line commands
    for (auto i = 1; i < g_numCircleVertices; i++)
    {
        InternalDrawLine(m_commandBuffer, transform.GetTranslation(), verts[i], color, thickness, depthTestState, TTL);
        InternalDrawLine(m_commandBuffer, verts[i - 1], verts[i], color, thickness, depthTestState, TTL);
    }

    InternalDrawLine(m_commandBuffer, transform.GetTranslation(), verts[0], color, thickness, depthTestState, TTL);
    InternalDrawLine(m_commandBuffer, verts[0], verts[g_numCircleVertices - 1], color, thickness, depthTestState, TTL);
}


    void DrawContext::InternalDrawCylinderOrCapsule( bool isCapsule, Transform const& worldTransform, float radius, float halfHeight, Float4 const& color, float thickness, DepthTestState depthTestState, Seconds TTL )
    {
        Vector const axisX = worldTransform.GetAxisX();
        Vector const axisY = worldTransform.GetAxisY();
        Vector const axisZ = worldTransform.GetAxisZ();

        Vector const origin = worldTransform.GetTranslation();
        Vector const halfHeightOffset = ( axisZ * halfHeight );

        Vector const cylinderCenterTop = origin + halfHeightOffset;
        Vector const cylinderCenterBottom = origin - halfHeightOffset;

        // 8 lines
        //-------------------------------------------------------------------------

        Vector xOffset = ( axisX * radius );
        Vector lt0 = cylinderCenterTop + xOffset;
        Vector lt1 = cylinderCenterTop - xOffset;
        Vector lb0 = cylinderCenterBottom + xOffset;
        Vector lb1 = cylinderCenterBottom - xOffset;

        DrawLine( lt0, lb0, color, thickness, depthTestState, TTL );
        DrawLine( lt1, lb1, color, thickness, depthTestState, TTL );

        Vector yOffset = ( axisY * radius );
        Vector lt2 = cylinderCenterTop + yOffset;
        Vector lt3 = cylinderCenterTop - yOffset;
        Vector lb2 = cylinderCenterBottom + yOffset;
        Vector lb3 = cylinderCenterBottom - yOffset;

        DrawLine( lt2, lb2, color, thickness, depthTestState, TTL );
        DrawLine( lt3, lb3, color, thickness, depthTestState, TTL );

        Vector xzOffset0 = ( axisX + axisY ).GetNormalized3() * radius;
        Vector lt4 = cylinderCenterTop + xzOffset0;
        Vector lt5 = cylinderCenterTop - xzOffset0;
        Vector lb4 = cylinderCenterBottom + xzOffset0;
        Vector lb5 = cylinderCenterBottom - xzOffset0;

        DrawLine( lt4, lb4, color, thickness, depthTestState, TTL );
        DrawLine( lt5, lb5, color, thickness, depthTestState, TTL );

        Vector xzOffset1 = ( axisX - axisY ).GetNormalized3() * radius;
        Vector lt6 = cylinderCenterTop + xzOffset1;
        Vector lt7 = cylinderCenterTop - xzOffset1;
        Vector lb6 = cylinderCenterBottom + xzOffset1;
        Vector lb7 = cylinderCenterBottom - xzOffset1;

        DrawLine( lt6, lb6, color, thickness, depthTestState, TTL );
        DrawLine( lt7, lb7, color, thickness, depthTestState, TTL );

        // Caps
        //-------------------------------------------------------------------------

        DrawCircle( Transform( worldTransform.GetRotation(), cylinderCenterTop ), Axis::Z, radius, color, thickness, depthTestState, TTL );
        DrawCircle( Transform( worldTransform.GetRotation(), cylinderCenterBottom ), Axis::Z, radius, color, thickness, depthTestState, TTL );

        //-------------------------------------------------------------------------

        if ( isCapsule )
        {
            Radians const radiansPerEdge( Math::TwoPi / g_numCircleVertices );

            auto DrawSemiCircle = [&]( Vector const& startPoint, Vector const& capCenterPoint, Vector const& shapeOrigin )
            {
                Vector planeVector = startPoint - capCenterPoint;
                Vector const rotationAxis = ( startPoint - capCenterPoint ).Cross3( startPoint - shapeOrigin ).GetNormalized3();
                Quaternion const rotation( rotationAxis, radiansPerEdge );

                Vector prevPlaneVector = planeVector;
                for ( auto c = 0; c < g_numCircleVertices / 2; c++ )
                {
                    prevPlaneVector = planeVector;
                    planeVector = rotation.RotateVector( planeVector );
                    DrawLine( capCenterPoint + prevPlaneVector, capCenterPoint + planeVector, color, thickness, depthTestState, TTL );
                }
            };

            DrawSemiCircle( lt0, cylinderCenterTop, origin );
            DrawSemiCircle( lt2, cylinderCenterTop, origin );
            DrawSemiCircle( lt4, cylinderCenterTop, origin );
            DrawSemiCircle( lt6, cylinderCenterTop, origin );

            DrawSemiCircle( lb0, cylinderCenterBottom, origin );
            DrawSemiCircle( lb2, cylinderCenterBottom, origin );
            DrawSemiCircle( lb4, cylinderCenterBottom, origin );
            DrawSemiCircle( lb6, cylinderCenterBottom, origin );
        }
    }


*/