#pragma once
#include "../Globals.h"
#include "../Game/SDK/Classes/Basic.h"
#include "../Game/SDK/Classes/Engine_classes.h"

/*
* @brief A wrapper for drawing functions
*/
class Drawing {
public:
	/*
	* @brief Draws a line on the screen
	*
	* @param ScreenPositionA - The starting position of the line
	* @param ScreenPositionB - The ending position of the line
	* @param Thickness - The thickness of the line
	* @param RenderColor - The color of the line
	* @param Outlined - Whether or not the line should be outlined
	*/
	static void Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws text on the screen
	* 
	* @param RenderText - The text to draw
	* @param ScreenPosition - The position of the text
	* @param FontSize - The size of the text
	* @param RenderColor - The color of the text
	* @param CenteredX - Whether or not the text should be centered on the X axis
	* @param CenteredY - Whether or not the text should be centered on the Y axis
	* @param Outlined - Whether or not the text should be outlined
	*/
	static void Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined);
	
	/*
	* @brief Gets the estimated size of text drawn on the screen
	* 
	* @param RenderText - The text to get the size of
	* @param FontSize - The size of the text
	*/
	static SDK::FVector2D TextSize(const wchar_t* RenderText, float FontSize);

	/*
	* @brief Draws a circle on the screen
	* 
	* @param ScreenPosition - The position of the circle
	* @param Radius - The radius of the circle
	* @param Segments - The amount of segments the circle should have
	* @param RenderColor - The color of the circle
	*/
	static void Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a filled rectangle on the screen
	* 
	* @param ScreenPositionA - The starting position of the rectangle
	* @param ScreenPositionB - The ending position of the rectangle
	* @param RenderColor - The color of the rectangle
	*/
	static void FilledRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a hollow rectangle on the screen
	* 
	* @param ScreenPositionA - The starting position of the rectangle
	* @param ScreenPositionB - The ending position of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	* @param Outlined - Whether or not the rectangle should be outlined
	*/
	static void Rect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a cornered rectangle on the screen
	* 
	* @param ScreenPositionA - The starting position of the rectangle
	* @param ScreenPositionB - The ending position of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	*/
	static void CorneredRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);
};