#include "PhysicsManager.h"
#include "Component/RigidBodyComponent.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Collisions.h"
#include "RayHit.h"
#include "Ray.h"
#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"
#include "Event/PhysicsEvents.h"
#include <iostream>
#include <algorithm>
#include "PhysicsDebugDraw.h"

#include "Bullet/include/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "Bullet/include/BulletCollision/Gimpact/btGImpactShape.h"

btDefaultCollisionConfiguration* Fracture::PhysicsManager::collisionConfiguration;
btCollisionDispatcher* Fracture::PhysicsManager::dispatcher;
btBroadphaseInterface* Fracture::PhysicsManager::overlappingPairCache;
btSequentialImpulseConstraintSolver* Fracture::PhysicsManager::solver;
btDiscreteDynamicsWorld* Fracture::PhysicsManager::dynamicsWorld;
btAlignedObjectArray<btCollisionShape*> Fracture::PhysicsManager::collisionShapes;
std::vector<int> Fracture::PhysicsManager::rigid_ids;
std::vector<int> Fracture::PhysicsManager::collision_ids;

Fracture::PhysicsManager::PhysicsManager()
{

	
}

Fracture::PhysicsManager::~PhysicsManager()
{


	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	collisionShapes.clear();

	delete collisionConfiguration;
	delete dispatcher;
	delete overlappingPairCache;
	delete solver;
	delete dynamicsWorld;
}

void Fracture::PhysicsManager::Init()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	setGravity(0, 0, 0);

	dynamicsWorld->setGravity(m_gravity);

	m_debug = new PhysicsDebugDraw();
	dynamicsWorld->setDebugDrawer(m_debug);

	
	///-----initialization_end----
}

void Fracture::PhysicsManager::setGravity(float x, float y, float z)
{
	m_gravity = btVector3(x,y,z);
}

void Fracture::PhysicsManager::ClearScene()
{
	collisionShapes.clear();

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

}

void Fracture::PhysicsManager::AddCollider(int id, btCollisionShape* collider)
{
	if (std::find(collision_ids.begin(), collision_ids.end(),id) != collision_ids.end())
	{
		FRACTURE_TRACE("Collider already exists");
	}
	else
	{
		collision_ids.push_back(id);
		collisionShapes.push_back(collider);
	}
}

void Fracture::PhysicsManager::RemoveCollider(btCollisionShape* collider)
{
	collisionShapes.remove(collider);
}

void Fracture::PhysicsManager::AddRigidBody(int id, btRigidBody* body)
{
	if (std::find(rigid_ids.begin(), rigid_ids.end(), id) != rigid_ids.end())
	{
		FRACTURE_TRACE("Rigid already exists");
	}
	else
	{
		rigid_ids.push_back(id);
		dynamicsWorld->addRigidBody(body);
	}
}

void Fracture::PhysicsManager::AddRigidBody(int id,btRigidBody* body,Fracture::CollisionGroup group, Fracture::CollisionMask mask)
{
	if (std::find(rigid_ids.begin(), rigid_ids.end(), id) != rigid_ids.end())
	{
		FRACTURE_TRACE("Rigid already exists");
	}
	else
	{
		rigid_ids.push_back(id);
		dynamicsWorld->addRigidBody(body,(int)group,(int)mask);
	}
}

void Fracture::PhysicsManager::RemoveRigidBody(btRigidBody* body)
{
	dynamicsWorld->removeRigidBody(body);
}

bool Fracture::PhysicsManager::RayCast(Ray rayIn, RayHit& out)
{
	glm::vec3 out_end = rayIn.GetEndPoint(1000.0f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(rayIn.GetOrigin().x, rayIn.GetOrigin().y, rayIn.GetOrigin().z),
		btVector3(out_end.x, out_end.y, out_end.z)
	);

	dynamicsWorld->updateAabbs();
	dynamicsWorld->computeOverlappingPairs();

	dynamicsWorld->rayTest(
		btVector3(rayIn.GetOrigin().x, rayIn.GetOrigin().y, rayIn.GetOrigin().z),
		btVector3(out_end.x, out_end.y, out_end.z),
		RayCallback
	);

	if (RayCallback.hasHit()) {
		
		FRACTURE_INFO("mesh : {} " ,(int)RayCallback.m_collisionObject->getUserPointer());
		out.ID = (int)RayCallback.m_collisionObject->getUserPointer();
		return true;
	}	
	FRACTURE_INFO("No Hit ");
	return false;
}

void Fracture::PhysicsManager::startPhysics()
{
	
	std::vector<std::shared_ptr<Component>> components = ComponentManager::GetAllComponents();	

	for (auto& component : components)
	{
		std::shared_ptr<RigidBodyComponent> c = std::dynamic_pointer_cast<RigidBodyComponent>(component);
		if (c)
		{
			if (!(std::find(rigid_ids.begin(), rigid_ids.end(), c->EntityID) != rigid_ids.end()))
			{
				AddRigidBody(c->EntityID, c->m_rigid, c->collisionGroup, c->collisionMask);
			}
		}			
	}
	for (auto& component : components)
	{
		std::shared_ptr<BoxColliderComponent> c = std::dynamic_pointer_cast<BoxColliderComponent>(component);
		if (c)
		{
			if (!(std::find(collision_ids.begin(), collision_ids.end(), c->EntityID) != collision_ids.end()))
			{
				AddCollider(c->EntityID, c->m_boxCollider);
			}
		}		
	}

}

void Fracture::PhysicsManager::onUpdate(float dt)
{

	//check any new Rigid or colliders
	startPhysics();

	ProfilerTimer timer("Physics Update");
	if (dynamicsWorld)
	{
		dynamicsWorld->stepSimulation(1.0f / 60.f);
	}
	
	checkCollision();

	int count = dynamicsWorld->getNumCollisionObjects();

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		if (body && body->getMotionState()) {
			
			body->getMotionState()->getWorldTransform(trans);			
			
			void* userPointer = body->getUserPointer();
			if (userPointer) {
				RigidBodyComponent* component = static_cast<RigidBodyComponent*>(userPointer);			

				btQuaternion orientation = trans.getRotation();

				std::shared_ptr<TransformComponent> transcomponent = ComponentManager::GetComponent<TransformComponent>(component->EntityID);

				if(transcomponent)				
				{
					transcomponent->Position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
					transcomponent->Rotation = glm::vec3(orientation.getX(), orientation.getY(), orientation.getZ());
				}
				
			}
		}
		
	}
	

}

void Fracture::PhysicsManager::DrawDebug()
{
	ProfilerTimer timer("Physics Debug Draw");
	dynamicsWorld->debugDrawWorld();
}

void Fracture::PhysicsManager::checkCollision()
{
	//std::map< btCollisionObject*, Collision*> new_contacts;
	int num_manifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < num_manifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		/* Check all contacts points */
		int numContacts = contactManifold->getNumContacts();
		

		RigidBodyComponent* pointerA = (RigidBodyComponent*)obA->getUserPointer();
		RigidBodyComponent* pointerB = (RigidBodyComponent*)obB->getUserPointer();

		Eventbus::Publish(new CollisionEvent(pointerA->EntityID,pointerB->EntityID));	

	}

}
