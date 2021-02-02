#pragma once

#include "Weapon.h"

class Rifle: public Weapon
{
public:
	 ~Rifle() = default;

	 void Load() override;

	 void Unload() override;

	 void Render() override;

	 void Fire(const X::Math::Vector2& position, const X::Math::Vector2& direction) override;
	 void ResetLine() override;
	 bool CanFire() const override;
	 void Reload() override;
};