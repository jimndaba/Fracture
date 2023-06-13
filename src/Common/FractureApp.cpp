#include "FracturePCH.h"
#include "FractureApp.h"
#include "Common/AppWindow.h"
#include "glad/glad.h"

std::unique_ptr<Fracture::AppWindow>  Fracture::IFractureApp::mWindow = nullptr;

Fracture::IFractureApp::IFractureApp()
{
}

Fracture::IFractureApp::~IFractureApp()
{
}

bool Fracture::IFractureApp::ShouldWindowClose()
{
    return mWindow->ShouldWindowClose();
}

bool Fracture::IFractureApp::CreateAppWindow(const WindowCreationInfo* info)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    mWindow = std::make_unique<AppWindow>(*info);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 0); // 4x antialiasing
    glfwWindowHint(GLFW_MAXIMIZED, info->IsMaximised);

    glfwWindowHint(GLFW_RESIZABLE, info->IsResizable);
    glfwWindowHint(GLFW_DECORATED, info->IsBordered);

    if (info->IsFullscreen)
    {
        mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_DECORATED, info->IsBordered);

        mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, NULL, NULL);
    }


    if (!mWindow->Context)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(mWindow->Context);  

    glfwSwapInterval(1);

    mWindow->Init();
   
    return true;
}

