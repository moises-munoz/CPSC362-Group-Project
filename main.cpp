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
	
	//Edit this to create a Startwindow Obj
	//MyFrame* frame = new MyFrame("EzPaint", wxDefaultPosition, wxDefaultSize);
	//frame->Show(true);
	StartWindow* startWindow = new StartWindow("EzPaint Launcher");
	startWindow->Show(true);
	return true;
}

