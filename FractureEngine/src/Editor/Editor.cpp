#include "Editor.h"
#include <iostream>
#include "Frame.h"
#include "Panels/Panel.h"
#include "Panels/SceneviewPanel.h"
#include "Panels/Inspector.h"
#include "Panels/ViewPanel.h"
#include "Panels/TabbedPanel.h"
#include "SandboxScene.h"

bool Fracture::Editor::opt_padding;
bool Fracture::Editor::p_open;
bool Fracture::Editor::opt_fullscreen;

std::shared_ptr<Fracture::SandboxScene> sandboxScene;
std::shared_ptr<Fracture::Scene> Fracture::Editor::m_ActiveScene;


inline void Style();


Fracture::Editor::Editor()
{
   
}

Fracture::Editor::~Editor()
{
    
}

void Fracture::Editor::onInit()
{
    m_logger = std::make_unique<Logger>();
    m_Eventbus = std::make_unique<Eventbus>();
    m_window = std::make_unique<GameWindow>(1280, 720, "Fracture Engine");
    m_InputManager = std::make_unique<InputManager>();
    m_AssetMaanger = std::make_unique<AssetManager>();

    /*
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags sdl_window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Fracture Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, sdl_window_flags);
    gl_context = SDL_GL_CreateContext(window);
    
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync 
    */


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();


    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImFont* pFont = io.Fonts->AddFontFromFileTTF("Play-Regular.TTF",15.0f);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    
    Style();
  
    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(m_window->Context(), m_window->glContext());
    ImGui_ImplOpenGL3_Init("#version 400");

    m_frame = std::shared_ptr<Fracture::Frame>(new Frame());

    m_sceneview = std::shared_ptr<Fracture::SceneView>(new SceneView("Scene"));
    m_inspectorpanel = std::shared_ptr<Fracture::InspectorPanel>(new InspectorPanel("Property editor"));
    m_viewpanel = std::shared_ptr<ViewPanel>(new ViewPanel("Viewport"));
    m_TabbedPanel = std::shared_ptr<TabbedPanel>(new TabbedPanel("Tab panel"));

    sandboxScene = std::make_shared<SandboxScene>();
    SetScene(sandboxScene);

    m_frame->AddPanel(m_sceneview);
    m_frame->AddPanel(m_inspectorpanel);
    m_frame->AddPanel(m_viewpanel);
    m_frame->AddPanel(m_TabbedPanel);


    m_Renderer = std::unique_ptr<Renderer>(new Renderer(1280, 720));
    m_Renderer->clearColor(0.3f, 0.5f, 9.0f);

    m_Renderer->onInit();
    m_viewpanel->setRenderer(m_Renderer.get());
    m_viewpanel->init();

    m_AssetMaanger->AddTexture("TranslateIcon", "bin/content/textures/TranslateIcon.png", TextureType::Diffuse);
    m_AssetMaanger->AddTexture("ScaleIcon", "bin/content/textures/ScaleIcon.png", TextureType::Diffuse);
    m_AssetMaanger->AddTexture("RotateIcon", "bin/content/textures/RotateIcon.png", TextureType::Diffuse);
}

void Fracture::Editor::run()
{
    onInit();
    while (!done)
    {
        onUpdate();
    }
    onShutdown();
}

void Fracture::Editor::onUpdate()
{
    done = false;
    p_open = true;
    opt_fullscreen = true;
    opt_padding = false;
   
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window->Context()))
            done = true;
    }
    InputManager::PollEvents();
       
    m_viewpanel->onUpdate(1 / 60.0f);


    m_frame->begin(m_window->Context());
    Render();
    m_frame->end();
    
    m_window->swapBuffers();
}

void Fracture::Editor::onShutdown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    m_window.reset();
}

void Fracture::Editor::Render()
{ 

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        panel_flags = ImGuiWindowFlags_NoCollapse;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;


    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
   
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("New Project", NULL);  
                ImGui::MenuItem("Open Project", NULL);   
                ImGui::Separator();        
                ImGui::MenuItem("New Scene", NULL);
                ImGui::MenuItem("Open Scene", NULL);
                ImGui::Separator();
                ImGui::MenuItem("Save", NULL);
                ImGui::MenuItem("Save As", NULL);
                if (ImGui::MenuItem("Exit", NULL))
                {
                    done = true;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Undo", "CTRL+Z");
                ImGui::MenuItem("Redo", "CTRL+Y");
                ImGui::MenuItem("Cut", "CTRL+X");
                ImGui::MenuItem("Copy", "CTRL+C");
                ImGui::MenuItem("Paste", "CTRL+V");
                ImGui::MenuItem("Delete", "Del");
                ImGui::Separator();
                ImGui::MenuItem("Select All", "CTRL+A");
                ImGui::Separator();
                ImGui::MenuItem("Project Settings", NULL);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Toolbar", NULL);
                ImGui::MenuItem("Content Manager", NULL);
                ImGui::MenuItem("Logging", NULL);
                ImGui::MenuItem("Project Settings", NULL);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Systems"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Render System", NULL);
                ImGui::MenuItem("Audio System", NULL);
                ImGui::MenuItem("Physics System", NULL);
                ImGui::MenuItem("Input System", NULL);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("View Help", NULL);
                ImGui::MenuItem("About Fracture", NULL);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
   
      

        m_Renderer->BeginFrame(m_ActiveScene);
        m_Renderer->RenderPasses();
        m_Renderer->EndFrame();

        m_frame->render();

        ImGui::End();
}

void Fracture::Editor::SetScene(std::shared_ptr<Scene> scene)
{
    scene->onLoad();
    m_ActiveScene = scene;
    m_sceneview->setScene(m_ActiveScene);
}

std::shared_ptr<Fracture::Scene> Fracture::Editor::ActiveScene()
{
    return m_ActiveScene;
}

inline void Style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 0;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
    colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = is3D;

    style.WindowRounding = 3;
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
}
