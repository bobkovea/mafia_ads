#pragma once

class ExtInt
{
	public:
	static bool IsInterruptEnabled()
	{
		return (EIMSK & (1 << INT0)) == (1 << INT0);
	}
	
	static bool IsInterruptActive()
	{
		return (EIFR & (1 << INTF0)) == (1 << INTF0);
	}
 
	static void DisableInterrupt()
	{
		EIMSK &= ~(1 << INT0);
		EIFR = (1 << INTF0);
	}
	
	static void EnableInterrupt()
	{
		EIFR = (1 << INTF0);
		EIMSK |= (1 << INT0);
	}
	
	static void ConfigInterrupt()
	{
		EICRA |= (1 << ISC01);  // Прерывание по спадающему фронту
    EICRA &= ~(1 << ISC00);
	}
};
