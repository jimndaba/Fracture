#include "FracturePCH.h"
#include "EmitterModifiers.h"
#include "Particle.h"
#include "Rendering/GraphicsDevice.h"
#include "World/SceneManager.h"

// Permutation table
static const int perm[] = { 151,160,137,91,90,15,
							131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
							8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
							35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
							77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54, 65,25,63,
							161, 1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
							3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,
							42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,129,22,39,253,19,98,108,
							110,79,113,224,232,178,185, 112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,
							51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
							138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

void Fracture::GravityModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	if(GravityMultiplier > 0.0f)
	{
		params.emitter.Particles[params.particle_index].Velocity.y += (-9.81) * GravityMultiplier * params.dt;
	}
}

void Fracture::ColourGradientModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;
	params.emitter.Particles[params.particle_index].Color = glm::lerp(EndColour, StartColour, t);
}

void Fracture::ScaleModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;
	params.emitter.Particles[params.particle_index].Scale = glm::lerp(EndScale, StartScale, t);
}

void Fracture::AngularVeloctiyModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;

	params.emitter.Particles[params.particle_index].Rotation.y +=  t * (MaxVelocity - MinVelocity) * params.dt;

}

void Fracture::WindModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	float t = params.emitter.Particles[params.particle_index].LifeSpan / params.emitter.ParticleLifeSpan;
	glm::vec3 direction = glm::vec3(GraphicsDevice::Instance()->WindSettings.WindDirection.x, GraphicsDevice::Instance()->WindSettings.WindDirection.y,GraphicsDevice::Instance()->WindSettings.WindDirection.z);
	params.emitter.Particles[params.particle_index].Velocity += direction * GraphicsDevice::Instance()->WindSettings.WindSpeed * GraphicsDevice::Instance()->WindSettings.WindStrength * Influence * params.dt;
}


int Fracture::NoiseModifier::hash(int x, int y, int z)
{	
	return perm[(perm[(perm[x % 256] + y) % 256] + z) % 256];
}

float Fracture::NoiseModifier::grad(int hash, float x, float y, float z)
{
	int h = hash & 15;  // Convert low 4 bits of hash code
	float u = h < 8 ? x : y;  // into 12 gradient directions.
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;  // Fix repeats at h = 12 to 15
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Fracture::NoiseModifier::perlinNoise(float x, float y, float z)
{
	int X = (int)floor(x) & 255;  // FIND UNIT CUBE THAT
	int Y = (int)floor(y) & 255;  // CONTAINS POINT.
	int Z = (int)floor(z) & 255;
	x -= floor(x);  // FIND RELATIVE X,Y,Z
	y -= floor(y);  // OF POINT IN CUBE.
	z -= floor(z);
	float u = fade(x);  // COMPUTE FADE CURVES
	float v = fade(y);  // FOR EACH OF X,Y,Z.
	float w = fade(z);
	int A = hash(X, Y, Z);
	int AA = hash(X, Y + 1, Z);
	int AB = hash(X, Y, Z + 1);
	int B = hash(X + 1, Y, Z);
	int BA = hash(X + 1, Y + 1, Z);
	int BB = hash(X + 1, Y, Z + 1);
	return lerp(w, lerp(v, lerp(u, grad(A, x, y, z), grad(B, x - 1, y, z)), lerp(u, grad(AA, x, y - 1, z), grad(BA, x - 1, y - 1, z))), lerp(v, lerp(u, grad(AB, x, y, z - 1), grad(BB, x - 1, y, z - 1)), lerp(u, grad(AA + 1, x, y - 1, z - 1), grad(BA + 1, x - 1, y - 1, z - 1))));
}

void Fracture::NoiseModifier::Modify(ModifierParams params)
{
	if (!Enabled)
		return;

	float noise = perlinNoise(
	params.emitter.Particles[params.particle_index].Position.x * NoiseScale + Frequency,
	params.emitter.Particles[params.particle_index].Position.y * NoiseScale + Frequency,
	params.emitter.Particles[params.particle_index].Position.z * NoiseScale + Frequency);

	// Apply noise to velocity
	params.emitter.Particles[params.particle_index].Velocity += glm::vec3(noise * MultiplierX, noise * MultiplierY, noise * MultiplierZ) * params.dt;
}

void Fracture::VortexModifier::Modify(ModifierParams params)
{
	const auto& transform = SceneManager::GetComponent<TransformComponent>(center_target);

	if (!transform)
		return;


	glm::vec3 direction = params.emitter.Particles[params.particle_index].Position - transform->Position;

	// Calculate the distance squared to avoid a costly square root
	float distanceSquared = glm::dot(direction, direction);

	// Normalize the direction vector
	direction = glm::normalize(direction);

	// Calculate the perpendicular direction for the vortex effect
	glm::vec3 perpendicularDirection = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));

	// Calculate the vortex velocity based on the distance and strength
	glm::vec3 vortexVelocity = perpendicularDirection * (VortexStrength / distanceSquared);
	
	params.emitter.Particles[params.particle_index].Velocity += vortexVelocity * VortexStrength * params.dt;
}
