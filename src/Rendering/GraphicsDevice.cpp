#include "FracturePCH.h"
#include "GraphicsDevice.h"


#include "Common/Logger.h"

std::unique_ptr<Fracture::GraphicsDevice> Fracture::GraphicsDevice::_Instance;
uint16_t Fracture::GraphicsDevice::DRAWCALL_COUNT;

std::string ShaderTypeToString(Fracture::ShaderType tpe)
{
    if (tpe == Fracture::ShaderType::Vertex) return "Vertex";
    if (tpe == Fracture::ShaderType::Fragement) return "Fragment";
    if (tpe == Fracture::ShaderType::Geometry) return "Geometry";
    if (tpe == Fracture::ShaderType::Compute) return "Compute";
    if (tpe == Fracture::ShaderType::TessalationControl) return "Tess_Eval";
    if (tpe == Fracture::ShaderType::TessalationEvaluation) return "Tess_Cntrl";
}

GLenum ShaderDataTypeToOpenGLBaseType(Fracture::ShaderDataType type)
{
    switch (type)
    {
    case Fracture::ShaderDataType::Float:    return GL_FLOAT;
    case Fracture::ShaderDataType::Float2:   return GL_FLOAT;
    case Fracture::ShaderDataType::Float3:   return GL_FLOAT;
    case Fracture::ShaderDataType::Float4:   return GL_FLOAT;
    case Fracture::ShaderDataType::Float4Instanced:   return GL_FLOAT;
    case Fracture::ShaderDataType::Mat3:     return GL_FLOAT;
    case Fracture::ShaderDataType::Mat4:     return GL_FLOAT;
    case Fracture::ShaderDataType::Int:      return GL_INT;
    case Fracture::ShaderDataType::Int2:     return GL_INT;
    case Fracture::ShaderDataType::Int3:     return GL_INT;
    case Fracture::ShaderDataType::Int4:     return GL_INT;
    case Fracture::ShaderDataType::Bool:     return GL_BOOL;
    }
    return 0;
}


Fracture::GraphicsDevice::GraphicsDevice()
{
}

Fracture::GraphicsDevice* Fracture::GraphicsDevice::Instance()
{
    if (_Instance)
        return _Instance.get();
    else
    {
        _Instance = std::make_unique<GraphicsDevice>();
        _Instance->Startup();
        return _Instance.get();
    }
}

void Fracture::GraphicsDevice::Startup()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        FRACTURE_CRITICAL("Failed to initialize GLAD");
        return;
    }
    else
    {
        FRACTURE_INFO("OpenGL loaded");
        FRACTURE_INFO("Vendor: {}", (const char*)glGetString(GL_VENDOR));
        FRACTURE_INFO("Renderer: {}", (const char*)glGetString(GL_RENDERER));
        FRACTURE_INFO("Version: {}", (const char*)glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_SCISSOR_TEST);
    }

    {
        BufferDescription desc;
        desc.Name = "Global Frame Data Buffer";
        desc.bufferType = BufferType::UniformBuffer;
        desc.data = NULL;
        desc.size = sizeof(GlobalFrameData);
        desc.usage = BufferUsage::Stream;
        mGFrameData = std::make_shared<Buffer>();
        CreateBuffer(mGFrameData.get(), desc);
        SetBufferIndexRange(mGFrameData.get(), 0, 0);
    }
    {
        BufferDescription desc;
        desc.Name = "Global Light Data Buffer";
        desc.bufferType = BufferType::ShaderStorageBuffer;
        desc.data = NULL;
        desc.size = sizeof(LightData) * MAX_LIGHTS;
        desc.usage = BufferUsage::Stream;
        mGLightBuffer = std::make_shared<Buffer>();
        CreateBuffer(mGLightBuffer.get(), desc);
        SetBufferIndexRange(mGLightBuffer.get(), 0, 0);

        mLightData.resize(MAX_LIGHTS);
    }


}


//Update Global Frame Buffers
void Fracture::GraphicsDevice::UpdateGlobalFrameData(const GlobalFrameData& data)
{   
    GraphicsDevice::Instance()->UpdateBufferData(mGFrameData.get(), 0, sizeof(GlobalFrameData), &data);
}

