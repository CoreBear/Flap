#ifndef CONSTS_H
#define CONSTS_H

#include <Windows.h>

namespace Consts
{
	// Floats
	constexpr float GRAVITY_FORCE_F = 9.8F;
	constexpr float NO_VALUE_F = 0.0F;

	// Ints
	constexpr int FPS_TARGET = 60;
	constexpr int NO_VALUE = 0;
	constexpr int NUMBER_OF_INPUTS = 4;
	constexpr int OFF_BY_ONE = 1;

	// Keys
	constexpr unsigned short INPUTS[NUMBER_OF_INPUTS]
	{
		(unsigned short)VK_DOWN,
		(unsigned short)VK_LEFT,
		(unsigned short)VK_RIGHT,
		(unsigned short)VK_UP
	};

	// Seconds to milli (*1000) to micro (*1000)
	constexpr short int ORDER_OF_MAG_CONVERTER = 1000;
	constexpr double FIXED_DELTA_TIME_D = 1.0 / Consts::FPS_TARGET;
	constexpr long long int FIXED_DELTA_TIME_LL = static_cast<long long int>(FIXED_DELTA_TIME_D * ORDER_OF_MAG_CONVERTER * ORDER_OF_MAG_CONVERTER);
	constexpr float GRAVITY_FORCE_CONVERTED_F = static_cast<float>(GRAVITY_FORCE_F * FIXED_DELTA_TIME_D);
}

#endif CONSTS_H