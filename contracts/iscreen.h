#pragma once

class IScreen
{
public:
	virtual void OnEnter()
	{
	}
	
	virtual void Tick(const uint32_t uptime)
	{
	}
};