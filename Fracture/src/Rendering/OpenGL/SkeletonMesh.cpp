#include "SkeletonMesh.h"
#include "OpenGLBase.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "BufferAttribute.h"

Fracture::SkeletonMesh::SkeletonMesh(std::vector<AnimatedVertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures) :
	m_vertices(vertices),
	m_Indices(indices),
	m_textures(textures)
{
    m_VerterArray = VertexArray::Create();

    m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size() * sizeof(unsigned int));

    m_VertexBuffer = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));

    m_VerterArray->bind();
    m_IndexBuffer->bind();
    m_VertexBuffer->bind();

    VertexBufferLayout vertexLayout;

    vertexLayout =
    {
        { ShaderDataType::Float3, "aPos" },
        { ShaderDataType::Float3, "aNormal" },
        { ShaderDataType::Float2, "aTexCoord" },
        { ShaderDataType::Float3, "aTangent" },
        { ShaderDataType::Float3, "aBiTanget" },
        { ShaderDataType::Int4,   "aBoneIds" },
        { ShaderDataType::Float4, "aWeights" },
    };

    m_VertexBuffer->SetLayout(vertexLayout);

    bind();
}

uint32_t Fracture::SkeletonMesh::RenderID()const
{
    return m_VerterArray->RenderID();
}

uint32_t Fracture::SkeletonMesh::CountOfIndices()
{
    return m_Indices.size();
}

void Fracture::SkeletonMesh::SetBufferLayout(const VertexBufferLayout& layout)
{
    m_VertexBuffer->SetLayout(layout);
}

void Fracture::SkeletonMesh::bind()
{
    uint32_t attribIndex = 0;

    auto& m_layout = m_VertexBuffer->GetLayout();

    for (const auto& attribute : m_layout.GetAttributes())
    {
        auto shadertype = attribute.Type();
        glEnableVertexAttribArray(attribIndex);

        if (shadertype == ShaderDataType::Int)
        {
            glVertexAttribIPointer(attribIndex,
                attribute.GetCount(),
                ShaderDataTypeToOpenGLBaseType(shadertype),
                m_layout.GetStride(),
                (const void*)(intptr_t)attribute.Offset());

        }
        else
        {
            glVertexAttribPointer(attribIndex,
                attribute.GetCount(),
                ShaderDataTypeToOpenGLBaseType(shadertype), GL_FALSE,
                m_layout.GetStride(),
                (const void*)(intptr_t)attribute.Offset());
        }

        attribIndex++;
    }
}

std::shared_ptr<Fracture::SkeletonMesh> Fracture::SkeletonMesh::Create(std::vector<AnimatedVertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
{
	return std::make_shared<SkeletonMesh>(vertices, indices, textures);
}
