#pragma once

#include <wx/wx.h>
#include <vector>


// this struct represents a single "squiggle"
struct Path
{
	std::vector<wxPoint2DDouble> points;
	wxColour color;
	int width;
};
