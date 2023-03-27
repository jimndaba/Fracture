#include "FracturePCH.h"
#include "PhysicsManager.h"
#include "EventSystem/Eventbus.h"
#include "PhysicsEvents.h"
#include "World/SceneManager.h"
#include "World/Components.h"

Fracture::PhysicsManager::PhysicsManager(const PhysicsConfiguration& config):Config(config)
{
}

void Fracture::PhysicsManager::OnInit()
{
	_broadphase = new btDbvtBroadphase();
	_collisionConfig = new btDefaultCollisionConfiguration();
	_collisionDispatcher = new btCollisionDispatcher(_collisionConfig);
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_collisionDispatcher,_broadphase,_solver,_collisionConfig);

	_world->setGravity(btVector3(Config.Gravity.x, Config.Gravity.y, Config.Gravity.z));
}

void Fracture::PhysicsManager::OnShutdown()
{
	mRigidBodies.clear();
	mCollisionShapes.clear();

	delete _world;
	delete _solver;
	delete _collisionConfig;
	delete _collisionDispatcher;
	delete _broadphase;
}

void Fracture::PhysicsManager::OnClearPhysicsWorld()
{
	for(const auto& rigid : mRigidBodies)
	{
		_world->removeRigidBody(rigid.second.get());
	}

	mRigidBodies.clear();
	mCollisionShapes.clear();
}

void Fracture::PhysicsManager::OnPhysicsUpdate(float dt)
{
	_world->stepSimulation(1.0f / 60.0f, 10);

	for (const auto& body : mRigidBodies)
	{
		if (body.second->getMotionState())
		{
			const auto& entity = (Fracture::UUID)body.second->getUserPointer();
			const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(entity);
			if (!rigidbody->IsDynamic)
			{
				continue;
			}

			const auto& transform = SceneManager::GetComponent<TransformComponent>(entity);
			btTransform trans = body.second->getWorldTransform();
			transform->Position = { trans.getOrigin().getX(),trans.getOrigin().getY() ,trans.getOrigin().getZ() };
			transform->Rotation = { trans.getRotation().getX(),trans.getRotation().getY() ,trans.getRotation().getZ() ,trans.getRotation().getW() };
		}
	}
}

void Fracture::PhysicsManager::OnCollisionUpdate()
{
	int numManifolds = _world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		const auto& contactManifold = _world->getDispatcher()->getManifoldByIndexInternal(i);
		
		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			//5
			const auto& obA = contactManifold->getBody0();
			const auto& obB = contactManifold->getBody1();

			//6
			const auto& pnA = (Fracture::UUID)obA->getUserPointer();
			CollisionContext a;
			a.entity = pnA;
			
			const auto& pnB = (Fracture::UUID)obB->getUserPointer();
			CollisionContext b;
			b.entity = pnB;

			Eventbus::Publish<OnCollisionEvent>(a,b);

		}

	}
}

void Fracture::PhysicsManager::SetGravity(const glm::vec3& g)
{
	_world->setGravity(btVector3(g.x, g.y, g.z));
}

void Fracture::PhysicsManager::SetMovementConstraints(const Fracture::UUID& entity,const bool c[3])
{
	if (mRigidBodies.find(entity) != mRigidBodies.end() && mRigidBodies[entity])
		mRigidBodies[entity]->setLinearFactor(btVector3(c[0], c[1], c[2]));

}

void Fracture::PhysicsManager::SetRotationConstraints(const Fracture::UUID& entity, const bool c[3])
{
	if(mRigidBodies.find(entity) != mRigidBodies.end() && mRigidBodies[entity])
		mRigidBodies[entity]->setAngularFactor(btVector3(c[0], c[1], c[2]));
}

void Fracture::PhysicsManager::AddRigidBodyToWorld(Fracture::UUID entity)
{
	if (mCollisionShapes.find(entity) == mCollisionShapes.end())
	{
		FRACTURE_ERROR("No Collider Assigned to entity: {}", entity);
		FRACTURE_ERROR("Attach COllider before Rigidbody");
		return;
	}

	const auto& transform = SceneManager::GetComponent<TransformComponent>(entity);
	const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(entity);

	btQuaternion rotation = {transform->Rotation.x,transform->Rotation.y,transform->Rotation.z ,transform->Rotation.w};
	btVector3 position = btVector3(transform->Position.x, transform->Position.y, transform->Position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	btScalar bodyMass = rigidbody->Mass;
	btVector3 bodyInertia;
	
	
	mCollisionShapes[entity]->calculateLocalInertia(bodyMass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, mCollisionShapes[entity].get(), bodyInertia);
	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;

	mRigidBodies[entity] = std::make_shared<btRigidBody>(bodyCI);
	mRigidBodies[entity]->setUserPointer(&entity);
	mRigidBodies[entity]->setLinearFactor(btVector3(rigidbody->LinearConstraints[0], rigidbody->LinearConstraints[1], rigidbody->LinearConstraints[2]));
	mRigidBodies[entity]->setAngularFactor(btVector3(rigidbody->AngularConstraints[0], rigidbody->AngularConstraints[1], rigidbody->AngularConstraints[2]));


	_world->addRigidBody(mRigidBodies[entity].get());	
}

void Fracture::PhysicsManager::AttachCollisionShape(const Fracture::UUID& entity)
{
	if (mCollisionShapes[entity])
	{
		return;
	}
	const auto& colliderComp = SceneManager::GetComponent<ColliderComponent>(entity);

	switch (colliderComp->Shape)
	{
		case ColliderType::Sphere:
		{
			mCollisionShapes[entity] = std::make_shared<btSphereShape>(colliderComp->Radius);
			break;
		}
		case ColliderType::Box:
		{
			mCollisionShapes[entity] = std::make_shared<btBoxShape>(btVector3(colliderComp->Size.x / 2, colliderComp->Size.y / 2, colliderComp->Size.z / 2));
			break;
		}
		case ColliderType::Capsule:
		{
			mCollisionShapes[entity] = std::make_shared<btCapsuleShape>(colliderComp->Radius,colliderComp->Height);
			break;
		}
		case ColliderType::Cone:
		{
			mCollisionShapes[entity] = std::make_shared<btConeShape>(colliderComp->Radius, colliderComp->Height);
			break;
		}
		case ColliderType::Cylinder:
		{
			
			mCollisionShapes[entity] = std::make_shared<btCylinderShape>(btVector3(colliderComp->Size.x / 2, colliderComp->Size.y / 2, colliderComp->Size.z / 2));
			break;
		}
	}
}

void Fracture::PhysicsManager::RemoveCollider(const Fracture::UUID& entity)
{

}

void Fracture::PhysicsManager::RemoveRigidBody(const Fracture::UUID& entity)
{
	if (mRigidBodies.find(entity) != mRigidBodies.end())
	{
		_world->removeRigidBody(mRigidBodies[entity].get());
		mRigidBodies.erase(entity);
	}
}
