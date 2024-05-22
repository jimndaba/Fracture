#include "FracturePCH.h"
#include "ParticleFXSerialiser.h"
#include "Particles/Particle.h"
#include "Particles/EmitterModifiers.h"

Fracture::ParticleFxSerialiser::ParticleFxSerialiser(IOMode mode, SerialiseFormat format):
	ISerialiser(mode,format)
{
}

void Fracture::ParticleFxSerialiser::WriteParticleFX(Fracture::ParticleFX* fx)
{
	BeginStruct("ParticleFx");
	{
		Property("ID", fx->ID);
		Property("Name", fx->Name);
		Property("Duration", fx->Duration);		
		Property("PlayMode", (int)fx->PlayMode);

		BeginCollection("Emitters");
		for (auto& emitter : fx->mEmitters)
		{
			SerialiseEmitter(emitter.second);			
		}
		EndCollection();

	}
	EndStruct();
}

std::shared_ptr<Fracture::ParticleFX> Fracture::ParticleFxSerialiser::ReadParticleFX()
{
	if (BeginStruct("ParticleFx"))
	{
		auto fx = std::make_shared<ParticleFX>();

		fx->ID = ID("ID");
		fx->Name= STRING("Name");
		fx->Duration = FLOAT("Duration");
		fx->PlayMode = (ParticleFX::PlayModeOptions)INT("PlayMode");

		if (BeginCollection("Emitters"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				ReadEmitter(fx.get());				
				NextInCollection();
			}
			EndCollection();
		}
		
		EndStruct();
		return fx;
	}
	return nullptr;
}

void Fracture::ParticleFxSerialiser::SerialiseEmitter(ParticleEmitter& emitter)
{
	BeginStruct("Emitter");
	{
		Property("Name", emitter.Name);
		Property("Duration", emitter.Duration);
		Property("StartOffset", emitter.StartOffset);
		Property("HasParticleTexture", emitter.HasParticleTexture);
		Property("IsTextureAtlas", emitter.IsTextureAtlas);
		Property("MaterialID", emitter.MaterialID);
		Property("HasMaterial", emitter.HasMaterial);
		Property("MeshID", emitter.MeshID);
		Property("ParticlesPerSec", emitter.ParticlesPerSec);
		Property("EmittionAngle", emitter.EmittionAngle);
		Property("BlendMode", (int)emitter.BlendMode);
		Property("EmitterType", (int)emitter.EmitterType);
		Property("ParticleLifeSpan", emitter.ParticleLifeSpan);
		Property("ParticleSpawnRate", emitter.SpawnRate);
		Property("ParticleSpeed", emitter.ParticleSpeed);
		Property("ParticleTexture", emitter.ParticleTexture);
		Property("ParticleSpread", emitter.ParticleSpread);
		Property("PositionOffset", emitter.PositionOffset);

		BeginCollection("Modifiers");
		{
			for (const auto& modifier : emitter.mModifiers)
			{
				WriteModifierOfType<ColourGradientModifier>(modifier);
				WriteModifierOfType<ScaleModifier>(modifier);
				WriteModifierOfType<GravityModifier>(modifier);
			}
		}
		EndCollection();

		BeginCollection("SubEmitters");
		for (auto& sub : emitter.mSubEmitters)
		{
			SerialiseEmitter(sub);
		}
		EndCollection();
	}
	EndStruct();

}

void Fracture::ParticleFxSerialiser::SerialiseModifier(GravityModifier* modifier)
{
	BeginStruct("Gravity");
	{
		Property("GravitiyMultiplier", modifier->GravityMultiplier);
	}
	EndStruct();
}

void Fracture::ParticleFxSerialiser::SerialiseModifier(ColourGradientModifier* modifier)
{
	BeginStruct("ColourGradient");
	{
		Property("Start", modifier->StartColour);
		Property("End", modifier->EndColour);
	}
	EndStruct();
}

void Fracture::ParticleFxSerialiser::SerialiseModifier(ScaleModifier* modifier)
{
	BeginStruct("Scale");
	{
		Property("Start", modifier->StartScale);
		Property("End", modifier->EndScale);
	}
	EndStruct();
}

void Fracture::ParticleFxSerialiser::ReadGravityModifierIfExists(ParticleEmitter* em)
{
	if (BeginStruct("Gravity"))
	{
		auto gravity = std::make_shared<GravityModifier>();
		gravity->GravityMultiplier = FLOAT("GravitiyMultiplier");
		em->mModifiers.push_back(gravity);
		EndStruct();
	}
}

