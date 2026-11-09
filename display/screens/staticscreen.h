#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"

class StaticScreen : public IScreen
{
	public:
	StaticScreen(IDisplay& lcd, const __FlashStringHelper* row0, const __FlashStringHelper* row1) 
	: _lcd(lcd), _row0(row0), _row1(row1)
	{}
	
	void OnEnter() override
	{
		_lcd.clear();
		_lcd.setCursor(0, 0);
		_lcd.print(_row0);
		_lcd.setCursor(0, 1);
		_lcd.print(_row1);
	}
	
	private:
	IDisplay& _lcd;
	const __FlashStringHelper* _row0;
	const __FlashStringHelper* _row1;
};
