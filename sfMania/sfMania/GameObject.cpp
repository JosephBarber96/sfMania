#include "GameObject.h"

std::vector<GameObject*> GameObject::GameObjects;

GameObject::GameObject()
{
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

void GameObject::Start()
{

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

void GameObject::OnSetPosition()
{

}