#include <wx/wx.h>
#include "StartWindow.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();
	
	//Creates and opens the EzPaint Launcher Window
	StartWindow* startWindow = new StartWindow("EzPaint Launcher", wxDefaultPosition, wxDefaultSize);
	startWindow->Maximize(true);
	startWindow->Show(true);
	startWindow->SetSize(800, 600);
	return true;
}

