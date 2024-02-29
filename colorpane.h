#pragma once
#include "selectablepane.h"

/*
    ColorPane class is derived from the SelectablePane class that
    lets the user set the color and overrides the draw content method
*/

class ColorPane : public SelectablePane
{
public:
    ColorPane(wxWindow* parent, wxWindowID id, const wxColor& paneColor, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    wxColour color;

private:
    virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};

ColorPane::ColorPane(wxWindow* parent, wxWindowID id, const wxColor& paneColor, const wxPoint& pos, const wxSize& size)
    : SelectablePane(parent, id, pos, size), color(paneColor)
{
}

void ColorPane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
    gc->SetPen(wxPen(color));
    gc->SetBrush(wxBrush(color));

    gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);
}