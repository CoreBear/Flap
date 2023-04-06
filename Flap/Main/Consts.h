#ifndef CONSTS_H
#define CONSTS_H

#include <Windows.h>

namespace Consts
{
	// Chars
	constexpr char EMPTY_SPACE_CHAR = ' ';

	// Floats
	constexpr float MULTIPLICATIVE_HALF_F = 0.5F;
	constexpr float NO_VALUE_F = 0.0F;

	// Ints
	constexpr int ASCII_OFFSET = static_cast<int>('0');
	constexpr int FPS_TARGET = 60;
	constexpr int MAX_NUMBER_OF_PLAYERS = 2;
	constexpr int NO_VALUE = 0;
	constexpr int NUMBER_OF_INPUTS = 6;						// Should mach Enums::InputNames::NumberOfNames...
	constexpr int OFF_BY_ONE = 1;

	// Shorts
	constexpr short WHITE_BACKGROUND = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	constexpr short WHITE_FOREGROUND = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	constexpr unsigned short INPUTS[MAX_NUMBER_OF_PLAYERS][NUMBER_OF_INPUTS]
	{
		// Player 1 Input
		{
			(unsigned short)0x0D,	//Accept | Enter
			(unsigned short)0x28,	//Down	 | Arrow
			(unsigned short)0x25,	//Left	 | Arrow
			(unsigned short)0x08,	//Pause  | Backspace
			(unsigned short)0x27,	//Right	 | Arrow
			(unsigned short)0x26,	//Up	 | Arrow
		},

		// Player 2 Input
		{
			(unsigned short)0x20,	//Accept | Spacebar
			(unsigned short)0x53,	//Down	 | S
			(unsigned short)0x41,	//Left	 | A
			(unsigned short)0x1B,	//Pause  | Escape
			(unsigned short)0x44,	//Right	 | D
			(unsigned short)0x57	//Up	 | W
		}
	};

	// Time
	// Seconds to milli (*1000) to micro (*1000)
	constexpr short int ORDER_OF_MAG_CONVERTER = 1000;
	constexpr float FIXED_DELTA_TIME_F = static_cast<float>(1.0F / Consts::FPS_TARGET);
	constexpr long long int FIXED_DELTA_TIME_LL = static_cast<long long int>(static_cast<double>(FIXED_DELTA_TIME_F) * ORDER_OF_MAG_CONVERTER * ORDER_OF_MAG_CONVERTER);
}

#endif CONSTS_H