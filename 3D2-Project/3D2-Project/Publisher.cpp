#include "Publisher.h"
std::vector<Subscriber*> Publisher::m_subscribers;

Publisher::Publisher()
{
}


Publisher::~Publisher()
{
	Shutdown();
}

bool Publisher::Initialize()
{
	m_subscribers = std::vector<Subscriber*>();

	return true;
}

void Publisher::Shutdown()
{
	m_subscribers.clear();
}

void Publisher::AddSubscriber(Subscriber* p_subscriber)
{
	m_subscribers.push_back(p_subscriber);
}

bool Publisher::Unsubscribe(Subscriber* p_subscriber)
{
	for (int i = 0; i < m_subscribers.size(); i++)
	{
		if (m_subscribers[i] == p_subscriber)
		{
			m_subscribers.erase(m_subscribers.begin() + i);
			return true;
		}
	}
	return false;
}

void Publisher::Update(bool p_keys[256], float deltaTime)
{
	for (int i = 0; i < m_subscribers.size(); i++)
	{
		m_subscribers[i]->Update(p_keys, deltaTime);
	}
}

void Publisher::UpdateMouse(int p_x, int p_y, float deltaTime)
{
	for (int i = 0; i < m_subscribers.size(); i++)
	{
		m_subscribers[i]->UpdateMouse(p_x, p_y, deltaTime);
	}
}