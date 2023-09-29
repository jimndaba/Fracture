#include "FracturePCH.h"
#include "ParticleSystem.h"
#include "World/SceneManager.h"
#include "Rendering/GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/RenderCommands.h"
#include "Common/Random.h"
#include "EmitterModifiers.h"
#include "Serialisation/ParticleFXSerialiser.h"
#include "Rendering/Material.h"

std::map<Fracture::UUID, std::shared_ptr<Fracture::ParticleFX>> Fracture::ParticleSystem::mParticleFXs;
std::map<Fracture::UUID, Fracture::ParticleFxRegistry> Fracture::ParticleSystem::mParticleFxRegister;
std::map<Fracture::UUID, std::shared_ptr<Fracture::ParticleFX>> Fracture::ParticleSystem::mParticleFXsLibrary;
std::map<Fracture::UUID, Fracture::ParticleFXObservers> Fracture::ParticleSystem::mFxObserversList;
std::map<Fracture::UUID, Fracture::ParticleRenderBatch> Fracture::ParticleSystem::mRenderBatch;

Fracture::ParticleSystem::ParticleSystem()
{
}

void Fracture::ParticleSystem::Init()
{
    mBillboard = std::make_unique<ParticleBillboard>();    
    BufferDescription desc;
    desc.data = mBillboard->mVertices.data();
    desc.bufferType = BufferType::ArrayBuffer;
    desc.size = sizeof(mBillboard->mVertices[0]) * mBillboard->mVertices.size();
    desc.usage = BufferUsage::Static;
    desc.Name = "Verticies";
    mBillboard->Vertex_Buffer = std::make_shared<Buffer>();
    GraphicsDevice::Instance()->CreateBuffer(mBillboard->Vertex_Buffer.get(), desc);
}

void Fracture::ParticleSystem::OnLoad()
{
    for (const auto& component : SceneManager::GetAllComponents<ParticleSystemComponent>())
    {
        if (!component->IsFXAttached)
            continue;

        if (mParticleFxRegister.find(component->particleFXID) == mParticleFxRegister.end())
        {
            FRACTURE_ERROR("Particle FX {} is not registered", component->particleFXID);
        }
        else
        {
            mParticleFXs[component->GetID()] = std::make_shared<ParticleFX>(*GetMainParticleFX(component->GetID()));
            for (const auto& emitter : mParticleFXs[component->GetID()]->mEmitters)
            {
                if (emitter.second.HasParticleTexture)
                {
                    const auto& texture = AssetManager::GetTextureByID(emitter.second.ParticleTexture);
                    if (texture)
                        FRACTURE_INFO("Loaded particle texture");
                }
            }
        }
    }
}

void Fracture::ParticleSystem::OnSave()
{
    for(const auto& fx : mParticleFXsLibrary)
    {
        Fracture::ParticleFxSerialiser loader(Fracture::ISerialiser::IOMode::Save, Fracture::ISerialiser::SerialiseFormat::Json);
        const auto& reg = mParticleFxRegister[fx.first];
        loader.WriteParticleFX(fx.second.get());
        loader.Save(reg.Path);
        fx.second->IsDirty = false;

        if (mFxObserversList.find(fx.first) != mFxObserversList.end())
        {
            for (const auto& observer : mFxObserversList[fx.first].observers)
            {
                mParticleFXs.erase(observer);
                mParticleFXs[observer] = std::make_shared<ParticleFX>(*fx.second.get());
            }
        }
    }
}

