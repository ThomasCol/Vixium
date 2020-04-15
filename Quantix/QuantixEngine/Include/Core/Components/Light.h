#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <Vec3.h>
#include <Vec2.h>

#include "Core/DLLHeader.h"
#include "Core/Type.h"

namespace Quantix::Core::Components
{
	enum class ELightType
	{
		DEFAULT,
		DIRECTIONAL,
		POINT,
		SPOT,
		COUNT
	};

	struct alignas(16) QUANTIX_API Light
	{
#pragma region Attributes
		Math::QXvec3		direction; QXfloat _pad1{0};
		Math::QXvec3		position; QXfloat _pad2{ 0 };

		Math::QXvec3   		ambient; QXfloat _pad3{ 0 };
		Math::QXvec3   		diffuse; QXfloat _pad4{ 0 };
		Math::QXvec3   		specular;

		QXfloat				constant{ 0 }; 
		QXfloat				linear{0}; 
		QXfloat				quadratic{0};

		QXfloat				cutOff{0};
		QXfloat				outerCutOff{0};

		ELightType			type;
#pragma endregion

#pragma region Constructors

		/**
		 * @brief Construct a new Light object
		 */
		Light() = default;
		/**
		 * @brief Construct a new Light object
		 * 
		 * @param light Light to copy
		 */
		Light(const Light& light) = default;

		/**
		 * @brief Construct a new Light object
		 * 
		 * @param light Light to move
		 */
		Light(Light&& light) = default;

		/**
		 * @brief Construct a new Light object
		 * 
		 * @param ambientValue Vec3 for ambient value
		 * @param diffuseValue Vec3 for diffuse value
		 * @param specularValue Vec3 for specular value
		 * @param lightType Light type value
		 */
		Light(Math::QXvec3 ambientValue, Math::QXvec3 diffuseValue, Math::QXvec3 specularValue, ELightType lightType) noexcept;

		/**
		 * @brief Destroy the Light object
		 */
		~Light() = default;

#pragma endregion
	};
}


#endif // __LIGHT_H__