//Update Global Light Buffers
void Fracture::GraphicsDevice::UpdateGlobalLightData(const std::vector<LightData>& data)
{
    GraphicsDevice::Instance()->UpdateBufferData(mGLightBuffer.get(), 0, sizeof(LightData) * data.size(), data.data());
}


void Fracture::GraphicsDevice::Shutdown()
{
}

void Fracture::GraphicsDevice::ClearBuffers(uint32_t bufferbit)
{
    glClear(bufferbit); glCheckError();
}

void Fracture::GraphicsDevice::ClearColor(float r, float g, float b, float a)
{  
    glClearColor(r, g, b, a); glCheckError();
}

void Fracture::GraphicsDevice::CreateBuffer(Buffer* buffer, const BufferDescription& desc)
{
   
    glCreateBuffers(1,&buffer->RenderID);    glCheckError();
    buffer->Description = desc;


    if (desc.data)
    {
        glNamedBufferData(buffer->RenderID, desc.size, desc.data, (GLenum)desc.usage);    glCheckError();
    }
    else
    {
        glNamedBufferData(buffer->RenderID, desc.size, NULL, (GLenum)desc.usage);    glCheckError();

    }
}

void Fracture::GraphicsDevice::SetBufferData(const Buffer& buffer, const void* data)
{
    glNamedBufferData(buffer.RenderID, buffer.Description.size, data, (GLenum)buffer.Description.usage); glCheckError();
}

void Fracture::GraphicsDevice::SetBufferIndexRange(Buffer* buffer, uint32_t index, uint32_t offset)
{
    glBindBufferRange((GLenum)buffer->Description.bufferType, index, buffer->RenderID, offset, buffer->Description.size);   glCheckError(); 
}

void Fracture::GraphicsDevice::UpdateBufferData(Buffer* buffer, uint32_t offset, uint32_t size, const void* data)
{
    glCheckError();
    GLint m_size;
    glGetBufferParameteriv((GLenum)buffer->Description.bufferType, GL_BUFFER_SIZE, &m_size); glCheckError();
    if (m_size != size)
    {
        glNamedBufferSubData(buffer->RenderID, offset, size, data);  glCheckError();
    }
    else
    {
        glNamedBufferSubData(buffer->RenderID, offset, size, data);  glCheckError();
    }

}

void Fracture::GraphicsDevice::CreateVertexArray(uint32_t& vao, const VertexArrayCreationInfo& info)
{
    glCreateVertexArrays(1, &vao);
}

void Fracture::GraphicsDevice::VertexArray_BindVertexBuffer(const uint32_t& vao, const uint32_t& bindingIndex, const uint32_t& stride, const uint32_t& VBO)
{
    glVertexArrayVertexBuffer(vao, bindingIndex, VBO, 0, stride); glCheckError();
}

void Fracture::GraphicsDevice::VertexArray_BindIndexBuffers(const uint32_t& vao, const uint32_t& IBO)
{
    glVertexArrayElementBuffer(vao, IBO); glCheckError();
}