void Fracture::ParticleSystem::Update(float dt)
{
    for (const auto& component : SceneManager::GetAllComponents<ParticleSystemComponent>())
    {
        if (!component->IsFXAttached || !component)
            continue;

        const auto& fx = GetParticleFX(component->GetID());
        const auto& transform = SceneManager::GetComponent<TransformComponent>(component->GetID());
        const auto& hierachy = SceneManager::GetComponent<HierachyComponent>(component->GetID());

        glm::vec3 ParentPos = glm::vec3(0);
        glm::quat ParentRot = glm::quat();
        if (hierachy)
        {
            if (hierachy->HasParent)
            {
                const auto& parent_transform = SceneManager::GetComponent<TransformComponent>(hierachy->Parent);

                glm::vec4 p = parent_transform->WorldTransform * glm::vec4(transform->Position, 1.0f);// 
                ParentPos = glm::vec3(p.x, p.y, p.z);

                ParentRot = parent_transform->Rotation * transform->Rotation;

            }
        }
        if (fx->FxState == ParticleFX::StateOptions::Playing)
        {
            fx->CurrentTime += 0.016f;

            ///Emmit Particle
            for (int emitter_i = 0; emitter_i < fx->mEmitters.size(); emitter_i++)
            {
                fx->mEmitters[emitter_i].CurrentTime += 0.016f;
                float emmit_Start = fx->mEmitters[emitter_i].StartOffset;
                float emmit_End = fx->mEmitters[emitter_i].StartOffset + fx->mEmitters[emitter_i].Duration;

                if (fx->CurrentTime >= emmit_Start && fx->CurrentTime <= emmit_End)
                {
                    if (EmittParticleEmitter(fx->mEmitters[emitter_i], ParentPos, ParentRot))
                    {                        
                    }      
                }
            }

            if (fx->PlayMode == ParticleFX::PlayModeOptions::Looping)
            {
                fx->CurrentTime = fmod(fx->CurrentTime, fx->Duration);
            }
            else if (fx->CurrentTime >= fx->Duration)
            {
                fx->FxState = ParticleFX::StateOptions::Stopped;
                fx->CurrentTime = 0.0f;
            }
        }

        //Update particles
        for (int emitter_i = 0; emitter_i < fx->mEmitters.size(); emitter_i++)
        {
            for (int i = 0; i < fx->mEmitters[emitter_i].Particles.size(); i++)
            {
                auto& p = fx->mEmitters[emitter_i].Particles[i];
                if (!p.IsAlive)
                {
                    p.DistanceToCamera = -1.0f;
                    continue;
                }

                p.LifeSpan -= 0.016f;
                if (p.LifeSpan <= 0.0f)
                {
                    for (auto& sub : fx->mEmitters[emitter_i].mSubEmitters)
                    {
                        if (sub.EmmitStage == ParticleEmitter::SubEmittStageOption::OnDie)
                        {
                            EmittParticleEmitter(sub, p.Position, transform->Rotation);
                        }
                    }

                    p.IsAlive = false;
                    p.DistanceToCamera = -1.0f;
                    fx->mEmitters[emitter_i].AliveParticles -= 1;                    
                    continue;
                }


                for (const auto& modifier : fx->mEmitters[emitter_i].mModifiers)
                {
                    ModifierParams params =
                    {
                    .emitter = fx->mEmitters[emitter_i],
                    .dt = dt,
                    .particle_index = i
                    };
                    modifier->Modify(params);
                }

                p.Position += p.Velocity * 0.016f;
                p.Scale *= transform->Scale;
                p.DistanceToCamera = glm::length(SceneManager::ActiveCamera()->Position - p.Position);       

                for (auto& sub : fx->mEmitters[emitter_i].mSubEmitters)
                {                   
                    if (sub.EmmitStage == ParticleEmitter::SubEmittStageOption::During)
                    {
                        EmittParticleEmitter(sub, p.Position, transform->Rotation);
                    }
                }     

            }    


            for (int e = 0; e < fx->mEmitters[emitter_i].mSubEmitters.size(); e++)
            {
                if (fx->FxState == ParticleFX::StateOptions::Playing)
                {
                    fx->mEmitters[emitter_i].mSubEmitters[e].CurrentTime += 0.016f;
                }

                for (int i = 0; i < fx->mEmitters[emitter_i].mSubEmitters[e].Particles.size(); i++)
                {
                    auto& p = fx->mEmitters[emitter_i].mSubEmitters[e].Particles[i];
                    p.LifeSpan -= 0.016f;

                    if (p.LifeSpan <= 0.0f)
                    {
                        p.IsAlive = false;
                        p.DistanceToCamera = -1.0f;
                        fx->mEmitters[emitter_i].mSubEmitters[e].AliveParticles -= 1;
                    }
                    if (!p.IsAlive)
                    {
                        p.DistanceToCamera = -1.0f;
                        continue;
                    }


                    for (const auto& modifier : fx->mEmitters[emitter_i].mSubEmitters[e].mModifiers)
                    {
                        ModifierParams params =
                        {
                        .emitter = fx->mEmitters[emitter_i].mSubEmitters[e],
                        .dt = dt,
                        .particle_index = i
                        };
                        modifier->Modify(params);
                    }

                    p.Position += p.Velocity * 0.016f;
                    p.Scale *= transform->Scale;
                    p.DistanceToCamera = glm::length(SceneManager::ActiveCamera()->Position - p.Position);
                }

            }
        }

       
    }
}

