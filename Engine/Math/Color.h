#pragma once

enum class Color : int
{
	Red = 4,
	Green = 2,
	Blue = 1,
	White = Red | Green | Blue /* = 7 */,
	Intensity = 8,
};