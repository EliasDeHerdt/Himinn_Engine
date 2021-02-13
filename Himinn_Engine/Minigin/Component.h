#pragma once
class Component
{
public :
	Component() {};
	virtual ~Component() = default;
	Component(const Component& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update(float elapsedSec) = 0;
};