void Fracture::ParticleSystem::BeginRender(Fracture::RenderContext* context)
{
    OPTICK_EVENT();


    for (auto& batch : mRenderBatch)
    {
        batch.second.mPartilces.clear();
        batch.second.InstanceData.clear();
    }

    for (const auto& component : SceneManager::GetAllComponents<ParticleSystemComponent>())
    {
        if (!component->IsFXAttached)
            continue;

        const auto& fx = GetParticleFX(component->GetID());
        for (const auto& emitter : fx->mEmitters)
        {
            AddToBatch(emitter.second);
        }
       
    }

    for (auto& batch : mRenderBatch)
    {
        std::sort(batch.second.mPartilces.begin(), batch.second.mPartilces.end(), [](Particle& a, Particle& b) {return a < b; });

        for (const auto& particle : batch.second.mPartilces)
        {
            ParticleInstanceData data;
            data.Colour = particle.Color;
            data.PositionScale = glm::vec4(particle.Position, particle.Scale.x);
            batch.second.InstanceData.push_back(data);
        }
    }

    for (auto& batch : mRenderBatch)
    {
        if (batch.second.mPartilces.empty())
            continue;
        Fracture::RenderCommands::BufferSubData<ParticleInstanceData>(context, batch.second.Instance_Buffer->RenderID, batch.second.InstanceData, batch.second.InstanceData.size() * sizeof(ParticleInstanceData), 0);
        //Fracture::RenderCommands::BufferSubData<glm::vec4>(context, batch.second.Color_Buffer->RenderID, batch.second.Colors, batch.second.Colors.size() * sizeof(glm::vec4), 0);
    }
}

