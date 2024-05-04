#pragma once
#include <wx/wx.h>
#include "MyFrame.h"

class StartWindow : public wxFrame
{
private:
	int red, blue, green;

public:
	StartWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxFrame(nullptr, wxID_ANY, title, pos, size) {

		SetBackgroundColour(wxColour(187, 143, 206));

		//Fonts
		wxFont header_font(50, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD, false);
		wxFont button_font(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD, false);
		
		//EzPaint Header
		wxStaticText* text = new wxStaticText(this,wxID_ANY,"EzPaint", wxDefaultPosition,wxDefaultSize);
		text->SetFont(header_font);

		//Button to Open EzPaint Application
		wxButton* OpenButton = new wxButton(this, wxID_ANY, "New Canvas", wxDefaultPosition, wxSize(150,80));
		
		OpenButton->SetFont(button_font);
		//Font Color
		OpenButton->SetForegroundColour(*wxWHITE);
		OpenButton->SetBackgroundColour(wxColour(150, 150, 150));
		OpenButton->Bind(wxEVT_BUTTON, &StartWindow::OpenEzPaint, this);

		//Button to Open an existing Canvas
		wxButton* loadButton = new wxButton(this, wxID_ANY, "Load Canvas", wxDefaultPosition, wxSize(150,80));
		loadButton->SetFont(button_font);
		loadButton->SetForegroundColour(*wxWHITE);
		loadButton->SetBackgroundColour(wxColour(150, 150, 150));
		loadButton->Bind(wxEVT_BUTTON, &StartWindow::LoadImage, this);

		//Layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->AddSpacer(200);
		sizer->Add(text, 0, wxALIGN_CENTER | wxBOTTOM, 30);
		sizer->Add(OpenButton, 0, wxALIGN_CENTER | wxBOTTOM, 20);
		sizer->Add(loadButton,0, wxALIGN_CENTER);
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



