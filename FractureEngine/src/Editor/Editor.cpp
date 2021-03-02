#include "Editor.h"
#include <iostream>
#include "Frame.h"
#include "Panels/Panel.h"
#include "Panels/SceneviewPanel.h"
#include "Panels/Inspector.h"
#include "Panels/ViewPanel.h"
#include "Panels/TabbedPanel.h"
#include "Rendering/ShadowPass.h"
#include "Rendering/RenderTarget.h"
#include "Panels/AssetBrowserPanel.h"
#include "Entity/EntityFactory.h"
#include "EditorCamera.h"
#include "FreeCamera.h"
#include "EditorFrameGraph.h"


bool Fracture::Editor::opt_padding;
bool Fracture::Editor::p_open;
bool Fracture::Editor::m_loadNewProject;
bool Fracture::Editor::opt_fullscreen;
bool Fracture::Editor::showRenderConfig;
bool Fracture::Editor::showAudioConfig;
bool Fracture::Editor::showPhysicsConfig;
bool Fracture::Editor::showInputConfig;
bool Fracture::Editor::showProjectConfig;

bool Fracture::Editor::showInspector = true;
bool Fracture::Editor::showLogger;
bool Fracture::Editor::showScenegraph = true;
bool Fracture::Editor::showAssets;
bool Fracture::Editor::showViewport = true;

std::shared_ptr<Fracture::EditorFrameGraph> Fracture::Editor::m_graph;
std::shared_ptr<Fracture::Scene> Fracture::Editor::m_ActiveScene;
std::unique_ptr<Fracture::SceneManager> Fracture::Editor::m_SceneManager;
std::unique_ptr<Fracture::EntityFactory> Fracture::Editor::m_EntityFactory;
std::shared_ptr<Fracture::ProjectProperties> Fracture::Editor::m_properties;
std::shared_ptr<Fracture::GameSettings> Fracture::Editor::m_GameSettings;
std::shared_ptr<Fracture::SceneView> Fracture::Editor::m_sceneview;
std::unique_ptr<Fracture::Eventbus>  Fracture::Editor::m_Eventbus;

inline void Style();
const float FIXED_STEP = 1/60.0f;

Fracture::Editor::Editor()
{        
    m_logger = std::make_shared<Logger>();
    m_properties = std::make_shared<ProjectProperties>();
    m_loadNewProject = false;
    currentTime = glfwGetTime();
}

Fracture::Editor::~Editor()
{
    
}

void Fracture::Editor::onInit()
{      
    m_Eventbus = std::make_unique<Eventbus>();
    m_GameSettings = std::make_shared<GameSettings>();
    m_window = GameWindow::Create(1920, 1080, "Fracture Engine: " + m_properties->ProjectName);
    m_window->MaximiseWindow();
    m_Renderer = std::make_shared<Renderer>();
    m_AssetManger = std::make_unique<AssetManager>(m_properties);
    m_SceneManager = std::make_unique<SceneManager>();
    m_InputManager = std::make_unique<InputManager>();
    m_PhysicsManger = std::make_unique<PhysicsManager>();
    m_EntityFactory = std::make_unique<EntityFactory>();
    m_Profiler = std::make_unique<Profiler>();
    m_ComponentManager = std::make_unique<ComponentManager>();
    m_ScriptManger = std::make_shared<ScriptManager>();
    m_AnimationManger = std::make_unique<AnimationManager>();

    m_ComponentManager->onInit();

    m_Renderer->Subscribe(*m_Eventbus.get());
  
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

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    
    Style();
  
    // Setup Platform/Renderer bindings
    if (!ImGui_ImplGlfw_InitForOpenGL(m_window->Context(), true))
    {
        FRACTURE_ERROR("FAILED TO INIT IMGUI GLFW");
    }

    if (!ImGui_ImplOpenGL3_Init("#version 450"))
    {
        FRACTURE_ERROR("FAILED TO INIT IMGUI OPENGL");
    }

    m_frame = std::shared_ptr<Fracture::Frame>(new Frame());

    m_sceneview = std::shared_ptr<Fracture::SceneView>(new SceneView("Scene"));
    m_inspectorpanel = std::shared_ptr<Fracture::InspectorPanel>(new InspectorPanel("Property editor",*m_sceneview.get()));
    m_viewpanel = std::shared_ptr<ViewPanel>(new ViewPanel("Viewport", *m_sceneview.get(),*m_Renderer.get()));
    m_TabbedPanel = std::shared_ptr<TabbedPanel>(new TabbedPanel("Tab panel"));
    m_AssetBrowser = std::make_shared<AssetBrowserPanel>();

    ImFont* pFont = io.Fonts->AddFontFromFileTTF("content/fonts/Roboto-Regular.TTF", 14.0f);
   

    m_frame->AddPanel(m_sceneview);
    m_frame->AddPanel(m_inspectorpanel);
    m_frame->AddPanel(m_viewpanel);
    m_frame->AddPanel(m_TabbedPanel);
    m_frame->AddPanel(m_AssetBrowser);
   
    m_PhysicsManger->Init();
    camera = std::make_shared<FreeCamera>();//TODO - update init of camera;
    //m_Renderer = std::make_shared<Renderer>();
    m_Renderer->clearColor(0.3f, 0.5f, 9.0f);

   
    glfwSetKeyCallback(m_window->Context(), key_callback);
   
 
}

