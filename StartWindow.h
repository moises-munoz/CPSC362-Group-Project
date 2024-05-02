#pragma once
#include <wx/wx.h>
#include "MyFrame.h"

class StartWindow : public wxFrame
{
public:
	StartWindow(const wxString& title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize) 
		: wxFrame(nullptr, wxID_ANY, title, pos, size) {
		//Button to Open EzPaint Application
		wxButton* OpenButton = new wxButton(this, wxID_ANY, "New Canvas");
		OpenButton->Bind(wxEVT_BUTTON, &StartWindow::OpenEzPaint, this);

		//Button to Open an existing Canvas
		wxButton* loadButton = new wxButton(this, wxID_ANY, "Load Canvas");
		loadButton->Bind(wxEVT_BUTTON, &StartWindow::LoadImage, this);

		//Layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(OpenButton, 0, wxALIGN_CENTER | wxALL, 20);
		SetSizerAndFit(sizer);
	}

	//Function that Open the EzPaint Application
	void OpenEzPaint(wxCommandEvent& event) {
		//Close the Start Window
		Close();

		//Open the Start Window
		MyFrame* frame = new MyFrame("EzPaint", wxDefaultPosition, wxDefaultSize);
		frame->Show(true);
	}

	//Function that loads an existing drawing onto the EzPaint Canvas
	void LoadImage(wxCommandEvent& event) {
		Close();
		MyFrame* frame = new MyFrame("EzPaint", wxDefaultPosition, wxDefaultSize);
		frame->OpenXML(event);
		frame->Show(true);
		
	}
};



