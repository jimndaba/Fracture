#include "FracturePCH.h"
#include "FractureApp.h"
#include "Common/AppWindow.h"
#include "glad/glad.h"

//std::unique_ptr<Fracture::AppWindow>  Fracture::IFractureApp::mWindow = nullptr;

Fracture::IFractureApp::IFractureApp()
{
}

Fracture::IFractureApp::~IFractureApp()
{
}

bool Fracture::IFractureApp::ShouldWindowClose(Fracture::AppWindow* window)
{
    return window->ShouldWindowClose();
}

std::unique_ptr<Fracture::AppWindow>  Fracture::IFractureApp::CreateAppWindow(const WindowCreationInfo* info)
{
    /* Initialize the library */
    if (!info->mSharedContext)
    {
        if (!glfwInit())
            return nullptr;
    }

    auto mWindow = std::make_unique<AppWindow>(*info);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
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
        if(info->mSharedContext)
            mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, glfwGetPrimaryMonitor(), info->mSharedContext);
        else
        {
            mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, glfwGetPrimaryMonitor(), NULL);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            glfwSwapInterval(1);
        }
    }
    else
    {
        
        if (info->mSharedContext)
            mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, NULL, info->mSharedContext);
        else
        {

            mWindow->Context = glfwCreateWindow(info->Width, info->Height, info->Name, NULL, NULL);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            glfwSwapInterval(1);
        }
    }


    if (!mWindow->Context)
    {
        glfwTerminate();
        return nullptr;
    }    
    mWindow->Init();
   
    return std::move(mWindow);
}

