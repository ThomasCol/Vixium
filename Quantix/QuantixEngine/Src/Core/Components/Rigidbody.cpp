#include "Core/Components/Rigidbody.h"
#include "Core/DataStructure/GameComponent.h"

#include "Physic/PhysicHandler.h"
#include "Physic/PhysicDynamic.h"

#include "Core/MathHeader.h"

#include <iostream>

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<Quantix::Core::Components::Rigidbody>("Rigidbody")
	.constructor<>()
	.constructor<Quantix::Core::DataStructure::GameComponent*>()
	.constructor<const Quantix::Core::Components::Rigidbody&>()
	.constructor<Quantix::Core::Components::Rigidbody&&>()
	.property("Mass", &Quantix::Core::Components::Rigidbody::GetMass, &Quantix::Core::Components::Rigidbody::SetMass)
	.property("LinearVelocity", &Quantix::Core::Components::Rigidbody::GetLinearVelocity, &Quantix::Core::Components::Rigidbody::SetLinearVelocity)
	.property("AngularVelocity", &Quantix::Core::Components::Rigidbody::GetAngularVelocity, &Quantix::Core::Components::Rigidbody::SetAngularVelocity)
	.property("Position", &Quantix::Core::Components::Rigidbody::GetTransformPosition, &Quantix::Core::Components::Rigidbody::SetTransformPosition)
	.property("Rotation", &Quantix::Core::Components::Rigidbody::GetTransformRotation, &Quantix::Core::Components::Rigidbody::SetTransformRotation)
	.property("Actor Flag", &Quantix::Core::Components::Rigidbody::actorFlag)
			(rttr::metadata("Description", "The following information are Actor flags.\n\n"))
	.property("RigidBody Flag", &Quantix::Core::Components::Rigidbody::rigidFlag)
			(rttr::metadata("Description", "The following information are RigidBody flags.\n\n"));

	rttr::registration::class_<Quantix::Physic::RigidBodyFlag>("RigidBody Flag")
		.constructor<>()
		.property("Kinematic", &Quantix::Physic::RigidBodyFlag::GetRigidFlagKinematic, &Quantix::Physic::RigidBodyFlag::SetRigidFlagKinematic)
		.property("CCD", &Quantix::Physic::RigidBodyFlag::GetRigidFlagCCD, &Quantix::Physic::RigidBodyFlag::SetRigidFlagCCD)
		.property("CCDFriction", &Quantix::Physic::RigidBodyFlag::GetRigidFlagCCDFriction, &Quantix::Physic::RigidBodyFlag::SetRigidFlagCCDFriction)
		.property("SpeculativeCCD", &Quantix::Physic::RigidBodyFlag::GetRigidFlagSpeculativeCCD, &Quantix::Physic::RigidBodyFlag::SetRigidFlagSpeculativeCCD)
		.property("RetainAcceleration", &Quantix::Physic::RigidBodyFlag::GetRigidFlagRetainAcceleration, &Quantix::Physic::RigidBodyFlag::SetRigidFlagRetainAcceleration)
		.property("KinematicQueries", &Quantix::Physic::RigidBodyFlag::GetRigidFlagKineForQueries, &Quantix::Physic::RigidBodyFlag::SetRigidFlagKineForQueries)
		.property("PoseIntPreview", &Quantix::Physic::RigidBodyFlag::GetRigidFlagPosePreview, &Quantix::Physic::RigidBodyFlag::SetRigidFlagPosePreview)
		.property("CCDMaxImpulse", &Quantix::Physic::RigidBodyFlag::GetRigidFlagCCDMaxContactImpulse, &Quantix::Physic::RigidBodyFlag::SetRigidFlagCCDMaxContactImpulse);

	rttr::registration::class_<Quantix::Physic::ActorFlag>("Actor Flag")
		.constructor<>()
		.property("DisableGravity", &Quantix::Physic::ActorFlag::GetActorFlagDisableGravity, &Quantix::Physic::ActorFlag::SetActorFlagDisableGravity)
		.property("DisableSimulation", &Quantix::Physic::ActorFlag::GetActorFlagDisableSimulation, &Quantix::Physic::ActorFlag::SetActorFlagDisableSimulation);
}

