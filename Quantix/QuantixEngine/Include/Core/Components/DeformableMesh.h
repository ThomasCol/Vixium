#ifndef __DEFORMABLEMESH_H__
#define __DEFORMABLEMESH_H__

#include "rttrEnabled.h"
#include "Core/MathHeader.h"
#include "Core./DataStructure//Component.h"
#include "Resources/Scene.h"

namespace Quantix::Core::Components
{
	struct QUANTIX_API DeformableMesh : public Core::DataStructure::Component
	{
		Math::QXvec3 cubeSize;

		QXuint numCubeInWidth = 1;
		QXuint numCubeInHeight = 1;
		QXuint numCubeInDepth = 1;

		DeformableMesh() = default;
		//DeformableMesh(DataStructure::GameComponent* par);
		DeformableMesh(const DeformableMesh& src) = default;
		DeformableMesh(DeformableMesh&& src) = default;
		~DeformableMesh() = default;

		DeformableMesh& operator=(const DeformableMesh& other) noexcept = default;
		DeformableMesh& operator=(DeformableMesh&& other) noexcept = default;

		DeformableMesh* Copy() const override;

		void Init(DataStructure::GameComponent* par) override;
		void Generate(Resources::Scene* scene);
		void AddComponent(DataStructure::GameObject3D* object);

		QXuint GetNumWidth() { std::cout << numCubeInWidth << std::endl; return numCubeInWidth; }
		void SetNumWidth(QXuint i) 
		{ 
			std::cout << numCubeInWidth << std::endl; numCubeInWidth = i; 
		}

		CLASS_REGISTRATION(Core::DataStructure::Component);
	};
}

#endif
