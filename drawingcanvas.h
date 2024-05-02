#pragma once
#include <wx/wx.h>
#include <vector>
#include "xmlserializer.h"
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

    void ShowSaveDialog();

    XmlSerializer serializer{};

    void SaveToXml();
    void LoadFromXml();

    int currentWidth{ 1 };
    wxColour currentColor{ *wxBLACK };

    const std::vector<Path>& GetSquiggles() const { return squiggles; }

    void SetSquiggles(const std::vector<Path>&& squiggles)
    {
        this->squiggles = move(squiggles);
        Refresh();
    }

    void ClearSquigs() {
        std::vector<Path> squigglesCopy = squiggles;
        squigglesCopy.clear();
        this->squiggles = move(squigglesCopy);
        this->Refresh();
    }

private:
    void OnPaint(wxPaintEvent&);
    void DrawOnContext(wxGraphicsContext *gc);
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
