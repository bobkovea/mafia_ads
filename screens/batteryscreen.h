#pragma once

#include "../contracts/idisplay.h"
#include "../contracts/iscreen.h"
#include "../contracts/ibatterymanager.h"

class BatteryScreen : public IScreen
{
public:
	BatteryScreen(IDisplay& lcd, IBatteryManager& batteryManager) :
	_lcd(lcd), _batteryManager(batteryManager)
	{
	}
	
	void OnEnter() override
	{
		_lcd.noCursor();
		_lcd.noBlink();
		_lcd.clear();
		const float vBat = _batteryManager.GetVbat(); // todo сделать вывод в процентах
		static constexpr float MinVoltage = 3.0F; // todo 3.0
		static constexpr float MaxVoltage = 4.2F; // todo 4.2
		float percent = (vBat - MinVoltage) / (MaxVoltage - MinVoltage) * 100.0F;
		percent = constrain(percent, 0.0, 100.0);
		_lcd.print("Уровень заряда:");
		_lcd.setCursor(0, 1);
		_lcd.print((uint8_t)percent);
		_lcd.print("%");
	}

private:
	IDisplay& _lcd;
	IBatteryManager& _batteryManager;
};