void Fracture::GraphicsDevice::VertexArray_BindAttributes(const uint32_t& vao, const VertexArrayCreationInfo& info)
{
    uint32_t attribIndex = 0;
    uint32_t bindingIndex = 0;
    bool lastWasInterleaved = false;
    for (const auto& attribute : info.Layout.Attributes)
    {
        auto shadertype = ShaderDataTypeToOpenGLBaseType(attribute.Type);
        auto count = attribute.GetCount();
        auto stride = info.Layout.GetStride();     
            


        switch (attribute.Type)
        {
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                for (uint32_t i = 0; i < count; i++)
                {

                    glVertexArrayAttribBinding(vao, attribIndex, bindingIndex); glCheckError();
                    glVertexArrayAttribFormat(vao, attribIndex, count, shadertype, GL_FALSE, ((sizeof(float) * count) * i)); glCheckError();
                    glEnableVertexArrayAttrib(vao, attribIndex);		glCheckError();
                    if (attribute.Instanced)
                        glVertexArrayBindingDivisor(vao, attribIndex, attribute.divisor);	glCheckError();

                    attribIndex++;
                }
                bindingIndex += count;
                break;
            }           
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            {
                if (attribute.Interleaved)
                {
                    glEnableVertexArrayAttrib(vao, attribIndex);		glCheckError();
                    glVertexArrayAttribBinding(vao, attribIndex, 0); glCheckError();
                    glVertexArrayAttribIFormat(vao, attribIndex, count, shadertype, attribute.Offset); glCheckError();

                    if (attribute.Instanced)
                    {
                        glVertexArrayBindingDivisor(vao, attribIndex, attribute.divisor);	glCheckError();
                    }
                    attribIndex++;
                }
                else
                {
                    glEnableVertexArrayAttrib(vao, attribIndex);		glCheckError();
                    glVertexArrayAttribBinding(vao, attribIndex, bindingIndex); glCheckError();
                    glVertexArrayAttribIFormat(vao, attribIndex, count, shadertype, 0); glCheckError();

                    if (attribute.Instanced)
                    {
                        glVertexArrayBindingDivisor(vao, attribIndex, attribute.divisor);	glCheckError();

                    }
                    attribIndex++;
                }
                bindingIndex++;
                break;
            }
            case ShaderDataType::Bool:
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Float4Instanced:
            {
                if (attribute.Interleaved)
                {
                    glEnableVertexArrayAttrib(vao, attribIndex);		glCheckError();
                    glVertexArrayAttribBinding(vao, attribIndex, 0); glCheckError();
                    glVertexArrayAttribFormat(vao, attribIndex, count, shadertype, GL_FALSE, attribute.Offset); glCheckError();

                    if (attribute.Instanced)
                    {
                        glVertexArrayBindingDivisor(vao, attribIndex, attribute.divisor);	glCheckError();
                    }
                    attribIndex++;
                }
                else
                {
                    glEnableVertexArrayAttrib(vao, attribIndex);		glCheckError();
                    glVertexArrayAttribBinding(vao, attribIndex, bindingIndex); glCheckError();
                    glVertexArrayAttribFormat(vao, attribIndex, count, shadertype, GL_FALSE, 0); glCheckError();

                    if (attribute.Instanced)
                    {
                        glVertexArrayBindingDivisor(vao, attribIndex, attribute.divisor);	glCheckError();

                    }
                    attribIndex++;
                }
                bindingIndex++;
                break;
            }       
        }
        
    }
}

void Fracture::GraphicsDevice::CreateTexture(std::shared_ptr<Texture>& texture, const TextureCreationInfo& info)
{ 
    unsigned int target = GLenum(info.TextureTarget);
    unsigned int internalFormat =GLenum(info.internalFormat);
    unsigned int textureformat = GLenum(info.format);
    unsigned int formatType = GLenum(info.formatType);
    unsigned int MinFilter = GLenum(info.minFilter);
    unsigned int MagFilter = GLenum(info.magFilter);
    unsigned int wrap = GLenum(info.Wrap);

   
    glGenTextures(1, &texture->Handle);   glCheckError();
    glBindTexture(target, texture->Handle); glCheckError();

    glTextureParameteri(texture->Handle, GL_TEXTURE_WRAP_S, wrap);   glCheckError();
    glTextureParameteri(texture->Handle, GL_TEXTURE_WRAP_T, wrap);   glCheckError();
    if (info.TextureTarget == TextureTarget::TextureCubeMap)
        glTextureParameteri(texture->Handle, GL_TEXTURE_WRAP_R, wrap);   glCheckError();

    glTextureParameteri(texture->Handle, GL_TEXTURE_MIN_FILTER, MinFilter);   glCheckError();
    glTextureParameteri(texture->Handle, GL_TEXTURE_MAG_FILTER, MagFilter);   glCheckError();


    if (info.Wrap == TextureWrap::CampToBorder)
    {
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);
        glCheckError();
    }
    if (info.TextureTarget == TextureTarget::Texture2DArray || info.TextureTarget == TextureTarget::Texture3D)
    {

        if (info.data.size())
        {
            glTexImage3D(target, texture->Description.TextureArrayLevels, internalFormat, info.Width, info.Height, info.Depth, 0, textureformat, formatType, info.data.data());
            glCheckError();
        }
        else
        {
            glTexImage3D(target, 0, internalFormat, info.Width, info.Height, texture->Description.TextureArrayLevels, 0, textureformat, formatType, NULL); glCheckError();
        }

    }
    else
    {
        if (info.TextureTarget == TextureTarget::TextureCubeMap)
        {
            //glBindTexture(target, texture->RenderID);
            uint32_t levels = 1;
            if (info.GenMinMaps && info.MipLevels == 0)
            {
                levels = info.CaclMipLevels();
            }
            else
            {
                levels = info.MipLevels;
            }

            glTextureStorage2D(texture->Handle, levels, internalFormat, info.Width, info.Height); glCheckError();


            for (unsigned int i = 0; i < 6; ++i)
            {
                //note that we store each face with 16 bit floating point values
                glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, info.Width, info.Height, textureformat, formatType, NULL);
                glCheckError();

            }
        }
        else
        {
            if (info.data.size())
            {               
                uint32_t levels = 1;
                if (info.GenMinMaps && info.MipLevels == 0)
                {
                    levels = info.CaclMipLevels();
                }
                else
                {
                    levels = info.MipLevels;
                }
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(target, 0,internalFormat, info.Width, info.Height,0,textureformat,formatType,info.data.data()); glCheckError();
            }
            else
            {                
                uint32_t levels = 1;
                if (info.GenMinMaps && info.MipLevels == 0)
                {
                    levels = info.CaclMipLevels();
                }
                else
                {
                    levels = info.MipLevels;
                }       
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(target, 0, internalFormat, info.Width, info.Height, 0, textureformat, formatType, NULL); glCheckError();
            }
        }
    }

    if (info.GenMinMaps)
    {
        glGenerateTextureMipmap(texture->Handle);
        glCheckError();
    }

    if (info.data.size())
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      
    }
    glBindTexture(target, 0);
       
}

