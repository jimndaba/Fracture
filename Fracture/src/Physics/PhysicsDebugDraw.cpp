#include "PhysicsDebugDraw.h"
#include "GLAD/glad.h"
#include "Rendering/Renderer.h"

Fracture::PhysicsDebugDraw::PhysicsDebugDraw()
{
}

Fracture::PhysicsDebugDraw::~PhysicsDebugDraw()
{
}

void Fracture::PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//glColor3f(color.x(), color.y(), color.z());
	Renderer::DrawDebugLine(glm::vec3(from.getX(), from.getY(), from.getY()), glm::vec3(to.getX(),to.getY(), to.getZ()));
}

void Fracture::PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
}

void Fracture::PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void Fracture::PhysicsDebugDraw::drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&)
{
}

void Fracture::PhysicsDebugDraw::setDebugMode(int debugMode)
{
	m_debug = (DebugDrawModes)debugMode;
}

int Fracture::PhysicsDebugDraw::getDebugMode() const
{
	return m_debug;
}

void Fracture::PhysicsDebugDraw::clearLines()
{
}

void Fracture::PhysicsDebugDraw::flushLines()
{
}
