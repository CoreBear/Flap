#pragma region Includes
#include "Tools.h"

#include "SharedRender.h"
#pragma endregion

#pragma region Static Initialization
int Tools::s_reusableIterator_1;
int Tools::s_reusableIterator_2;
SharedRender* Tools::sp_sharedRender = nullptr;
#pragma endregion

#pragma region Functionality
int Tools::CenterText_ReturnStartColumn(int _textLineLength)
{
	return (sp_sharedRender->m_bufferHW.X / 2) - _textLineLength / 2;
}
char Tools::IntToChar(int _number)
{
	if (_number < Consts::NO_VALUE)
	{
		_number = -_number;
	}

	return static_cast<char>(_number + CHAR_OFFSETTER);
}
const char* Tools::IntToString(int _number)
{
	bool isNegative = _number < Consts::NO_VALUE;
	char* newString;
	int multiUse = Consts::NO_VALUE;	// NOTE: Stores number of digits
	int number = _number;
	constexpr int ONES_DIGIT_VIEWER = 10, SINGLE_DIGIT_MAX = 9;

	// Gets number of digits within number
	while (number != Consts::NO_VALUE)
	{
		number /= ONES_DIGIT_VIEWER;

		++multiUse;
	}

	// If a negative number vs >= 0
	// NOTE: Notice the incrementation
	newString = (isNegative) ? new char[++multiUse + Consts::OFF_BY_ONE] : newString = new char[multiUse + Consts::OFF_BY_ONE];
	newString[multiUse--] = '\0';

	// NOTE: multiUse is now representing index that's being written into

	number = _number;

	// Creates a string out of the number
	while (number != Consts::NO_VALUE)
	{
		// NOTE: Notice the incrementation
		newString[multiUse--] = IntToChar(number % ONES_DIGIT_VIEWER);
		number /= ONES_DIGIT_VIEWER;
	}

	// If a negative number
	if (isNegative)
	{
		newString[multiUse] = '-';
	}

	return newString;
}
int Tools::StringToInt(const char* _string)
{
	bool isNegative;
	const char* walker = _string;
	int numberOfDigits = Consts::NO_VALUE;

	if (*walker == '-')
	{
		isNegative = true;
		++walker;
	}
	else
	{
		isNegative = false;
	}

	while (*walker != '\0')
	{
		++numberOfDigits;
		++walker;
	}

	--walker;

	int newDigitInPlace;
	int newNumber = 0;
	for (s_reusableIterator_1 = Consts::NO_VALUE; s_reusableIterator_1 < numberOfDigits; s_reusableIterator_1++)
	{
		newDigitInPlace = CharToInt(*walker);

		for (s_reusableIterator_2 = Consts::NO_VALUE; s_reusableIterator_2 < s_reusableIterator_1; s_reusableIterator_2++)
		{
			constexpr int PLACE_CONVERTER = 10;
			newDigitInPlace *= PLACE_CONVERTER;
		}

		newNumber += newDigitInPlace;
		--walker;
	}

	return (isNegative) ? -newNumber : newNumber;
}
#pragma endregion