void Fracture::ParticleSystem::Render(Fracture::RenderContext* context)
{
    OPTICK_EVENT();
    const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

    if (!global_color)
        return;

    int clearValue = 1;

    RenderCommands::SetRenderTarget(context, global_color);
    RenderCommands::Enable(context, Fracture::GLCapability::DepthTest);    
    RenderCommands::Disable(context, Fracture::GLCapability::FaceCulling);
    RenderCommands::Enable(context, Fracture::GLCapability::Blending);
  

    for (const auto& batch : mRenderBatch)
    {
        const auto& material = AssetManager::GetMaterialByID(batch.first);
        if (material)
        {
            switch (material->BlendMode)
            {
            case BlendingModeOption::Additive:
            {
                RenderCommands::DepthMask(context, false);
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::One);
                break;
            }
            case BlendingModeOption::Linear:
            {
                RenderCommands::DepthMask(context, true);
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
                break;
            }
            case BlendingModeOption::Multiply:
            {
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::DstColor, Fracture::BlendFunc::Zero);
                break;
            }
            }

            const auto& shader = AssetManager::GetShaderByID(material->Shader);
            RenderCommands::UseProgram(context, shader->Handle);
            RenderCommands::SetUniform(context, shader.get(), "HasTexture", material->HasAlbedoTexture);
            if (material->HasAlbedoTexture)
            {
                const auto& texture = AssetManager::GetTextureByID(material->AlbedoTexture);
                if(texture)
                    RenderCommands::SetTexture(context, shader.get(), "InTexture", texture->Handle, 0);
            }

            Fracture::RenderCommands::BindVertexArrayObject(context, batch.second.VAO);
            if (batch.second.InstanceData.size())
            {
                DrawArraysInstanced cmd;
                cmd.mode = DrawMode::TriangleStrip;
                cmd.instance_count = batch.second.InstanceData.size();
                cmd.first = 0;
                cmd.count = 4;
                Fracture::RenderCommands::DrawArrayInstanced(context, cmd);
            }

            RenderCommands::UseProgram(context, 0);
        }
    }

    /*
    for (const auto& component : SceneManager::GetAllComponents<ParticleSystemComponent>())
    {
        if (!component->IsFXAttached)
            continue;

        const auto& fx = GetParticleFX(component->GetID());


        for (auto emitter : fx->mEmitters)
        {
            std::sort(emitter.second.Particles.begin(), emitter.second.Particles.end(), [](Particle& a, Particle& b) {return a < b; });

            switch (emitter.second.BlendMode)
            {
            case ParticleEmitter::BlendingModeOption::Additive:
            {
                RenderCommands::DepthMask(context, false);
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::One);
                break;
            }
            case ParticleEmitter::BlendingModeOption::Linear:
            {
                RenderCommands::DepthMask(context, true);
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
                break;
            }
            case ParticleEmitter::BlendingModeOption::Multiply:
            {
                RenderCommands::BlendFunction(context, Fracture::BlendFunc::DstColor, Fracture::BlendFunc::Zero);
                break;
            }
            }

            const auto& shader = AssetManager::GetShader("Particles");
            RenderCommands::UseProgram(context, shader->Handle);
            RenderCommands::SetUniform(context, shader.get(), "HasTexture", emitter.second.HasParticleTexture);
            if (emitter.second.HasParticleTexture)
            {
                const auto& texture = AssetManager::GetTextureByID(emitter.second.ParticleTexture);
                RenderCommands::SetTexture(context, shader.get(), "InTexture", texture->Handle, 0);
            }

            for (int i = 0; i < emitter.second.Particles.size(); i++)
            {
                if (!emitter.second.Particles[i].IsAlive)
                    continue;

                RenderCommands::SetUniform(context, shader.get(), "Color", emitter.second.Particles[i].Color);
                RenderCommands::SetUniform(context, shader.get(), "Position", emitter.second.Particles[i].Position);
                RenderCommands::SetUniform(context, shader.get(), "Scale", emitter.second.Particles[i].Scale);

                Fracture::RenderCommands::BindVertexArrayObject(context, QuadVAO);
                DrawArray cmd =
                {
                    .mode = DrawMode::TriangleStrip,
                    .first = 0,
                    .count = 4
                };
                Fracture::RenderCommands::DrawArray(context, cmd);

            }
            RenderCommands::UseProgram(context, 0);

            for (auto subemitter : emitter.second.mSubEmitters)
            {
                std::sort(subemitter.Particles.begin(), subemitter.Particles.end(), [](Particle& a, Particle& b) {return a < b; });

                switch (subemitter.BlendMode)
                {
                case ParticleEmitter::BlendingModeOption::Additive:
                {
                    RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::One);
                    break;
                }
                case ParticleEmitter::BlendingModeOption::Linear:
                {
                    RenderCommands::BlendFunction(context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
                    break;
                }
                case ParticleEmitter::BlendingModeOption::Multiply:
                {
                    RenderCommands::BlendFunction(context, Fracture::BlendFunc::DstColor, Fracture::BlendFunc::Zero);
                    break;
                }
                }

                const auto& shader = AssetManager::GetShader("Particles");
                RenderCommands::UseProgram(context, shader->Handle);
                RenderCommands::SetUniform(context, shader.get(), "HasTexture", subemitter.HasParticleTexture);
                if (subemitter.HasParticleTexture)
                {
                    const auto& texture = AssetManager::GetTextureByID(subemitter.ParticleTexture);
                    RenderCommands::SetTexture(context, shader.get(), "InTexture", texture->Handle, 0);

                }


                for (int i = 0; i < subemitter.Particles.size(); i++)
                {
                    if (!subemitter.Particles[i].IsAlive)
                        continue;

                    RenderCommands::SetUniform(context, shader.get(), "Color", subemitter.Particles[i].Color);
                    RenderCommands::SetUniform(context, shader.get(), "Position", subemitter.Particles[i].Position);
                    RenderCommands::SetUniform(context, shader.get(), "Scale", subemitter.Particles[i].Scale);

                    Fracture::RenderCommands::BindVertexArrayObject(context, QuadVAO);
                    DrawArray cmd =
                    {
                        .mode = DrawMode::TriangleStrip ,
                        .first = 0,
                        .count = 4
                    };
                    Fracture::RenderCommands::DrawArray(context, cmd);

                }
                RenderCommands::UseProgram(context, 0);
            }
        }
        
        }

        RenderCommands::Disable(context, Fracture::GLCapability::Blending);
    }
    */

    RenderCommands::Disable(context, Fracture::GLCapability::Blending);
}

