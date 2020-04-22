#ifndef _GAMEOBJECT3D_H_
#define _GAMEOBJECT3D_H_

#include "Core/DataStructure/GameComponent.h"
#include "Physic/Transform3D.h"

namespace Quantix::Core::DataStructure
{
	/**
	 * @brief class GameObject3D
	 * 
	 */
	class QUANTIX_API GameObject3D : public GameComponent
	{
	protected:
		#pragma region Attributes
		std::shared_ptr<Quantix::Physic::Transform3D>		_transform;
		#pragma endregion Attributes
	public:
		#pragma region Constructors/Destructor
		GameObject3D() = default;
		GameObject3D(const QXstring& name, const Math::QXvec3& pos = Math::QXvec3( 0, 0, 0 ), const Math::QXvec3& rot = Math::QXvec3(0, 0, 0), const Math::QXvec3& scale = Math::QXvec3(1, 1, 1)) noexcept;
		GameObject3D(const GameObject3D& g3d) noexcept;
		GameObject3D(GameObject3D&& g3d) noexcept;
		~GameObject3D();
		#pragma endregion Constructors/Destructor

		#pragma region Methods

		inline void								AddChild(const GameObject3D* object) { _transform->AddChild(object->GetTransform()); }

		void									Update(std::vector<Core::Components::Mesh*>& meshes);

		void									Update(std::vector<Core::Components::Mesh*>& meshes, const GameObject3D* parentObject);

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(_component);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(_component);
		}

		#pragma region Accessors
		/**
		 * @brief Set the Global Position object
		 *
		 * @param pos QXvec3
		 */
		void									SetGlobalPosition(Math::QXvec3 pos);

		/**
		 * @brief Set the Global Rotation object
		 *
		 * @param rot QXvec3
		 */
		void									SetGlobalRotation(Math::QXvec3 rot);

		/**
		* @brief Get the Global Position object
		*
		*/
		//Math::QXvec3							GetGlobalPosition() const { return _transform.GetGlobalPosition(); };

		/**
		* @brief Get the Global Rotation object
		*
		*/
		//Math::QXvec3							GetGlobalRotation() const { return _transform.GetGlobalRotation(); };

		/**
		* @brief Set the Local Position object
		*
		* @param pos QXvec3
		*/
		void									SetLocalPosition(Math::QXvec3 pos);

		/**
		 * @brief Set the Local Rotation object
		 *
		 * @param rot QXvec3
		 */
		void									SetLocalRotation(Math::QXvec3 rot);

		/**
		 * @brief Set the Local Scale object
		 *
		 * @param scale QXvec3
		 */
		void									SetLocalScale(Math::QXvec3 scale);

		/**
		* @brief Get the Local Position object
		*
		*/
		const Math::QXvec3&						GetLocalPosition() { return _transform->GetPosition(); };

		/**
		* @brief Get the Local Rotation object
		*
		*/
		const Math::QXvec3&						GetLocalRotation() { return _transform->GetRotation();
		};

		/**
		* @brief Get the Local Scale object
		*
		*/
		const Math::QXvec3&						GetLocalScale() { return _transform->GetScale(); };

		/**
		 * @brief Set the Transform object
		 *
		 * @param pos QXvec3
		 * @param rot QXvec3
		 * @param scale QXvec3
		 */
		void									SetTransformValue(const Math::QXvec3& pos, const Math::QXvec3& rot, const Math::QXvec3& scale);

		inline void								SetTransform(std::shared_ptr<Quantix::Physic::Transform3D> transform) { _transform = transform; };

		/**
		 * @brief Get the Transform object
		 *
		 */
		std::shared_ptr<Quantix::Physic::Transform3D>			GetTransform() const { return _transform; };
		#pragma endregion Accessors
		GameObject3D&											operator=(const GameObject3D& object);
		#pragma endregion Methods

		CLASS_REGISTRATION(GameComponent);
	};
}

namespace cereal
{
	template <class Archive>
	struct specialize<Archive, Quantix::Core::DataStructure::GameObject3D, cereal::specialization::member_load_save> {};
}

CEREAL_REGISTER_TYPE(Quantix::Core::DataStructure::GameObject3D);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Quantix::Core::DataStructure::GameComponent, Quantix::Core::DataStructure::GameObject3D)

#endif // !_GAMEOBJECT3D_H_
