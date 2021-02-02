#pragma once

#include "Weapon.h"

class Handgun: public Weapon
{
public:
	 ~Handgun() = default;

	 void Load() override;

	 void Unload() override;

	 void Render() override;

	 void Fire(const X::Math::Vector2& position, const X::Math::Vector2& direction) override;

	 bool CanFire() const override;

	 void Reload() override;

	 void ResetLine() override;

};