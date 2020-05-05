#include "Physic/PhysicDynamic.h"

#include <iostream>
#include "Physic/PhysicStatic.h"
#include <PxRigidActorExt.h>

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<Quantix::Physic::PhysicDynamic>("PhysicDynamic")
	.constructor<>()
	.constructor<physx::PxPhysics*>()
	.constructor<physx::PxPhysics*, Quantix::Physic::PhysicStatic*>()
	.constructor<const Quantix::Physic::PhysicDynamic&>()
	.constructor<Quantix::Physic::PhysicDynamic&&>()
	.enumeration<Quantix::Physic::EPhysXShape>("EPhysXShape")
					 (rttr::value("Cube", Quantix::Physic::EPhysXShape::CUBE),
					 rttr::value("Sphere", Quantix::Physic::EPhysXShape::SPHERE),
					 rttr::value("Capsule", Quantix::Physic::EPhysXShape::CAPSULE))
	.property("dynamic", &Quantix::Physic::PhysicDynamic::GetRigid, &Quantix::Physic::PhysicDynamic::SetRigid);
}

namespace Quantix::Physic
{
	using namespace physx;
	PhysicDynamic::PhysicDynamic() noexcept : IPhysicType(ETypePhysic::DYNAMIC)
	{
	}

	PhysicDynamic::PhysicDynamic(PxPhysics* SDK) noexcept : IPhysicType(ETypePhysic::DYNAMIC)
	{
		// Create ActorPhysic Dynamic
		_dynamic = SDK->createRigidDynamic(PxTransform(PxVec3(0, 2, 0)));
	}

	PhysicDynamic::PhysicDynamic(PxPhysics* SDK, PhysicStatic* physicStatic) noexcept
	{
		// Set Type
		type = ETypePhysic::DYNAMIC;

		// Get All Shapes of physicStatic
		int numShapes = physicStatic->GetRigid()->getNbShapes();
		PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*) * numShapes);
		physicStatic->GetRigid()->getShapes(shapes, numShapes);

		// Prendre les valeurs du static
		_dynamic = SDK->createRigidDynamic(PxTransform(physicStatic->GetRigid()->getGlobalPose()));

		// Attach shape On the new actor and detach of the old actor
		physx::PxShape* currentShape = nullptr;
		for (int i = 0; i < numShapes; i++)
		{
			currentShape = shapes[i];
			physicStatic->GetRigid()->detachShape(*currentShape);
			_dynamic->attachShape(*currentShape);
		}

		free(shapes);

		// Set the userdata
		Core::DataStructure::GameObject3D* data = (Core::DataStructure::GameObject3D*)physicStatic->GetRigid()->userData;
		_dynamic->userData = data;
	}

	PhysicDynamic::PhysicDynamic(const PhysicDynamic& pd) noexcept : IPhysicType(pd)
	{
		_dynamic = pd._dynamic;
	}

	PhysicDynamic::PhysicDynamic(PhysicDynamic&& pd) noexcept : IPhysicType(pd)
	{
		type = std::move(pd.type);
		_dynamic = std::move(pd._dynamic);
	}

	PhysicDynamic::~PhysicDynamic()
	{
	}

	PxRigidDynamic* PhysicDynamic::GetRigid()
	{
		return _dynamic;
	}

	void PhysicDynamic::SetRigid(PxRigidDynamic* rigid)
	{
		_dynamic = rigid;
	}
}