void Fracture::ParticleFxSerialiser::ReadColourGradientModifierIfExists(ParticleEmitter* em)
{
	if (BeginStruct("ColourGradient"))
	{
		auto mod = std::make_shared<ColourGradientModifier>();
		mod->StartColour = VEC4("Start");
		mod->EndColour = VEC4("End");
		em->mModifiers.push_back(mod);
		EndStruct();
	}
}

void Fracture::ParticleFxSerialiser::ReadScaleModifierIfExists(ParticleEmitter* em)
{
	if (BeginStruct("Scale"))
	{
		auto mod = std::make_shared<ScaleModifier>();
		mod->StartScale = VEC3("Start");
		mod->EndScale = VEC3("End");
		em->mModifiers.push_back(mod);
		EndStruct();
	}
}

void Fracture::ParticleFxSerialiser::ReadEmitter(ParticleFX* fx)
{
	if (BeginStruct("Emitter"))
	{
		auto emitter = ParticleEmitter();
		emitter.Particles.resize(1000);
		emitter.Name = STRING("Name");
		emitter.Duration = FLOAT("Duration");
		emitter.StartOffset = FLOAT("StartOffset");
		emitter.HasParticleTexture = BOOL("HasParticleTexture");
		emitter.IsTextureAtlas = BOOL("IsTextureAtlas");
		emitter.MaterialID = ID("MaterialID");
		emitter.HasMaterial = BOOL("HasMaterial");
		emitter.MeshID = ID("MeshID");
		emitter.ParticlesPerSec = FLOAT("ParticlesPerSec");
		emitter.EmittionAngle = FLOAT("EmittionAngle");
		emitter.BlendMode = (ParticleEmitter::BlendingModeOption)INT("BlendMode");
		emitter.EmitterType = (ParticleEmitter::EmitterTypeOption)INT("EmitterType");
		emitter.ParticleLifeSpan = FLOAT("ParticleLifeSpan");
		emitter.ParticleSpeed = FLOAT("ParticleSpeed");
		emitter.SpawnRate = FLOAT("ParticleSpawnRate");
		emitter.ParticleTexture = ID("ParticleTexture");
		emitter.ParticleSpread = VEC3("ParticleSpread");
		emitter.PositionOffset = VEC3("PositionOffset");

		if (BeginCollection("Modifiers"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (HasKey("Gravity"))
					ReadGravityModifierIfExists(&emitter);
				if (HasKey("ColourGradient"))
					ReadColourGradientModifierIfExists(&emitter);
				if (HasKey("Scale"))
					ReadScaleModifierIfExists(&emitter);
				NextInCollection();
			}
			EndCollection();
		}

		if (BeginCollection("SubEmitters"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				ReadSubEmitter(&fx->mEmitters[CurrentCollectionIndex()]);
				NextInCollection();
			}
			EndCollection();
		}

		fx->mEmitters[fx->mEmitters.size()] = emitter;
		EndStruct();
	}
}

void Fracture::ParticleFxSerialiser::ReadSubEmitter(ParticleEmitter* fx)
{
	if (BeginStruct("Emitter"))
	{
		auto emitter = ParticleEmitter();
		emitter.Particles.resize(1000);
		emitter.Name = STRING("Name");
		emitter.Duration = FLOAT("Duration");
		emitter.StartOffset = FLOAT("StartOffset");
		emitter.HasParticleTexture = BOOL("HasParticleTexture");
		emitter.IsTextureAtlas = BOOL("IsTextureAtlas");
		emitter.MaterialID = ID("MaterialID");
		emitter.MeshID = ID("MeshID");
		emitter.ParticlesPerSec = FLOAT("ParticlesPerSec");
		emitter.EmittionAngle = FLOAT("EmittionAngle");
		emitter.BlendMode = (ParticleEmitter::BlendingModeOption)INT("BlendMode");
		emitter.EmitterType = (ParticleEmitter::EmitterTypeOption)INT("EmitterType");
		emitter.ParticleLifeSpan = FLOAT("ParticleLifeSpan");
		emitter.ParticleSpeed = FLOAT("ParticleSpeed");
		emitter.ParticleTexture = ID("ParticleTexture");
		emitter.ParticleSpread = VEC3("ParticleSpread");
		emitter.PositionOffset = VEC3("PositionOffset");

		if (BeginCollection("Modifiers"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (HasKey("Gravity"))
					ReadGravityModifierIfExists(&emitter);
				if (HasKey("ColourGradient"))
					ReadColourGradientModifierIfExists(&emitter);
				if (HasKey("Scale"))
					ReadScaleModifierIfExists(&emitter);

				NextInCollection();
			}
			EndCollection();
		}
		fx->mSubEmitters[fx->mSubEmitters.size()] = emitter;
		EndStruct();
	}
}
