#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

/*
    The Selectable Pane class handles elements such as creating creating the pane window
    and drawing the outline of the pen
*/

class SelectablePane : public wxWindow
{
public:
	SelectablePane(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);

	wxSize DoGetBestSize() const override {
		return FromDIP(wxSize(45, 45));
	}

	bool selected = false;

protected:
    // handles the actual drawing of the pen by the user
	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const = 0;

private:
	void OnPaint(wxPaintEvent& event);
};

/*
    Constructor that sets the background style and binds the paint event
*/

SelectablePane::SelectablePane(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &SelectablePane::OnPaint, this);
}

/*
    OnPaint creates the graphics context and sets up parameters for the 
    selection of colors available to the user
*/

void SelectablePane::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
    dc.Clear();

    auto gc = wxGraphicsContext::Create(dc);
    if (gc)
    {
        // draws the rounded ractangles
        wxRect selectionRect{ 0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight() };
        selectionRect.Deflate(FromDIP(1));

        wxRect contentRect = selectionRect;
        contentRect.Deflate(FromDIP(2));

        const auto roundness = FromDIP(4);

        // draws colored rectangle
        DrawContent(gc, contentRect, roundness);

        // statement to check for and highlight the users selection
        if (selected)
        {
            gc->SetPen(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE_PEN : *wxBLACK_PEN);
            gc->SetBrush(*wxTRANSPARENT_BRUSH);

            gc->DrawRoundedRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), 
                selectionRect.GetHeight(), roundness);
        }

        delete gc;
    }
}