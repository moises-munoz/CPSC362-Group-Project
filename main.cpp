#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>
#include <string>
#include <vector>
#include "colorpane.h"
#include "pensizepane.h"
#include "drawingcanvas.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

/*
	The MyFrame class represents our main window
*/

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
	wxPanel* BuildControlsPanel(wxWindow* parent);

	void SetUpColorPanes(wxWindow* parent, wxSizer* sizer);
	void SetUpPenPanes(wxWindow* parent, wxSizer* sizer);

	std::vector<ColorPane *> colorPanes{};
	std::vector<PenSizePane*> penPanes{};

	DrawingCanvas* canvas;

	// vector that contains the list of available colors
	const std::vector<std::string> niceColors = { "#000000", "#ffffff", "#fd7f6f",
												"#7eb0d5", "#b2e061", "#bd7ebe",
												"#ffb55a", "#ffee65", "#beb9db"};

	const int penCount = 6;

	const std::string lightBackground = "#f4f3f3";
	const std::string darkBackground = "#2c2828";
};

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame("EzPaint", wxDefaultPosition, wxDefaultSize);
	frame->Show(true);
	return true;
}

/*
	Iterate over all of the available colors and add them to 
	the sizer and colorPanes vector
*/

void MyFrame::SetUpColorPanes(wxWindow* parent, wxSizer* sizer)
{
	for (const auto& color : niceColors)
	{
		auto colorPane = new ColorPane(parent, wxID_ANY, wxColour(color));

		colorPanes.push_back(colorPane);
		sizer->Add(colorPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
	}
}

/*
	Function to create a series of pens in increasing size
*/

void MyFrame::SetUpPenPanes(wxWindow* parent, wxSizer* sizer)
{
	for (int i = 0; i < penCount; ++i)
	{
		auto penPane = new PenSizePane(parent, wxID_ANY, i * FromDIP(4) + FromDIP(1));

		penPanes.push_back(penPane);
		sizer->Add(penPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
	}
}

/* 
	Builds panels to contain the color pallete, pen sizes and button to save image
	as well as display supplementary information to the user. Will also wrap the
	display information on the left to not be hidden should the window be shortened
*/

wxPanel* MyFrame::BuildControlsPanel(wxWindow* parent)
{
	auto controlsPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
	controlsPanel->SetScrollRate(0, FromDIP(10));

	bool isDark = wxSystemSettings::GetAppearance().IsDark();
	controlsPanel->SetBackgroundColour(wxColor(isDark ? darkBackground : lightBackground));

	auto mainSizer = new wxBoxSizer(wxVERTICAL);

	auto text = new wxStaticText(controlsPanel, wxID_ANY, "Colors");
	mainSizer->Add(text, 0, wxALL, FromDIP(5));

	auto colorPanelSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpColorPanes(controlsPanel, colorPanelSizer);

	mainSizer->Add(colorPanelSizer, 0, wxALL, FromDIP(5));

	text = new wxStaticText(controlsPanel, wxID_ANY, "Pens");
	mainSizer->Add(text, 0, wxALL, FromDIP(5));

	auto penPaneSizer = new wxWrapSizer(wxHORIZONTAL);
	SetUpPenPanes(controlsPanel, penPaneSizer);
	mainSizer->Add(penPaneSizer, 0, wxALL, FromDIP(5));

	auto button = new wxButton(controlsPanel, wxID_ANY, "Save As...");

	mainSizer->AddStretchSpacer();
	mainSizer->Add(button, 0, wxALL, FromDIP(5));
	mainSizer->AddSpacer(FromDIP(5));

	controlsPanel->SetSizer(mainSizer);

	return controlsPanel;
}

/* 
	Constructor for the main window that splits the control panel
	and the drawable canvas allowing the user to change the proportion
	of the left and right side of the window
*/
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);

	splitter->SetMinimumPaneSize(FromDIP(150));

	auto controlsPanel = BuildControlsPanel(splitter);
	canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	splitter->SplitVertically(controlsPanel, canvas);
	splitter->SetSashPosition(FromDIP(220));

	this->SetSize(FromDIP(800), FromDIP(500));
	this->SetMinSize({FromDIP(400), FromDIP(200)});
}