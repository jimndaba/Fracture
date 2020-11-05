#include "Editor.h"
#include <iostream>
#include "Frame.h"
#include "Panels/Panel.h"
#include "Panels/SceneviewPanel.h"
#include "Panels/Inspector.h"
#include "Panels/ViewPanel.h"
#include "Panels/TabbedPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "SandboxScene.h"
#include "Entity/EntityFactory.h"

bool Fracture::Editor::opt_padding;
bool Fracture::Editor::p_open;
bool Fracture::Editor::opt_fullscreen;
bool Fracture::Editor::showRenderConfig;
bool Fracture::Editor::showAudioConfig;
bool Fracture::Editor::showPhysicsConfig;
bool Fracture::Editor::showInputConfig;
bool Fracture::Editor::showProjectConfig;

std::shared_ptr<Fracture::SandboxScene> sandboxScene;
std::shared_ptr<Fracture::Scene> Fracture::Editor::m_ActiveScene;
std::unique_ptr<Fracture::SceneManager> Fracture::Editor::m_SceneManager;
std::unique_ptr<Fracture::EntityFactory> Fracture::Editor::m_EntityFactory;

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
    m_AssetManger = std::make_unique<AssetManager>();
    m_PhysicsManger = std::make_unique<PhysicsManager>();
    m_SceneManager = std::make_unique<SceneManager>();
    m_Profiler = std::make_unique<Profiler>();
    m_EntityFactory = std::make_unique<EntityFactory>();

    showRenderConfig  = false;
    showAudioConfig   = false;
    showPhysicsConfig = false;
    showInputConfig   = false;
    showProjectConfig = false;
    done = false;
    p_open = true;
    opt_fullscreen = true;
    opt_padding = false;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImFont* pFont = io.Fonts->AddFontFromFileTTF("bin/content/fonts/Roboto-Regular.TTF",14.0f);
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
    m_AssetBrowser = std::make_shared<AssetBrowserPanel>();

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    m_SceneManager->AddScene("empty", scene);
    m_SceneManager->SetScene("empty");
    SetScene();

    m_frame->AddPanel(m_sceneview);
    m_frame->AddPanel(m_inspectorpanel);
    m_frame->AddPanel(m_viewpanel);
    m_frame->AddPanel(m_TabbedPanel);
    m_frame->AddPanel(m_AssetBrowser);
   

    m_AssetManger->AddShader("DebugShader", "bin/content/shaders/debug/vertex.glsl", "bin/content/shaders/debug/fragment.glsl");
    m_AssetManger->AddShader("PrimitiveMaterial", "bin/content/shaders/primitives/vertex.glsl", "bin/content/shaders/primitives/fragment.glsl");

    //textures
    Fracture::AssetManager::AddShader("default", "bin/content/shaders/model/vertex.glsl", "bin/content/shaders/model/fragment.glsl");

    m_AssetManger->AddMaterial("DebugMaterial",std::shared_ptr<Material>(new Material("DebugMaterial",AssetManager::getShader("DebugShader"))));

    std::shared_ptr<Material> primitivesMaterial = std::make_shared<Material>("PrimitiveMaterial",m_AssetManger->getShader("PrimitiveMaterial"));

    primitivesMaterial->setVec3("material.diffuse",glm::vec3(0.9,0.3,0.5));
    primitivesMaterial->setVec3("material.ambient", glm::vec3(0.9, 0.3, 0.5));
    primitivesMaterial->setVec3("material.specular", glm::vec3(1.0, 1.0, 1.0));

    m_AssetManger->AddMaterial("PrimitiveMaterial",primitivesMaterial);

    m_AssetManger->AddTexture("TranslateIcon", "bin/content/textures/TranslateIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("ScaleIcon", "bin/content/textures/ScaleIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("RotateIcon", "bin/content/textures/RotateIcon.png", TextureType::Diffuse);
    Fracture::AssetManager::AddTexture("container", "bin/content/textures/container.png", Fracture::TextureType::Diffuse);
    Fracture::AssetManager::AddTexture("specular", "bin/content/textures/container_specular.png", Fracture::TextureType::Specular);


    m_AssetManger->AddModel("Plane", "bin/content/models/primitives/plane.fbx");
    m_AssetManger->AddModel("Cube", "bin/content/models/primitives/cube.fbx");
    m_AssetManger->AddModel("Sphere", "bin/content/models/primitives/sphere.fbx");
    m_AssetManger->AddModel("Cylinder", "bin/content/models/primitives/cylinder.fbx");
    m_AssetManger->AddModel("Torus", "bin/content/models/primitives/torus.fbx");
    m_AssetManger->AddModel("Suzane", "bin/content/models/primitives/suzane.fbx");

    m_AssetManger->AddTexture("GameObjectIcon", "bin/content/textures/GameObjectIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("CameraIcon", "bin/content/textures/CameraIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("LightIcon", "bin/content/textures/LightIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("EyeIcon", "bin/content/textures/EyeIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture("EyeIconC", "bin/content/textures/EyeIconC.png", TextureType::Diffuse);

    m_PhysicsManger->Init();
  
    m_Renderer = std::unique_ptr<Renderer>(new Renderer(1280, 720));
    m_Renderer->clearColor(0.3f, 0.5f, 9.0f);
    m_Renderer->onInit();
    m_viewpanel->setRenderer(m_Renderer.get());
    m_viewpanel->init();
}

void Fracture::Editor::run()
{
    Profiler::Get().BeginSession("EditorProfile");
    onInit();
    double lastTime = SDL_GetTicks() / 1000.0;

    while (!done)
    {
        double current = SDL_GetTicks() / 1000.0;
        double elapsed = current - lastTime;
        onUpdate((float)elapsed);
        m_PhysicsManger->stepUpdate();
        onRender();
        lastTime = current;
    }
    onShutdown();
}

void Fracture::Editor::onUpdate(float dt)
{
    ProfilerTimer timer("onUpdate");

    
   
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
    
    m_PhysicsManger->startPhysics();

    m_viewpanel->onUpdate(dt);
}

void Fracture::Editor::onRender()
{
    m_frame->begin(m_window->Context());

    if (showRenderConfig) showRenderManager(&showRenderConfig, m_Renderer);
    if (showAudioConfig) showAudioManager(&showRenderConfig);
    if (showPhysicsConfig) showPhysicsManager(&showRenderConfig);
    if (showInputConfig) showInputManager(&showRenderConfig);
    if (showProjectConfig) showProjectSettings(&showRenderConfig);
   
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
    m_Renderer.reset();  
    m_PhysicsManger.reset();
    m_window.reset();
    Profiler::Get().EndSession();
}

void Fracture::Editor::Render()
{ 
    ProfilerTimer timer("onRender");
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

        DrawMenuBar();
       
       
        m_Renderer->BeginFrame(m_SceneManager->GetActiveScene());
        m_Renderer->RenderPasses();
        m_Renderer->EndFrame();

        m_frame->render();
        ImGui::End();
}

void Fracture::Editor::SetScene()
{    
    m_ActiveScene = m_SceneManager->GetActiveScene();
    m_sceneview->setScene(m_ActiveScene);    
}

void Fracture::Editor::DrawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("New Project", NULL);
            ImGui::MenuItem("Open Project", NULL);
            ImGui::Separator();
            if (ImGui::MenuItem("New Scene", NULL))
            {
                m_ActiveScene->clearScene();
                IDManager::ResetIDs();
                m_SceneManager->AddScene("newScene", std::make_shared<Scene>());
                m_SceneManager->SetScene("newScene");
               
            }
            if (ImGui::MenuItem("Open Scene", NULL))
            {
                IDManager::ResetIDs();
                std::shared_ptr<Scene> newscene = std::make_shared<Scene>();
                SceneSerializer serializer(newscene);
                if (!serializer.DeSerialize("bin/content/Sandbox.json"))
                {
                    FRACTURE_ERROR("FAILED TO LOAD SCENE");
                }
                m_SceneManager->AddScene(newscene->Name, newscene);
                m_SceneManager->SetScene(newscene->Name);
                SetScene();                
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", NULL))
            {
                SceneSerializer serializer(sandboxScene);
                serializer.Serialize("bin/content/Sandbox.json");
            }
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
            ImGui::MenuItem("Project Settings", NULL, &showProjectConfig);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Toolbar", NULL);
            ImGui::MenuItem("Hierachy View", NULL);
            ImGui::MenuItem("Inspector", NULL);
            ImGui::MenuItem("Asset Viewer", NULL);
            ImGui::MenuItem("Logging", NULL);
            ImGui::MenuItem("Project Settings", NULL);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Create"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            if (ImGui::MenuItem("Empty", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateEmpty(m_ActiveScene));
            };
            if (ImGui::MenuItem("Camera", NULL)) {};
            if (ImGui::MenuItem("Sunlight", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateSunlight(m_ActiveScene));
            };
            if (ImGui::MenuItem("Pointlight", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreatePointlight(m_ActiveScene));
            };
            if (ImGui::MenuItem("Spotlight", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateSpotlight(m_ActiveScene));
            };
            if (ImGui::MenuItem("Cube", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateCube(m_ActiveScene));
            };
            if (ImGui::MenuItem("Sphere", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateSphere(m_ActiveScene));
            };
            if (ImGui::MenuItem("Plane", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreatePlane(m_ActiveScene));
            };
            if (ImGui::MenuItem("Torus", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateTorus(m_ActiveScene));
            };
            if (ImGui::MenuItem("Cylinder", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateCylinder(m_ActiveScene));
            };
            if (ImGui::MenuItem("Suzane", NULL))
            {
                m_ActiveScene->addEntity(EntityFactory::CreateSuzane(m_ActiveScene));
            };
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Systems"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Render System", NULL, &showRenderConfig);
            ImGui::MenuItem("Audio System", NULL, &showAudioConfig);
            ImGui::MenuItem("Physics System", NULL, &showPhysicsConfig);
            ImGui::MenuItem("Input System", NULL, &showInputConfig);
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

}

