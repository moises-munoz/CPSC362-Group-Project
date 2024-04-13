#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "drawingcanvas.h"


/*
    Binds the mouse events to be detected on the canvas
*/

DrawingCanvas::DrawingCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxWindow(parent, id, pos, size)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);

    BuildContextMenu();

    this->Bind(wxEVT_CONTEXT_MENU, &DrawingCanvas::OnContextMenuEvent, this);
}

/* 
    Sets the background of the canvas to white
    and calls on DrawOnContext to create "lines"
*/
void DrawingCanvas::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        DrawOnContext(gc);

        delete gc;
    }
}

/*
    Connects the points where mouse clicks are registered to
    simulate a line being drawn
*/
void DrawingCanvas::DrawOnContext(wxGraphicsContext *gc)
{
    for (const auto& s : squiggles)
    {
        auto pointsVector = s.points;
        if (pointsVector.size() > 1)
        {
            gc->SetPen(wxPen(
                s.color,
                s.width));
            gc->StrokeLines(pointsVector.size(), pointsVector.data());
        }
    }
}

/*
    Function that allows the user to save their drawing
    from the canvas as a .png file
*/

void DrawingCanvas::ShowSaveDialog()
{
    wxFileDialog saveFileDialog(this, _("Save drawing"), "", "",
                                "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxBitmap bitmap(this->GetSize() * this->GetContentScaleFactor());

    wxMemoryDC memDC;

    memDC.SetUserScale(this->GetContentScaleFactor(), this->GetContentScaleFactor());

    memDC.SelectObject(bitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);

    if (gc)
    {
        DrawOnContext(gc);
        delete gc;
    }

    bitmap.SaveFile(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
}


//function to save our painting as an xml file

void DrawingCanvas::SaveToXml()
{
    wxFileDialog xmlFileDialog(this, "Save as xml", "", "",
        "PX files (*.px)|*.px", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (xmlFileDialog.ShowModal() == wxID_CANCEL)
        return;
    auto doc = serializer.SerializePaths(GetSquiggles());
    doc.Save(xmlFileDialog.GetPath());
}

//function to load our painting as an xml file

void DrawingCanvas::LoadFromXml()
{
    wxFileDialog loadFileDialog(this, "Load xml", "", "",
        "PX files (*.px)|*.px", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (loadFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxXmlDocument doc;

    if (!doc.Load(loadFileDialog.GetPath()))
    {
        wxMessageBox("Failed to load file");
        return;
    }

    SetSquiggles(serializer.DeserializePaths(doc));
}


void DrawingCanvas::OnMouseDown(wxMouseEvent &)
{
    squiggles.push_back({{}, currentColor, currentWidth });
    isDrawing = true;
}

/*
    Method that adds point when the left mouse button is clicked
*/

void DrawingCanvas::OnMouseMove(wxMouseEvent& event)
{
    if (isDrawing)
    {
        auto pt = event.GetPosition();
        auto& currentSquiggle = squiggles.back();

        currentSquiggle.points.push_back(pt);
        Refresh();
    }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent&)
{
    isDrawing = false;
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent&)
{
    isDrawing = false;
}

/*
    Function that allows the user to clear the
    canvas or save the image on the canvas by
    right-clicking on the canvas
*/

void DrawingCanvas::BuildContextMenu()
{
    auto clear = contextMenu.Append(wxID_ANY, "&Clear");
    auto save = contextMenu.Append(wxID_ANY, "Save &As...");

    this->Bind(
        wxEVT_MENU,
        [this](wxCommandEvent&)
        {
            this->squiggles.clear();
            this->Refresh();
        },
        clear->GetId());

    this->Bind(
        wxEVT_MENU,
        [this](wxCommandEvent&)
        {
            this->ShowSaveDialog();
        },
        save->GetId());
}

void DrawingCanvas::OnContextMenuEvent(wxContextMenuEvent& e)
{
    auto clientPos = e.GetPosition() == wxDefaultPosition
        ? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
        : this->ScreenToClient(e.GetPosition());
    PopupMenu(&this->contextMenu, clientPos);
}