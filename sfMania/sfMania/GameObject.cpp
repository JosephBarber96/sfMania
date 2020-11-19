#include "GameObject.h"

#include "Settings.h"

std::vector<GameObject*> GameObject::GameObjects;

GameObject::GameObject()
{
	m_active = true;
	GameObject::GameObjects.push_back(this);
}

GameObject::~GameObject()
{
	GameObject::GameObjects.erase(
		std::remove(GameObject::GameObjects.begin(), GameObject::GameObjects.end(), this), 
		GameObject::GameObjects.end());
}

void GameObject::Init()
{
	GameObject::GameObjects = std::vector<GameObject*>();
}

void GameObject::Update()
{

}

void GameObject::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
	OnSetPosition();
}

void GameObject::SetPositionNormalized(float x, float y)
{
	m_x = Settings::WindowX() * x;
	m_y = Settings::WindowY() * y;
	OnSetPosition();
}

void GameObject::OnSetPosition()
{

}

void GameObject::SetActive(bool active)
{
	m_active = active;
}

bool GameObject::IsActive() const
{
	return m_active;
}