void Fracture::GraphicsDevice::CreateGlobalTexture(const std::string& Name, const TextureCreationInfo& info)
{
    auto texture = std::make_shared<Texture>(info);
    CreateTexture(texture, info);
    mGlobalResources[Name] = texture;
}

Fracture::RenderTarget* Fracture::GraphicsDevice::GetGlobalRenderTarget(const std::string& Name)
{
    auto it = mGlobalResources.find(Name);
    if (it == mGlobalResources.end())
        return nullptr;

    return static_cast<RenderTarget*>(mGlobalResources[Name].get());
}

std::shared_ptr<Fracture::Shader> Fracture::GraphicsDevice::CreateShader(const ShaderDescription& desc)
{
    auto shader = std::make_shared<Shader>(desc);
    shader->Handle = glCreateProgram();

    bool loaded = true;
    std::vector<unsigned int> mShaders;

    if (!desc.VertexPath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.VertexPath, ShaderType::Vertex);
        if (handle)
        {
            mShaders.push_back(handle);
        }
    }

    if (!desc.FragmentPath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.FragmentPath, ShaderType::Fragement);
        if (handle)
        {
            mShaders.push_back(handle);

        }
        else
        {
            loaded = false;
        }
    }

    if (!desc.ComputePath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.ComputePath, ShaderType::Compute);
        if (handle)
        {
            mShaders.push_back(handle);
        }
        else
        {
            loaded = false;
        }
    }

    if (!desc.GeometryPath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.GeometryPath, ShaderType::Geometry);
        if (handle)
        {
            mShaders.push_back(handle);
        }
        else
        {
            loaded = false;
        }
    }

    if (!desc.TesselationControlPath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.TesselationControlPath, ShaderType::TessalationControl);
        if (handle)
        {
            mShaders.push_back(handle);
        }
        else
        {
            loaded = false;
        }
    }

    if (!desc.TesselationEvaluationPath.empty())
    {
        auto handle = CompileShader(desc.Name, desc.TesselationEvaluationPath, ShaderType::TessalationEvaluation);
        if (handle)
        {
            mShaders.push_back(handle);
        }
        else
        {
            loaded = false;
        }
    }

    if (loaded)
    {
        for (const auto& mshader : mShaders)
        {
            AttachShaderToProgram(mshader, shader->Handle);
        }
        glLinkProgram(shader->Handle); glCheckError();;

        return std::move(shader);
    }

    return nullptr;
}

void Fracture::GraphicsDevice::AttachShaderToProgram(const unsigned int& shader, const unsigned int& program)
{
    if (program && shader)
    {
        glAttachShader(program, shader);
        glCheckError();
    }
    else
    {
      FRACTURE_ERROR("Cannot Attach program or program");
    }

}

