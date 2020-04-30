#include "Core\DataStructure\GameComponent.h"

namespace Quantix::Core::DataStructure
{
	GameComponent::GameComponent(const GameComponent& object) noexcept :
		_name{ object._name },
		_layer{ object._layer },
		_isStatic{ object._isStatic },
		_isActive{ object._isActive },
		_toRender{ false }
	{
		for (size_t i = 0; i < object._component.size(); i++)
		{
			_component.push_back(object._component[i]);
		}
	}

	GameComponent::GameComponent(GameComponent&& object) noexcept :
		_component{ std::move(object._component) },
		_name{ std::move(object._name) },
		_layer{ std::move(object._layer) },
		_isStatic{ std::move(object._isStatic) },
		_isActive{ std::move(object._isActive) },
		_toRender{ std::move(object._toRender) }
	{
	}

	GameComponent::GameComponent(const QXstring& name, const QXbool& isStatic) noexcept :
		_name{ name },
		_layer{ 0 },
		_isStatic{ isStatic },
		_isActive{ false },
		_toRender { false }
	{
	}

	GameComponent::~GameComponent()
	{
		for (size_t i = 0; i < _component.size(); i++)
		{
			if (_component[i] != nullptr)
			{
				//_component[i]->Destroy();
				delete _component[i];
			}
		}
	}

	void GameComponent::CleanDestroyedComponents()
	{
		for (unsigned i{ 0 }; i < _component.size(); ++i)
		{
			if (_component[i]->IsDestroyed())
			{
				//_component[i]->Destroy();
				delete _component[i];
				_component.erase(_component.begin() + i--);
			}
		}
	}

	GameComponent& GameComponent::operator=(const GameComponent& gc)
	{
		_component = gc._component;
		_name = gc._name;
		_layer = gc._layer;
		_isStatic = gc._isStatic;
		_isActive = gc._isActive;
		_toRender = gc._toRender;

		return *this;
	}

	void GameComponent::Start(std::vector<Core::Components::Mesh*>& meshes)
	{
		for (QXint i = 0; i < _behaviours.size(); i++)
			_behaviours[i]->Start();
	}

	void GameComponent::Update(std::vector<Core::Components::Mesh*>& meshes, std::vector<Core::Components::ICollider*>& colliders)
	{
		for (QXint i = 0; i < _behaviours.size(); i++)
			_behaviours[i]->Update();
	}
}
