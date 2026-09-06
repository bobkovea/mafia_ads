#pragma once

enum class MafiaRole : uint8_t
{
  Citizen = 0,
  Sheriff = 1,
  Mafia = 2,
  Don = 3
};

class RoleManager
{
	public: 
	void GenerateRole()
	{
    _role = (MafiaRole)random(RolesCount);
	}

  MafiaRole GetRole()
  {
    return _role;
  }
  private:
  static constexpr uint8_t RolesCount = 4;
  MafiaRole _role = (MafiaRole)0;
};