std::shared_ptr<Fracture::RenderTarget> Fracture::GraphicsDevice::CreateRenderTarget(const RenderTargetCreationInfo info)
{
    std::shared_ptr<Fracture::RenderTarget> target = std::make_shared<Fracture::RenderTarget>();
   
    glCreateFramebuffers(1, &target->Handle);
    glCheckError();

    if (!info.ColorAttachments.empty())
    {
        for (unsigned int n = 0; n < info.ColorAttachments.size(); n++)
        {          
            auto texture = std::make_shared<Texture>(info.ColorAttachments[n]);
            CreateTexture(texture, info.ColorAttachments[n]);

            glNamedFramebufferTexture(target->Handle, GLenum(info.ColorAttachments[n].AttachmentTrgt) + n, texture->Handle,0); glCheckError();
    
            target->ColorAttachments.push_back(texture);
        }
        
        if (!info.DepthStencilAttachments.empty())
        {
            target->DepthStencilAttachment = std::make_shared<Texture>(info.DepthStencilAttachments[0]);
            CreateTexture(target->DepthStencilAttachment, info.DepthStencilAttachments[0]);
            glNamedFramebufferTexture(target->Handle, GLenum(info.DepthStencilAttachments[0].AttachmentTrgt), target->DepthStencilAttachment->Handle, 0);  glCheckError();           
        }

    }
    else
    {
        if (!info.DepthStencilAttachments.empty())
        {
            for (unsigned int n = 0; n < info.DepthStencilAttachments.size(); n++)
            {
                TextureCreationInfo t_info;
                target->DepthStencilAttachment = std::make_shared<Texture>(info.DepthStencilAttachments[n]);
                CreateTexture(target->DepthStencilAttachment, info.DepthStencilAttachments[n]);
                glCheckError();
                glNamedFramebufferTexture(target->Handle, GLenum(info.Target), target->DepthStencilAttachment->Handle, 0);  glCheckError();
            }
        }
    }

    SetDrawBuffers(target);

    auto fboState = glCheckNamedFramebufferStatus(target->Handle, GL_FRAMEBUFFER);
    if (fboState != GL_FRAMEBUFFER_COMPLETE)
    {
        FRACTURE_ERROR("Framebuffer incomplete: {}", fboState);
    }

    return target;
}

void Fracture::GraphicsDevice::CreateGlobalRenderTarget(const std::string& Name, const RenderTargetCreationInfo& info)
{
    auto rt = CreateRenderTarget(info);
    mGlobalResources[Name] = rt;
}

void Fracture::GraphicsDevice::SetDrawBuffers(std::shared_ptr<RenderTarget>& rt)
{
    if (!rt->ColorAttachments.empty())
    {
        unsigned int attachments[10];
        for (int i = 0; i < rt->ColorAttachments.size(); i++)
        {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glNamedFramebufferDrawBuffers(rt->Handle, rt->ColorAttachments.size(), attachments);   glCheckError();

    }
    else
    {
        glNamedFramebufferDrawBuffer(rt->Handle, GL_NONE);   glCheckError();
        glNamedFramebufferReadBuffer(rt->Handle, GL_NONE);   glCheckError();

    }
}

void Fracture::GraphicsDevice::CHECKGLERRRORS()
{
    glCheckError();
}

unsigned int Fracture::GraphicsDevice::CompileShader(const std::string& name, const std::string& path, ShaderType shadertype)
{
    unsigned int handle;

    std::string  shadercode;
    std::ifstream ShaderFile;

    // ensure ifstream objects can throw exceptions:
    ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        ShaderFile.open(path);

        std::stringstream vShaderStream;
        // read file's buffer contents into streams
        vShaderStream << ShaderFile.rdbuf();

        // close file handlers
        ShaderFile.close();

        // convert stream into string
        shadercode = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        FRACTURE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = shadercode.c_str();

    handle = glCreateShader(GLenum(shadertype));
    glCheckError();

    glShaderSource(handle, 1, &vShaderCode, NULL);
    glCheckError();

    glCompileShader(handle);
    checkCompileErrors(name, handle, ShaderTypeToString(shadertype));
    glCheckError();

    return handle;
}

void Fracture::GraphicsDevice::checkCompileErrors(const std::string& name, unsigned int shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            FRACTURE_ERROR("ERROR::SHADER_COMPILATION_ERROR {} of type: {} , {} ",name,type ,infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            FRACTURE_ERROR("ERROR::PROGRAM_LINKING_ERROR {} of type: {} , {} ", name, type, infoLog);
        }
    }
}