bool Fracture::Editor::onLoad()
{   

    ProjectSerializer seriliazer(m_properties);
    if (!seriliazer.DeSerialize(m_properties->ProjectFilePath))
    {
        FRACTURE_ERROR("FAiLED to load Project");
        return false;
    }     

    m_Renderer->onInit();
    m_SceneManager->SetScene(m_properties->ActiveScene);
    m_viewpanel->init();
    m_Renderer->SetCamera(camera);
    m_viewpanel->setRenderer(*m_Renderer.get());   
    SetScene();   
    m_graph = std::shared_ptr<EditorFrameGraph>(new EditorFrameGraph(*m_Renderer));
    m_graph->Buildgraph();
    return true;
}

void Fracture::Editor::onLoadNew()
{
    m_Renderer->onInit();
    m_AssetManger->AddTexture2D("TranslateIcon", "content/textures/TranslateIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("ScaleIcon", "content/textures/ScaleIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("RotateIcon", "content/textures/RotateIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("GameObjectIcon", "content/textures/GameObjectIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("CameraIcon", "content/textures/CameraIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("LightIcon", "content/textures/LightIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("EyeIcon", "content/textures/EyeIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("EyeIconC", "content/textures/EyeIconC.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("MeshIcon", "content/textures/MeshIcon.png", TextureType::Diffuse); 
    m_AssetManger->AddTexture2D("SceneIcon", "content/textures/SceneIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("MaterialIcon", "content/textures/MaterialIcon.png", TextureType::Diffuse);
    m_AssetManger->AddTexture2D("ShaderIcon", "content/textures/ShaderIcon.png", TextureType::Diffuse);

    m_AssetManger->AddHDR("Loft",  "content/environments/Newport_Loft_Env.hdr",TextureType::Diffuse);

    //Environment
    m_AssetManger->AddShader("CubeMap", "content/shaders/CubeMap/vertex.glsl", "content/shaders/CubeMap/fragment.glsl");

    //PointShadows
    m_AssetManger->AddShader("PointShadows", "content/shaders/PointShadows/vertex.glsl", "content/shaders/PointShadows/fragment.glsl", "content/shaders/PointShadows/geometry.glsl");


    //Irradiance
    m_AssetManger->AddShader("irradiance", "content/shaders/irradiance/vertex.glsl", "content/shaders/irradiance/fragment.glsl");

    //prefilter
    m_AssetManger->AddShader("prefilter", "content/shaders/prefilter/vertex.glsl", "content/shaders/prefilter/fragment.glsl");

    //bdrf
    AssetManager::AddShader("bdrf", "content/shaders/bdrf/vertex.glsl", "content/shaders/bdrf/fragment.glsl");

    //Skybox
    AssetManager::AddShader("Skybox", "content/shaders/Environment/vertex.glsl", "content/shaders/Environment/fragment.glsl");

    //DebugShaders
    AssetManager::AddShader("DebugShader", "content/shaders/debug/vertex.glsl", "content/shaders/debug/fragment.glsl");
    
    //Primite material
    AssetManager::AddShader("PrimitiveMaterial", "content/shaders/primitives/vertex.glsl", "content/shaders/primitives/fragment.glsl");

    //billboards
    AssetManager::AddShader("BillboardShader", "content/shaders/Billboards/vertex.glsl", "content/shaders/Billboards/fragment.glsl");

    //depthShader
    AssetManager::AddShader("DepthShader", "content/shaders/DepthMap/DepthVertex.glsl", "content/shaders/DepthMap/DepthFragment.glsl");

    //PBR Shader
    AssetManager::AddShader("PBRStatic", "content/shaders/PBRStatic/vertex.glsl", "content/shaders/PBRStatic/fragment.glsl");

    //PBR Primitives
    AssetManager::AddShader("PBRPlaneShader", "content/shaders/PBRPrimitive/vertex.glsl", "content/shaders/PBRPrimitive/fragment.glsl");

    //PBR Animated
    AssetManager::AddShader("PBRAnimated", "content/shaders/PBRAnimated/vertex.glsl", "content/shaders/PBRAnimated/fragment.glsl");
    
    //PickingShader
    AssetManager::AddShader("PickingShader", "content/shaders/Picking/vertex.glsl", "content/shaders/Picking/fragment.glsl");

    //textured models
    AssetManager::AddShader("default", "content/shaders/model/vertex.glsl", "content/shaders/model/fragment.glsl");

    //Outline Shader
    AssetManager::AddShader("OutlinePass", "content/shaders/outline/vertex.glsl", "content/shaders/outline/fragment.glsl");

    //Invert Color Shader
    AssetManager::AddShader("InvertColor", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/invert_frag.glsl");

    //Tone Mapping Shader
    AssetManager::AddShader("ToneMap", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/tonemapping_frag.glsl");

    //Threshold Mapping Shader
    AssetManager::AddShader("Threshold", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/threshold_frag.glsl");

    //Mix Shader
    AssetManager::AddShader("MixNode", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/Mix_frag.glsl");

    //Add Shader
    AssetManager::AddShader("AdditiveMix", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/AdditiveMix_frag.glsl");
   
    //Threshold Mapping Shader
    AssetManager::AddShader("ColorMap", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/ColorMap_frag.glsl");

    //Threshold Mapping Shader
    AssetManager::AddShader("BoxBlur", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/BoxBlur_frag.glsl");

    //Threshold Mapping Shader
    AssetManager::AddShader("DepthPass", "content/shaders/postprocess/depthPass_vert.glsl", "content/shaders/postprocess/depthPass_frag.glsl");

    //SSAO Shader
    AssetManager::AddShader("SSAOPASS", "content/shaders/postprocess/SSAO_vert.glsl", "content/shaders/postprocess/SSAO_frag.glsl");

    //SSAO BLUR Shader
    AssetManager::AddShader("SSAOBLUR", "content/shaders/postprocess/vertex.glsl", "content/shaders/postprocess/ssao_blur_frag.glsl");


    //Threshold Mapping Shader
    AssetManager::AddShader("MultiplyMix", "content/shaders/postprocess/SSAO_vert.glsl", "content/shaders/postprocess/MultiplyMix_frag.glsl");

    
    
    std::shared_ptr<Material> primitivesMaterial = std::make_shared<Material>("PrimitiveMaterial", m_AssetManger->getShader("PrimitiveMaterial"));
    primitivesMaterial->setColor3("material.diffuse", glm::vec3(0.9, 0.3, 0.5));
    primitivesMaterial->setColor3("material.ambient", glm::vec3(0.9, 0.3, 0.5));
    primitivesMaterial->setColor3("material.specular", glm::vec3(1.0, 1.0, 1.0));
    m_AssetManger->AddMaterial("PrimitiveMaterial", primitivesMaterial);    
    
    std::shared_ptr<Material> billboardMaterial = std::make_shared<Material>("billboardIcons", m_AssetManger->getShader("BillboardShader"));
    m_AssetManger->AddMaterial("billboardIcons", billboardMaterial);    

    /*
    std::shared_ptr<Material> pbrPrimitive = std::shared_ptr<Material>(new Material("PBRPlane", AssetManager::getShader("PBRPlaneShader")));
    pbrPrimitive->setColor3("albedo", glm::vec3(1.0f, 0.0f, 0.0f));
    pbrPrimitive->setFloat("metallic", 0.4f);
    pbrPrimitive->setFloat("roughness", 0.2f);
    pbrPrimitive->setFloat("ao", 1.0f);    
    */

    //std::shared_ptr<Material> pbrTextured = std::shared_ptr<Material>(new Material("PBRTextured", m_AssetManger->getShader("PBRStatic")));
    //pbrTextured->SetTexture("albedoMap",AssetManager::getTexture2D("Rust_albedo"),3);
    //pbrTextured->SetTexture("normalMap", AssetManager::getTexture2D("Rust_normal"), 4);
    //pbrTextured->SetTexture("metallicMap", AssetManager::getTexture2D("Rust_metallic"),5);
    //pbrTextured->SetTexture("roughnessMap", AssetManager::getTexture2D("Rust_roughness"),6);
    //pbrTextured->SetTexture("aoMap", AssetManager::getTexture2D("Rust_ao"),7);
    //pbrTextured->setFloat("albedoFlag", 1.0f);
    //pbrTextured->setFloat("normalFlag", 1.0f);
    //pbrTextured->setFloat("metallicFlag", 1.0f);
    //pbrTextured->setFloat("roughnessFlag", 1.0f);
    //pbrTextured->setFloat("aoFlag", 1.0f);
    
    std::shared_ptr<Material> base_material = std::shared_ptr<Material>(new Material("BaseMaterial", m_AssetManger->getShader("PBRStatic")));

    AssetManager::AddMaterial("BaseMaterial", base_material);

    AssetManager::AddMaterial("DebugMaterial", std::shared_ptr<Material>(new Material("DebugMaterial", AssetManager::getShader("DebugShader"))));

    AssetManager::AddMaterial("DepthMaterial", std::shared_ptr<Material>(new Material("DepthMaterial", AssetManager::getShader("DepthShader"))));

    AssetManager::AddMaterial("PickingMaterial", std::shared_ptr<Material>(new Material("PickingMaterial", AssetManager::getShader("PickingShader"))));

    AssetManager::AddMaterial("OutlinePass", std::shared_ptr<Material>(new Material("OutlinePass", AssetManager::getShader("OutlinePass"))));


    AssetManager::AddModel("Plane", "content/models/primitives/plane.fbx");
    AssetManager::AddModel("Cube", "content/models/primitives/cube.fbx");
    AssetManager::AddModel("Sphere", "content/models/primitives/sphere.fbx");
    AssetManager::AddModel("Cylinder", "content/models/primitives/cylinder.fbx");
    AssetManager::AddModel("Torus", "content/models/primitives/torus.fbx");
    AssetManager::AddModel("Suzane", "content/models/primitives/suzane.fbx");

    std::shared_ptr<Scene> scene = m_SceneManager->NewScene();
    m_SceneManager->AddScene("empty", scene);
    m_SceneManager->SetScene("empty");
    SetScene();
    m_viewpanel->init();
    m_Renderer->SetCamera(camera);
    m_viewpanel->setRenderer(*m_Renderer.get());
    m_graph = std::shared_ptr<EditorFrameGraph>(new EditorFrameGraph(*m_Renderer.get()));
    m_graph->Buildgraph();
}

void Fracture::Editor::run()
{
    Profiler::Get().BeginSession("EditorProfile");
    onInit();

    if (m_loadNewProject)
    {
        onLoadNew();
    }
    else
    {
        if (!onLoad())
        {
            FRACTURE_INFO("Failed to Loaded Program");
            done = true;
        }
    }
    //TODO - Set Editor Camera as camera;
    double lastTime = glfwGetTime();

    while (!m_window->ShouldClose() && !done)
    {       
        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

       


        while (frameTime > 0.0)
        {
            ProfilerTimer timer("Update Accumulator");
            float deltaTime = (float)std::min(frameTime, dt);
            onUpdate(deltaTime);
            frameTime -= deltaTime;
            time += deltaTime;
        }
       
        onRender();        
    }
    onShutdown();
}

void Fracture::Editor::onUpdate(float dt)
{
    ProfilerTimer timer("onUpdate");   
    m_window->pollEvents();
    InputManager::PollEvents();    

    if (InputManager::IsKeyDown(KeyCode::Delete))
    {
        if (m_sceneview->SelectedEntity())
        {
            FRACTURE_INFO("Deleting Entity :  {}", m_sceneview->SelectedEntity()->GetId());
            m_SceneManager->GetActiveScene()->Destroy(m_sceneview->SelectedEntity()->GetId());
        }
    }
    //m_PhysicsManger->startPhysics();
    m_AnimationManger->OnUpdate(FIXED_STEP);

    //m_PhysicsManger->stepUpdate();  
    m_ScriptManger->OnUpdate(dt);

    m_viewpanel->onUpdate(dt);
    
}

void Fracture::Editor::onRender()
{
    m_frame->begin(m_window->Context());

    if (showRenderConfig) showRenderManager(&showRenderConfig, m_Renderer);
    if (showAudioConfig) showAudioManager(&showAudioConfig);
    if (showPhysicsConfig) showPhysicsManager(&showPhysicsConfig);
    if (showInputConfig) showInputManager(&showInputConfig);
    if (showProjectConfig) showProjectSettings(&showProjectConfig,m_properties);

    Render();   


    m_frame->end(); 
    m_window->swapBuffers();
}

void Fracture::Editor::onShutdown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    m_AssetManger->Clear();
    m_PhysicsManger->onShutdown();    
    m_GameSettings.reset();
    m_SceneManager.reset();
    m_Renderer.reset();  
    m_PhysicsManger.reset();
    m_AssetManger.reset();
    m_window.reset();
    m_properties.reset();
    m_logger.reset();
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
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
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

    ImGui::Begin("DockSpace", &p_open, window_flags);
   
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("FractureDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        DrawMenuBar();

        m_Renderer->BeginFrame(m_SceneManager->GetActiveScene());      
        m_graph->execute(*m_Renderer);          
        m_Renderer->EndFrame();
       
        if (showScenegraph)
        { 
        m_sceneview->begin(&showScenegraph);
        m_sceneview->render();
        m_sceneview->end();
        }

        if (showInspector)
        {
            m_inspectorpanel->begin(&showInspector);
            m_inspectorpanel->render();
            m_inspectorpanel->end();
        }

        if (showViewport)
        {
            m_viewpanel->begin(&showViewport);
            m_viewpanel->render();
            m_viewpanel->end();
        }

        if (showLogger)
        {
            m_TabbedPanel->begin(&showLogger);
            m_TabbedPanel->render();
            m_TabbedPanel->end();
        }

        if (showAssets)
        {
            m_AssetBrowser->begin(&showAssets);
            m_AssetBrowser->render();
            m_AssetBrowser->end();
        }


        ImGui::End();
}

void Fracture::Editor::SetScene()
{    
    m_ActiveScene = m_SceneManager->GetActiveScene();
    m_sceneview->setScene(m_ActiveScene);    
}

void Fracture::Editor::oEvent(Event* m_event)
{
    m_Eventbus->Publish(m_event);
}

void Fracture::Editor::onChangeTitleName(std::string title)
{
    std::string newTitle = "Fracture Engine: " + title;
    glfwSetWindowTitle(GameWindow::Context(), newTitle.c_str());
    //SDL_SetWindowTitle(GameWindow::Context(),newTitle.c_str());
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
            if(ImGui::MenuItem("Save Project", NULL))
            {
                ProjectSerializer project(m_properties);
                project.Serialize(m_properties->ProjectDirectory+"/"+m_properties->ProjectName+".Fracture");

                GameSerializer serialiser(m_GameSettings);
                serialiser.Serialize(m_properties->ProjectDirectory + "/" + "Game.config");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("New Scene", NULL))
            {

                m_ActiveScene->clearScene();
                m_SceneManager->AddScene("Untitled", m_SceneManager->NewScene());
                m_SceneManager->SetScene("Untitled");
                SetScene();
               
            }
            if (ImGui::MenuItem("Open Scene", NULL))
            {
                std::shared_ptr<Scene> newscene = std::make_shared<Scene>();
                SceneSerializer serializer(newscene);
                if (!serializer.DeSerialize("bin/content/Sandbox.json"))//todo
                {
                    FRACTURE_ERROR("FAILED TO LOAD SCENE");
                }
                m_SceneManager->AddScene(newscene->Name, newscene);
                m_SceneManager->SetScene(newscene->Name);
                SetScene();                
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Scene", NULL))
            {
                SceneSerializer serializer(m_ActiveScene);
                serializer.Serialize(m_properties->ScenesPath+"/"+m_ActiveScene->Name+".scene");
            }
            if (ImGui::MenuItem("Save FrameGraph", NULL))
            {
                FramaGraphSerialiser serialiser(m_graph);
                serialiser.SerialiseGraph(m_properties->ScenesPath + "/" + m_ActiveScene->Name + ".graph");
            }
           
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
            // which we can't undo at the moment without finer window depth/z control
            ImGui::MenuItem("Vierport", NULL, &showViewport);
            ImGui::MenuItem("Hierachy View",NULL, &showScenegraph);
            ImGui::MenuItem("Inspector", NULL,&showInspector);
            ImGui::MenuItem("Asset Viewer", NULL,&showAssets);
            ImGui::MenuItem("Logging", NULL,&showLogger);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Create"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            if (ImGui::MenuItem("Empty", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateEmpty(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Camera", NULL)) {};
            if (ImGui::MenuItem("Sunlight", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateSunlight(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Pointlight", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreatePointlight(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Spotlight", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateSpotlight(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Skylight", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateSkylight(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Cube", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateCube(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Sphere", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateSphere(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Plane", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreatePlane(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Torus", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateTorus(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Cylinder", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateCylinder(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::MenuItem("Suzane", NULL))
            {
                std::shared_ptr<Entity> entity = EntityFactory::CreateSuzane(m_ActiveScene);
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            };
            if (ImGui::BeginMenu("Model"))
            {
                for (auto model : AssetManager::GetModels())
                {
                    if (ImGui::MenuItem(model.second->Name.c_str()))
                    {
                        std::shared_ptr<Entity> entity = EntityFactory::CreateModel(m_ActiveScene, model.second->Name.c_str());
                        m_ActiveScene->addEntity(entity);
                        m_sceneview->setSelectEntity(entity);
                    }
                }        
                ImGui::EndMenu();
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

void Fracture::Editor::isNewProject(bool isnew)
{
    m_loadNewProject = isnew;
}

std::shared_ptr<Fracture::Scene> Fracture::Editor::ActiveScene()
{
    return m_SceneManager->GetActiveScene();
}

std::shared_ptr<Fracture::Logger> Fracture::Editor::GetLogger()
{
    return m_logger;
}

std::shared_ptr<Fracture::ProjectProperties> Fracture::Editor::Properties()
{
    return m_properties;
}


void Fracture::Editor::showRenderManager(bool* p_open,std::shared_ptr<Fracture::Renderer>& _renderer)
{
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Render System", p_open,ImGuiWindowFlags_NoDocking|ImGuiWindowFlags_NoCollapse))
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        //float width,  height;
        ImGui::PushID("renderConfig");
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::Text("Resolution");
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 20.0f, lineHeight };      

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Width", buttonSize))
        {
            m_GameSettings->Resolution_Width = 0;
        }         
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragInt("##X", &m_GameSettings->Resolution_Width, 1,0,0, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Height", buttonSize))
        {
            m_GameSettings->Resolution_Height = 0;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragInt("##Y", &m_GameSettings->Resolution_Height, 1, 0, 0 ,"%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleVar();

        bool isWindowResizable = m_GameSettings->IsResizable;
        ImGui::NextColumn();
        ImGui::Text("Is Window Resizable");
        ImGui::NextColumn();
        ImGui::PushFont(boldFont);
        ImGui::Checkbox("##drawdebug", &isWindowResizable);
        m_GameSettings->IsResizable = isWindowResizable;
        ImGui::PopFont();
        ImGui::Separator();

        ImGui::Separator();
        bool bloom ;
        ImGui::NextColumn();
        ImGui::Text("Exposure /Gamma");
        ImGui::NextColumn();

        ImGui::PushFont(boldFont);
        ImGui::Checkbox("##Bloom", &bloom);
        ImGui::DragFloat("##exp", &m_graph->ToneMap->Exposure, 0.1f, 0.0f, 5.0f, "%.2f");
        ImGui::DragFloat("##gam", &m_graph->ToneMap->Gamma, 0.1f, 0.0f, 5.0f, "%.2f");
        ImGui::DragFloat("##bright", &m_graph->BrightPass->brightPassThreshold, 0.1f, 0.0f, 5.0f, "%.2f");
        ImGui::DragInt("##size", &m_graph->ssaoblur->amount, 1, 0, 5, "%.2f");
  
        ImGui::NextColumn();
        ImGui::Text("SSAO");
        ImGui::SameLine();
        ImGui::NextColumn();

        ImGui::Text("Strength");
        ImGui::SameLine();
        ImGui::DragFloat("##strength", &m_graph->ssao->total_strength, 0.001f, 0.0001f, 0.0f, "%.2f");

        ImGui::Text("Area");
        ImGui::SameLine();
        ImGui::DragFloat("##area", &m_graph->ssao->area, 0.001f, 0.0001f, 0.0f, "%.2f");

        ImGui::Text("Radius");
        ImGui::SameLine();
        ImGui::DragFloat("##radius", &m_graph->ssao->radius, 0.001f, 0.0001f, 0.0f, "%.2f");

        ImGui::Text("Falloff");
        ImGui::SameLine();
        ImGui::DragFloat("##falloff", &m_graph->ssao->falloff, 0.001f, 0.0001f, 0.0f, "%.2f");

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

        bool DebugGrid = _renderer->IsDrawGrid();
        ImGui::NextColumn();
        ImGui::Text("Draw Debug");
        ImGui::NextColumn();
        ImGui::PushFont(boldFont);
        ImGui::Checkbox("##drawdebug", &DebugGrid);
        _renderer->SetDrawGrid(DebugGrid);
        ImGui::PopFont();
        ImGui::Separator();
        ImGui::NextColumn();
        ImGui::BeginChild("ShadowMap");
        ImGui::Image((void*)_renderer->m_ShadowPass->GetRenderTarget()->GetDepthStencilTexture()->GetTextureID(), ImVec2(200,200));
        ImGui::EndChild();
        ImGui::NextColumn();
        static float vnear;
        static float vfar;
        static float vleft;
        static float vright;
        static float vtop;
        static float vbottom;

        vnear = _renderer->m_ShadowPass->GetNearFarPlanes().x;
        vfar = _renderer->m_ShadowPass->GetNearFarPlanes().y;
        vleft = _renderer->m_ShadowPass->GetOrthor().x;
        vright = _renderer->m_ShadowPass->GetOrthor().y;
        vtop = _renderer->m_ShadowPass->GetOrthor().z;
        vbottom = _renderer->m_ShadowPass->GetOrthor().w;

        ImGui::DragFloat("##vn", &vnear, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("##vf", &vfar, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("##vl", &vleft, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("##vr", &vright, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("##vt", &vtop, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("##vb", &vbottom, 0.1f, 0.0f, 0.0f, "%.2f");
        _renderer->m_ShadowPass->SetNearFarPlanes(vnear, vfar);
        _renderer->m_ShadowPass->SetOrthor(vleft, vright, vtop, vbottom);
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

void Fracture::Editor::showProjectSettings(bool* p_open, std::shared_ptr<Fracture::ProjectProperties>& _properties)
{
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Project Settings", p_open,ImGuiWindowFlags_NoDocking|ImGuiWindowFlags_NoCollapse))
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        std::string title = _properties->ProjectName;
        InspectorPanel::DrawTextInputControl("Project Name",title);
        onChangeTitleName(title);
        _properties->ProjectName = title;
        InspectorPanel::DrawTextInputControl("Project Directory", _properties->ProjectDirectory);
        InspectorPanel::DrawTextInputControl("Content Directory", _properties->ContentDirectory);
        InspectorPanel::DrawTextInputControl("Scenes Path", _properties->ScenesPath);
        InspectorPanel::DrawTextInputControl("Shaders Path", _properties->ShadersPath);
        InspectorPanel::DrawTextInputControl("Textures Path", _properties->TexturesPath);
        InspectorPanel::DrawTextInputControl("Models Path", _properties->ModelsPath);
        InspectorPanel::DrawTextInputControl("Game Config Path", _properties->GameConfigPath);
     
        std::string Gametitle = m_GameSettings->Title;
        InspectorPanel::DrawTextInputControl("Game Window Title", Gametitle);
        m_GameSettings->Title = Gametitle;

      
        ImGui::End();
        return;
    }
    ImGui::End();


}

void Fracture::Editor::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if (mods == GLFW_MOD_CONTROL)
    {
        //New
        if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            FRACTURE_INFO("New");
        }
        //Open
        if (key == GLFW_KEY_O && action == GLFW_PRESS)
        {
            FRACTURE_INFO("Open");
        }
        //Save
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            FRACTURE_INFO("Project Saved");
            ProjectSerializer project(m_properties);
            project.Serialize(m_properties->ProjectDirectory + "/" + m_properties->ProjectName + ".Fracture");

            GameSerializer serialiser(m_GameSettings);
            serialiser.Serialize(m_properties->ProjectDirectory + "/" + "Game.config");
        }
        //Select ALL
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            FRACTURE_INFO("Select All");
        }
        //Duplicate
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            if(m_sceneview->m_selection)
            {
                std::shared_ptr<Entity> entity = m_ActiveScene->Duplicate(m_sceneview->SelectedEntity());
                m_ActiveScene->addEntity(entity);
                m_sceneview->setSelectEntity(entity);
            }            
        }
    }
}

void Style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 0;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.31f, 0.31f, 0.31f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.73f, 0.82f, 0.93f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.32f, 0.41f, 0.52f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.52f, 0.66f, 0.81f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.51f, 0.51f, 0.51f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.99f, 0.99f, 0.99f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.78f, 0.78f, 0.78f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.45f, 0.45f, 0.45f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);




    style.PopupRounding = 3;
    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = (float)is3D;

    style.WindowRounding = 2;
    style.ChildRounding = 3;
    style.FrameRounding = 4;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 2;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = (float)is3D;
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


