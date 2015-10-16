#pragma once

#include "Subscriber.h"
#include <vector>

class Publisher
{
public:
	Publisher();
	~Publisher();

	bool Initialize();
	void Shutdown();

	static void AddSubscriber(Subscriber* p_subscriber);
	static bool Unsubscribe(Subscriber* p_subscriber);

	void Update(bool p_keys[256], float deltaTime);
	void UpdateMouse(int p_x, int p_y, float deltaTime);
private:
	static std::vector<Subscriber*> m_subscribers;
};