void Fracture::ParticleSystem::Shutdown()
{
}

void Fracture::ParticleSystem::EmittParticles(Fracture::UUID entity)
{
    if (SceneManager::HasComponent<ParticleSystemComponent>(entity))
    {
        const auto& fx = GetParticleFX(entity);
        fx->FxState = ParticleFX::StateOptions::Playing;
    }
}

void Fracture::ParticleSystem::StopParticles(Fracture::UUID entity)
{
    if (SceneManager::HasComponent<ParticleSystemComponent>(entity))
    {
        const auto& fx = GetParticleFX(entity);
        fx->FxState = ParticleFX::StateOptions::Stopped;
    }
}

void Fracture::ParticleSystem::UpdateObservers(Fracture::UUID fx)
{
    if (mFxObserversList.find(fx) != mFxObserversList.end())
    {
        for (const auto& observer : mFxObserversList[fx].observers)
        {            
            mParticleFXs.erase(observer);
            mParticleFXs[observer] = std::make_shared<ParticleFX>(*mParticleFXsLibrary[fx].get());
        }
    }
}

bool Fracture::ParticleSystem::EmittParticleEmitter(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation)
{
    float freq = emitter.SpawnRate / (float)emitter.ParticlesPerSec;
    float dt_t = emitter.TimeSinceLastSpawn + freq - emitter.StartOffset;

    if (emitter.CurrentTime >= dt_t)
    {
        emitter.TimeSinceLastSpawn = emitter.CurrentTime;
        for (int i = 0; i < emitter.ParticlesPerSec; i++)
        {
            int particle_index = FindLastUsedParticleIndex(emitter);
            if (!emitter.Particles[particle_index].IsAlive)
            {
                glm::vec3 rand_vec = glm::vec3(0);
                rand_vec.x = (Random::Float() - 0.5f) * emitter.ParticleSpread.x;
                rand_vec.y = (Random::Float() - 0.5f) * emitter.ParticleSpread.y;
                rand_vec.z = (Random::Float() - 0.5f) * emitter.ParticleSpread.z;

                glm::vec3 rand_velocity = glm::vec3(0);
                float r = std::tan(emitter.EmittionAngle);

                rand_velocity.z = (2.0f * Random::Float() - 1.0f) * r;
                rand_velocity.x = (2.0f * Random::Float() - 1.0f) * r;
                rand_velocity.y = 1;
                rand_velocity = glm::normalize(rand_velocity);

                emitter.AliveParticles += 1;
                emitter.Particles[particle_index].IsAlive = true;
                emitter.Particles[particle_index].Position = Position + rand_vec + emitter.PositionOffset;
                emitter.Particles[particle_index].LifeSpan = emitter.ParticleLifeSpan;
                emitter.Particles[particle_index].Velocity = Rotation * rand_velocity * emitter.ParticleSpeed;
                emitter.ParticlePoolIndex = ++emitter.ParticlePoolIndex % emitter.Particles.size();
            }
        }

        return true;
    }
    return false;
}

