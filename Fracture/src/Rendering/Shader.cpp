#include "Shader.h"
#include "GLAD/glad.h"
#include <iostream>
#include "Texture.h"
#include "Logging/Logger.h"

Fracture::Shader::Shader(std::string name, std::string vertexPath, std::string fragmentPath):Name(name)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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
    }
    catch (std::ifstream::failure e)
    {   
        FRACTURE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        FRACTURE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {}", infoLog);    
    };

    // Frag Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        FRACTURE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
    };

    // shader Program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    // print linking errors if any
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        FRACTURE_ERROR("ERROR::SHADER::PROGRAM LINKING_FAILED\n {}", infoLog);
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);


}

Fracture::Shader::~Shader()
{
}

void Fracture::Shader::use()
{
}

void Fracture::Shader::unbind()
{
    glUseProgram(m_program);
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
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Fracture::Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(m_program, name.c_str()),1,&value[0]);
}

void Fracture::Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Fracture::Shader::setVec4(const std::string& name, glm::vec4 value) const
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
    glBindTexture(GL_TEXTURE_2D, value->id);   
}

unsigned int Fracture::Shader::ID()
{
    return m_program;
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
