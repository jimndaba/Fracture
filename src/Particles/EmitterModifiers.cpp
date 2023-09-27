#include "FracturePCH.h"
#include "EmitterModifiers.h"
#include "Particle.h"

void Fracture::GravityModifier::Modify(ModifierParams params)
{
	if(GravityMultiplier > 0.0f)
	{
		params.emitter.Particles[params.particle_index].Velocity.y += (-9.81) * GravityMultiplier * params.dt;
	}
}

void Fracture::ColourGradientModifier::Modify(ModifierParams params)
{
	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;
	params.emitter.Particles[params.particle_index].Color = glm::lerp(EndColour, StartColour, t);
}

void Fracture::ScaleModifier::Modify(ModifierParams params)
{
	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;
	params.emitter.Particles[params.particle_index].Scale = glm::lerp(EndScale, StartScale, t);
}