bool Fracture::ParticleSystem::EmittParticleContius(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation)
{
    for (int i = 0; i < emitter.ParticlesPerSec; i++)
    {
        int particle_index = FindLastUsedParticleIndex(emitter);
        if (!emitter.Particles[particle_index].IsAlive)
        {
            glm::vec3 rand_vec = glm::vec3(0);
            rand_vec.x = (Random::Float() - 0.5f) * emitter.ParticleSpread.x;
            rand_vec.y = (Random::Float() - 0.5f) * emitter.ParticleSpread.y;
            rand_vec.z = (Random::Float() - 0.5f) * emitter.ParticleSpread.z;

            glm::vec3 rand_velocity = glm::vec3(0);
            float r = std::tan(emitter.EmittionAngle);

            rand_velocity.z = (2.0f * Random::Float() - 1.0f) * r;
            rand_velocity.x = (2.0f * Random::Float() - 1.0f) * r;
            rand_velocity.y = 1;
            rand_velocity = glm::normalize(rand_velocity);

            emitter.AliveParticles += 1;
            emitter.Particles[particle_index].IsAlive = true;
            emitter.Particles[particle_index].Position = Position + rand_vec + emitter.PositionOffset;
            emitter.Particles[particle_index].LifeSpan = emitter.ParticleLifeSpan;
            emitter.Particles[particle_index].Velocity = rand_velocity * emitter.ParticleSpeed * Rotation;
            emitter.ParticlePoolIndex = ++emitter.ParticlePoolIndex % emitter.Particles.size();
        }
    }
    return true;
}

Fracture::ParticleFX* Fracture::ParticleSystem::GetParticleFX(Fracture::UUID id)
{
    if (mParticleFXs.find(id) == mParticleFXs.end())
    {
        const auto& comp = SceneManager::GetComponent<ParticleSystemComponent>(id);
        if (mParticleFxRegister.find(comp->particleFXID) == mParticleFxRegister.end())
        {
            FRACTURE_ERROR("Particle FX {} is not registered", comp->particleFXID);
            return nullptr;
        }
        else
        {            
            mParticleFXs[id] = std::make_shared<ParticleFX>(*GetMainParticleFX(comp->GetID()));
            mFxObserversList[comp->particleFXID].observers.push_back(id);
            return mParticleFXs[id].get();
        }       
    }
    else
    {
        return mParticleFXs[id].get();
    }
    return nullptr;
}

Fracture::ParticleFX* Fracture::ParticleSystem::GetMainParticleFX(Fracture::UUID id)
{
    if (SceneManager::HasComponent<ParticleSystemComponent>(id))
    {
        const auto& comp = SceneManager::GetComponent<ParticleSystemComponent>(id);
        if (mParticleFXsLibrary.find(comp->particleFXID) == mParticleFXsLibrary.end())
        {
            if (mParticleFxRegister.find(comp->particleFXID) == mParticleFxRegister.end())
            {
                FRACTURE_ERROR("Particle FX {} is not registered", comp->particleFXID);
                return nullptr;
            }
            else
            {
                ParticleFxSerialiser loader = ParticleFxSerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
                loader.Open(mParticleFxRegister[comp->particleFXID].Path);
                auto fx = loader.ReadParticleFX();
                if (fx)
                {
                    mParticleFXsLibrary[comp->particleFXID] = fx;
                    mFxObserversList[comp->particleFXID] = ParticleFXObservers();
                    return mParticleFXsLibrary[comp->particleFXID].get();
                }
            }
        }
        else
        {
            return mParticleFXsLibrary[comp->particleFXID].get();
        }
    }
    return nullptr;
}