std::shared_ptr<Fracture::Scene> Fracture::Editor::ActiveScene()
{
    return m_SceneManager->GetActiveScene();
}

void Fracture::Editor::showRenderManager(bool* p_open,std::unique_ptr<Fracture::Renderer>& _renderer)
{
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Render System", p_open,ImGuiWindowFlags_NoDocking|ImGuiWindowFlags_NoCollapse))
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        float width,  height;
        ImGui::PushID("renderConfig");
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::Text("Resolution");
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 20.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Width", buttonSize))
        {

        }         
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##X", &width, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Height", buttonSize))
        {
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &height, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleVar();

        ImGui::Separator();
        bool bloom ;
        ImGui::NextColumn();
        ImGui::Text("Bloom");
        ImGui::NextColumn();

        ImGui::PushFont(boldFont);
        ImGui::Checkbox("##Bloom", &bloom);
        ImGui::PopFont();

        ImGui::Separator();
        bool DebugDraw = _renderer->IsDebugDraw();
        ImGui::NextColumn();
        ImGui::Text("Draw Debug");
        ImGui::NextColumn();

        ImGui::PushFont(boldFont);
        ImGui::Checkbox("##drawdebug", &DebugDraw);
        _renderer->SetDebugRender(DebugDraw);
        ImGui::PopFont();

        ImGui::Separator();


        ImGui::Columns(1);
        ImGui::PopID();
        ImGui::End();
        return;
    }
    ImGui::End();
}

void Fracture::Editor::showAudioManager(bool* p_open)
{
}

void Fracture::Editor::showPhysicsManager(bool* p_open)
{
}

void Fracture::Editor::showInputManager(bool* p_open)
{
}

void Fracture::Editor::showProjectSettings(bool* p_open)
{
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

    style.WindowRounding = 2;
    style.ChildRounding = 2;
    style.FrameRounding = 2;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 2;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.22f, 0.23f, 0.25f, 1.00f);
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


