#pragma once
#include <wx/wx.h>
#include <vector>

/*
    Class to handle events for the canvas such as where
    mouse is moving and if to draw on the canvas
*/

class DrawingCanvas : public wxWindow
{
public:
    DrawingCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    virtual ~DrawingCanvas() noexcept {}

private:
    void OnPaint(wxPaintEvent&);
    void OnMouseDown(wxMouseEvent&);
    void OnMouseMove(wxMouseEvent&);
    void OnMouseUp(wxMouseEvent&);
    void OnMouseLeave(wxMouseEvent&);

    bool isDrawing{};
    std::vector<std::vector<wxPoint2DDouble>> squiggles;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent&);
};