int Fracture::ParticleSystem::FindLastUsedParticleIndex(ParticleEmitter& emitter)
{
    for (int i = emitter.LastUsedParticle; i < emitter.Particles.size(); i++)
    {
        if (!emitter.Particles[i].IsAlive)
        {
            emitter.LastUsedParticle = i;           
            return i;
        }
    }

    for (int i = 0; i < emitter.LastUsedParticle; i++)
    {
        if (!emitter.Particles[i].IsAlive)
        {
            emitter.LastUsedParticle = i;           
            return i;
        }
    }
    return 0;
}

void Fracture::ParticleSystem::AddToBatch(const ParticleEmitter& emitter)
{
    int MAX_RENDERERED_PARTICLES = 10000;
    if (!emitter.HasMaterial)
        return;

    if (mRenderBatch.find(emitter.MaterialID) == mRenderBatch.end())
    {
        mRenderBatch[emitter.MaterialID] = ParticleRenderBatch();
        mRenderBatch[emitter.MaterialID].MaterialID = emitter.MaterialID;
        mRenderBatch[emitter.MaterialID].InstanceData.resize(MAX_RENDERERED_PARTICLES);

        VertexArrayCreationInfo info;
        info.Layout =
        {
            { ShaderDataType::Float3,"aPos",0,true },
            { ShaderDataType::Float3,"aNormal",0,true },
            { ShaderDataType::Float2,"aUV",0,true },
            { ShaderDataType::Float4, "ParticleColour",1 },
            { ShaderDataType::Float4, "PositionScale",1 }
        };
        GraphicsDevice::Instance()->CreateVertexArray(mRenderBatch[emitter.MaterialID].VAO, info);
        GraphicsDevice::Instance()->VertexArray_BindAttributes(mRenderBatch[emitter.MaterialID].VAO, info);
        {
            GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mRenderBatch[emitter.MaterialID].VAO, 0, sizeof(mBillboard->mVertices[0]), mBillboard->Vertex_Buffer->RenderID, 0);
            GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mRenderBatch[emitter.MaterialID].VAO, 1, sizeof(mBillboard->mVertices[0]), mBillboard->Vertex_Buffer->RenderID, sizeof(glm::vec3));
            GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mRenderBatch[emitter.MaterialID].VAO, 2, sizeof(mBillboard->mVertices[0]), mBillboard->Vertex_Buffer->RenderID, sizeof(glm::vec3) * 2);
        }       
        {
            BufferDescription desc;
            desc.bufferType = BufferType::ArrayBuffer;
            desc.size = sizeof(ParticleInstanceData) * MAX_RENDERERED_PARTICLES;
            desc.usage = BufferUsage::Stream;
            desc.Name = "ParticleMatricies";
            desc.data = nullptr;
            mRenderBatch[emitter.MaterialID].Instance_Buffer = std::make_shared<Buffer>();
            GraphicsDevice::Instance()->CreateBuffer(mRenderBatch[emitter.MaterialID].Instance_Buffer.get(), desc);
            GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mRenderBatch[emitter.MaterialID].VAO, 3, sizeof(ParticleInstanceData), mRenderBatch[emitter.MaterialID].Instance_Buffer->RenderID);

            GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mRenderBatch[emitter.MaterialID].VAO, 4, sizeof(ParticleInstanceData), mRenderBatch[emitter.MaterialID].Instance_Buffer->RenderID, sizeof(glm::vec4));

            GraphicsDevice::Instance()->VertexArray_SetDivisor(mRenderBatch[emitter.MaterialID].VAO, 3, 1);
            GraphicsDevice::Instance()->VertexArray_SetDivisor(mRenderBatch[emitter.MaterialID].VAO, 4, 1);
        }
    }

    for (const auto& particle : emitter.Particles)
    {
        if (!particle.IsAlive)
            continue;

        mRenderBatch[emitter.MaterialID].mPartilces.push_back(particle);
    }

    for (auto& subemitter : emitter.mSubEmitters)
    {
        if (!subemitter.HasMaterial)
            continue;
        AddToBatch(subemitter);
    }
    
}