namespace Quantix::Core::Components
{
	Rigidbody::Rigidbody(DataStructure::GameComponent* par):
		Core::DataStructure::Component(par),
		actorPhysic{ (Physic::PhysicDynamic*)Physic::PhysicHandler::GetInstance()->GetObject(par, true) }
	{
	}

	Rigidbody::Rigidbody(const Rigidbody& src) noexcept :
		Core::DataStructure::Component(src),
		actorPhysic{ src.actorPhysic }
	{
	}

	Rigidbody::Rigidbody(Rigidbody&& by_ref) noexcept :
		Core::DataStructure::Component(by_ref),
		actorPhysic{ std::move(by_ref.actorPhysic) }
	{}

	void Rigidbody::AddForce(Math::QXvec3 vec) noexcept
	{
		actorPhysic->GetObjectDynamic()->GetRigid()->addForce(physx::PxVec3(vec.x, vec.y, vec.z));
	}

	void Rigidbody::AddTorque(Math::QXvec3 vec) noexcept
	{
		actorPhysic->GetObjectDynamic()->GetRigid()->addTorque(physx::PxVec3(vec.x, vec.y, vec.z));
	}

	Rigidbody* Rigidbody::Copy() const
	{
		return new Rigidbody(*this);
	}

	void Rigidbody::Init(DataStructure::GameComponent* par)
	{
		_object = par;
		_isDestroyed = false;
		_isEnable = true;

		actorPhysic = (Physic::PhysicDynamic*)Physic::PhysicHandler::GetInstance()->GetObject(par, true);
		actorFlag.actorPhysic = actorPhysic;
		rigidFlag.actorPhysic = actorPhysic;
	}

	void Rigidbody::Destroy()
	{
		// Actualize the physic actor
		Physic::PhysicHandler::GetInstance()->GetObject(_object, false);
		
		// Actualize the pointer in colliders
		std::vector<ICollider*> vector{ _object->GetComponents<ICollider>() };
		for (QXuint i = 0; i < vector.size(); i++)
			vector[i]->UpdateActorPhysic();
	}

	QXfloat Rigidbody::GetMass()
	{
		return mass;
	}

	void Rigidbody::SetMass(QXfloat m)
	{
		if (m < 0.f)
			m = 0.00001f;
		mass = m;
		physx::PxRigidBodyExt::updateMassAndInertia(*actorPhysic->GetRigid(), m);
	}

	Math::QXvec3 Rigidbody::GetLinearVelocity()
	{
		physx::PxVec3 v = actorPhysic->GetRigid()->getLinearVelocity();
		return 	Math::QXvec3(v.x, v.y, v.z);
	}

	void Rigidbody::SetLinearVelocity(Math::QXvec3 v)
	{
		actorPhysic->GetRigid()->setLinearVelocity(physx::PxVec3(v.x, v.y, v.z));
	}

	Math::QXvec3 Rigidbody::GetAngularVelocity()
	{
		physx::PxVec3 v = actorPhysic->GetRigid()->getAngularVelocity();
		return 	Math::QXvec3(v.x, v.y, v.z);
	}

	void Rigidbody::SetAngularVelocity(Math::QXvec3 v)
	{
		actorPhysic->GetRigid()->setAngularVelocity(physx::PxVec3(v.x, v.y, v.z));
	}

	Math::QXvec3 Rigidbody::GetTransformPosition()
	{
		physx::PxVec3 v = actorPhysic->GetRigid()->getGlobalPose().p;
		return 	Math::QXvec3(v.x, v.y, v.z);
	}

	void Rigidbody::SetTransformPosition(Math::QXvec3 v)
	{
		actorPhysic->GetRigid()->setGlobalPose(physx::PxTransform(
												physx::PxVec3(v.x, v.y, v.z),
												actorPhysic->GetRigid()->getGlobalPose().q));
	}

	Math::QXquaternion Rigidbody::GetTransformRotation()
	{
		physx::PxQuat q = actorPhysic->GetRigid()->getGlobalPose().q;
		return 	Math::QXquaternion(q.w, q.x, q.y, q.z);
	}

	void Rigidbody::SetTransformRotation(Math::QXquaternion q)
	{
		actorPhysic->GetRigid()->setGlobalPose(physx::PxTransform(
												actorPhysic->GetRigid()->getGlobalPose().p, 
												physx::PxQuat(q.v.x, q.v.y, q.v.z, q.w)
												));
	}
}
