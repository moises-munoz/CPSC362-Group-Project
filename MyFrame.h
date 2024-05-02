#pragma once
#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>
#include <string>
#include <vector>
#include "path.h"
#include "colorpane.h"
#include "pensizepane.h"
#include "drawingcanvas.h"

/*
	The MyFrame class represents our main window
*/
class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);


	//Restarts the Application 
	void OnNew(wxCommandEvent& event) {
		wxExecute(wxTheApp->argv[0]);
		Close(true);
	}
	//Saves the drawing as PNG
	void SaveAsPNG(wxCommandEvent& event) {
		canvas->ShowSaveDialog();
	}
	//Saves the drawing as XML
	void SaveAsXML(wxCommandEvent& event) {
		canvas->SaveToXml();
	}
	//Loads the XML File
	void OpenXML(wxCommandEvent& event) {
		canvas->LoadFromXml();
	}
	// Handles the Quit Event which closes the Application
	void OnExit(wxCommandEvent& event) {
		Close(true);
	}
	void OnClear(wxCommandEvent& event) {
		canvas->ClearSquigs();
	}

private:
	wxPanel* BuildControlsPanel(wxWindow* parent);

	void SetUpColorPanes(wxWindow* parent, wxSizer* sizer);
	void SetUpPenPanes(wxWindow* parent, wxSizer* sizer);

	void SelectColorPane(ColorPane* pane);
	void SelectPenPane(PenSizePane* pane);

	void SetupMainMenu();

	std::vector<ColorPane*> colorPanes{};
	std::vector<PenSizePane*> penPanes{};

	DrawingCanvas* canvas;

	// vector that contains the list of available colors
	const std::vector<std::string> niceColors = { "#000000", "#ffffff", "#fd7f6f",
												"#7eb0d5", "#b2e061", "#bd7ebe",
												"#ffb55a", "#ffee65", "#beb9db" };

	const int penCount = 6;

	const std::string lightBackground = "#f4f3f3";
	const std::string darkBackground = "#2c2828";
};
/*
	Iterate over all of the available colors and add them to
	the sizer and colorPanes vector
*/

void MyFrame::SetUpColorPanes(wxWindow* parent, wxSizer* sizer)
{
	for (const auto& color : niceColors)
	{
		auto colorPane = new ColorPane(parent, wxID_ANY, wxColour(color));

		colorPane->Bind(wxEVT_LEFT_DOWN, [this, colorPane](wxMouseEvent& event)
			{SelectColorPane(colorPane); });

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

		penPane->Bind(wxEVT_LEFT_DOWN, [this, penPane](wxMouseEvent& event)
			{SelectPenPane(penPane); });

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
	controlsPanel->SetBackgroundColour(wxColour(isDark ? darkBackground : lightBackground));

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

	/* 
	//OLD BUTTONS BELOW PENS
	auto pngbutton = new wxButton(controlsPanel, wxID_ANY, "Save as png");
	auto xmlbutton = new wxButton(controlsPanel, wxID_ANY, "Save as xml");
	auto loadbutton = new wxButton(controlsPanel, wxID_ANY, "Load xml file");

	mainSizer->AddStretchSpacer();
	mainSizer->Add(pngbutton, 0, wxALL, FromDIP(5));
	mainSizer->Add(xmlbutton, 0, wxALL, FromDIP(5));
	mainSizer->Add(loadbutton, 0, wxALL, FromDIP(5));
	mainSizer->AddSpacer(FromDIP(5));
	*/

	controlsPanel->SetSizer(mainSizer);

	/* 
	//OLD BUTTON FUNCTIONS
	pngbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& EVENT)
		{canvas->ShowSaveDialog(); });

	xmlbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& EVENT)
		{canvas->SaveToXml(); });

	loadbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& EVENT)
		{canvas->LoadFromXml(); });
	*/

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
	this->SetMinSize({ FromDIP(400), FromDIP(200) });

	SelectColorPane(colorPanes[0]);
	SelectPenPane(penPanes[0]);
	SetupMainMenu();

	/* MenuBar Event Handling */
	Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT); // Quit Event
	Bind(wxEVT_MENU, &MyFrame::OpenXML, this, wxID_OPEN); // Open Event
	Bind(wxEVT_MENU, &MyFrame::OnNew, this, wxID_NEW); // New Event

	//Save As Events
	Bind(wxEVT_MENU, &MyFrame::SaveAsPNG, this, wxID_SAVEAS); // PNG
	Bind(wxEVT_MENU, &MyFrame::SaveAsXML, this, wxID_SAVE); // XML

	Bind(wxEVT_MENU, &MyFrame::OnClear, this, wxID_DELETE);

}

/*
	function to iterate over all available color panes
	and select the one chosen by the user
*/
void MyFrame::SelectColorPane(ColorPane* pane)
{
	for (auto colorPane : colorPanes)
	{
		colorPane->selected = (colorPane == pane);
		colorPane->Refresh();
	}

	canvas->currentColor = pane->color;
}

/*
	function to iterate over all available pen panes
	and select the one chosen by the user
*/

void MyFrame::SelectPenPane(PenSizePane* pane)
{
	for (auto penPane : penPanes)
	{
		penPane->selected = (penPane == pane);
		penPane->Refresh();
	}

	canvas->currentWidth = pane->penWidth;
}

/*
	Fucntion to set up the Menu Bar
*/
void MyFrame::SetupMainMenu()
{
	//Menu Bar 
	wxMenuBar* menuBar = new wxMenuBar();

	//File Menu
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW); // Create a New Canvas
	fileMenu->Append(wxID_OPEN); // Open an existing Image
	fileMenu->AppendSeparator();

	//Save As submenu 
	wxMenu* SaveSubMenu = new wxMenu();
	SaveSubMenu->Append(wxID_SAVEAS, "&Save As PNG\tCtrl+S");
	SaveSubMenu->Append(wxID_SAVE, "&Save As Xml\tCtrl+Alt+S");
	fileMenu->AppendSubMenu(SaveSubMenu, "Save As");


	fileMenu->Append(wxID_EXIT); // Exit Program

	menuBar->Append(fileMenu, "File");

	//Edit Menu
	wxMenu* editMenu = new wxMenu();
	//editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z");
	//editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y");
	//Custom MenuItem
	editMenu->Append(wxID_DELETE, _("&Clear\tCtrl+D"));

	wxAcceleratorEntry entries[3];
	entries[0].Set(wxACCEL_CTRL, (int)'D', wxID_DELETE);
	entries[1].Set(wxACCEL_CTRL, (int)'S', wxID_SAVEAS);
	entries[2].Set(wxACCEL_CTRL | wxACCEL_ALT, (int)'S', wxID_SAVE);
	wxAcceleratorTable accel(3, entries);
	SetAcceleratorTable(accel);

	menuBar->Append(editMenu, "Edit");
	SetMenuBar(menuBar);
}
