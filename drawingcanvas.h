#pragma once
#include <wx/wx.h>
#include <vector>
#include "path.h"

/*
    Class to handle events for the canvas such as where
    mouse is moving and if to draw on the canvas, as well
    as setting the default color and size of the pen
*/

class DrawingCanvas : public wxWindow
{
public:
    DrawingCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    virtual ~DrawingCanvas() noexcept {}

    int currentWidth{ 1 };
    wxColour currentColor{ *wxBLACK };

private:
    void OnPaint(wxPaintEvent&);
    void OnMouseDown(wxMouseEvent&);
    void OnMouseMove(wxMouseEvent&);
    void OnMouseUp(wxMouseEvent&);
    void OnMouseLeave(wxMouseEvent&);

    bool isDrawing{};
    std::vector<Path> squiggles;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent&);
};