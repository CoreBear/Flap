#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Consts.h"
#include "Enums.h"

#include <cmath>

namespace Structure
{
	struct Generic
	{

	};
	struct Input
	{
	public:
		Enums::InputPressState m_inputPressState;
		unsigned short m_input;
	};
	template<typename T> struct Vector2
	{
	public:
		// Member Variables
		float m_normalizationResult;
		T m_x;
		T m_y;

		// Initialization
		Vector2() : m_normalizationResult(Consts::NO_VALUE_F), m_x(Consts::NO_VALUE), m_y(Consts::NO_VALUE) { return; }
		Vector2(T _x, T _y) : m_normalizationResult(Consts::NO_VALUE_F), m_x(_x), m_y(_y) { return; }

		// Functionality
		Structure::Vector2<T> NormalizeReturn()
		{
			m_normalizationResult = sqrt((m_x * m_x) + (m_y * m_y));

			return Structure::Vector2<T>(m_x / m_normalizationResult, m_y / m_normalizationResult);
		}
		void operator*=(float _scalar)
		{
			m_x += _scalar;
			m_y += _scalar;
		}
		void operator+=(const Structure::Vector2<float>& _other)
		{
			m_x += _other.m_x;
			m_y += _other.m_y;
		}
		Structure::Vector2<T> operator*(float _scalar)
		{
			return Structure::Vector2<T>(m_x * _scalar, m_y * _scalar);
		}
		Structure::Vector2<T> operator-()
		{
			return Structure::Vector2<T>(-m_x, -m_y);
		}
		float SquareMagnitudeReturn()
		{
			return static_cast<float>((m_x * m_x) + (m_y * m_y));
		}
	};
	struct SpriteInfo
	{
	public:
		const char*** mppp_sprite;
		short m_color;
		int m_animationKeyFrameIndexToRender;
		int m_numberOfAnimationKeyFrames;
		int m_spriteHeight;
		Vector2<int> m_position;
	};
}

#endif STRUCTURE_H