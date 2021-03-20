#include "Shader.h"
#include "OpenGL/Texture.h"
#include "Logging/Logger.h"
#include <GLAD/glad.h>

const char* getTypeString(GLenum type) {
    // There are many more types than are covered here, but
    // these are the most common in these examples.
    switch (type) {
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_DOUBLE:
        return "double";
    case GL_INT:
        return "int";
    case GL_UNSIGNED_INT:
        return "unsigned int";
    case GL_BOOL:
        return "bool";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    default:
        return "?";
    }
}

Fracture::Shader::Shader(const std::string& name,const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath):Name(name),vertPath(vertexPath),fragPath(fragmentPath)
{
    m_program = createShaderFromFile(vertexPath, fragmentPath,geometryPath);
}

Fracture::Shader::~Shader()
{

}

void Fracture::Shader::use()
{
    if (m_program)
    {
        glUseProgram(m_program);
    }
    else
    {
        FRACTURE_ERROR("{} : SHADER IS NULL",Name);        
    }
}

void Fracture::Shader::unbind()
{
    glUseProgram(0);
}

void Fracture::Shader::reloadShader()
{
    if (m_program &&  !vertPath.empty() && !fragPath.empty())
    {
        unsigned int reloaded_program;
        if (!geoPath.empty())
        {
            reloaded_program = createShaderFromFile(vertPath, fragPath, geoPath);
        }
        else
        {
           reloaded_program = createShaderFromFile(vertPath, fragPath,"");
        }

        if (reloaded_program)
        {
            glDeleteProgram(m_program);
            m_program = reloaded_program;
        }
    }
}

void Fracture::Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
}

void Fracture::Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Fracture::Shader::setFloat(const std::string& name, float value) const
{  
    glUseProgram(m_program);
    auto location = glGetUniformLocation(m_program, name.c_str());
    if (location != -1)
        glUniform1f(location, value);
    else
    {
    }
        //FRACTURE_ERROR("Uniform '{0}' not found!", name);

}

void Fracture::Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(m_program, name.c_str()),1,&value[0]);
}

void Fracture::Shader::setVec3(const std::string& name, glm::vec3 value) const
{   
    glUseProgram(m_program);
    auto location = glGetUniformLocation(m_program, name.c_str());
    if (location != -1)
        glUniform3fv(location, 1, &value[0]);
    else
    {
    }
        //FRACTURE_ERROR("Uniform '{0}' not found!", name);
}

void Fracture::Shader::setVec4(const std::string& name, glm::vec4 value) const
{
    glUseProgram(m_program);
    auto location = glGetUniformLocation(m_program, name.c_str());
    if (location != -1)
        glUniform4fv(location, 1, &value[0]);
    else
    {
    }
        //FRACTURE_ERROR("Uniform '{0}' not found!", name);
}

void Fracture::Shader::setColor3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Fracture::Shader::setColor4(const std::string& name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Fracture::Shader::setMat2(const std::string& name, glm::mat2 value) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Fracture::Shader::setMat3(const std::string& name, glm::mat3 value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Fracture::Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Fracture::Shader::setTexture(const std::string& name, const Texture* value, unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), unit);
    glBindTexture(GL_TEXTURE_2D, value->GetTextureID());   
}

void Fracture::Shader::setTexture(const std::string& name, const unsigned int value, unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), unit);
    glBindTexture(GL_TEXTURE_2D, value);
}

void Fracture::Shader::setCubeMap(const std::string& name, const unsigned int value, unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, value);
}

unsigned int Fracture::Shader::ID()
{
    return m_program;
}

unsigned int Fracture::Shader::createShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    unsigned int program;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (!geometryPath.empty())
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure e)
    {
        FRACTURE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    //int success;
    //char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    checkCompileErrors(vertex, "VERTEX");

    // Frag Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (!geometryPath.empty())
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    // shader Program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    if (!geometryPath.empty())
        glAttachShader(program, geometry);

    glLinkProgram(program);
    // print linking errors if any
    checkCompileErrors(program, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!geometryPath.empty())
    {
        glDeleteShader(geometry);
    }

    return program;
}

void Fracture::Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            FRACTURE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {} \n {} \n ", type , infoLog, "-- --------------------------------------------------- -- ");
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);           
            FRACTURE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {} \n {} \n ", type, infoLog, "-- --------------------------------------------------- -- ");
        }
    }
}
