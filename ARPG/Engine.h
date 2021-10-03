#pragma once
#include "AttackAware.h"


class Engine : public AttackAware
{
public:
	void start(int screenWidth, int screenHeight);
	void attack(int x, int y);
};

