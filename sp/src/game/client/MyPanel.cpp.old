//The following include files are necessary to allow your MyPanel.cpp to compile.
#include "cbase.h"
#include "IMyPanel.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

//CMyPanel class: Tutorial example class
class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);
	//CMyPanel : This Class / vgui::Frame : BaseClass

	CMyPanel(vgui::VPANEL parent); 	// Constructor
	~CMyPanel(){};				// Destructor

protected:
	//VGUI overrides:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);

private:
	//Other used VGUI control Elements:
	Button *m_pCloseButton;

};

// Constuctor: Initializes the Panel
CMyPanel::CMyPanel(vgui::VPANEL parent)
	: BaseClass(NULL, "MyPanel")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);
	SetTitle("Play Singleplayer", parent ? false : true);


	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/UI/mypanel.res");

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	DevMsg("MyPanel has been constructed\n");

	//m_pCloseButton = new Button(this, "Button", "Close", this, "turnoff");
	//m_pCloseButton->SetPos(433, 472);
	//m_pCloseButton->SetDepressedSound("common/bugreporter_succeeded.wav");
	//m_pCloseButton->SetReleasedSound("ui/buttonclick.wav");
}

//Class: CMyPanelInterface Class. Used for construction.
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel *MyPanel;
public:
	CMyPanelInterface()
	{
		MyPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		MyPanel = new CMyPanel(parent);
	}
	void Destroy()
	{
		if (MyPanel)
		{
			MyPanel->SetParent((vgui::Panel *)NULL);
			delete MyPanel;
		}
	}
	void Activate(void)
	{
		if (MyPanel)
		{
			MyPanel->Activate();
		}
	}
};
static CMyPanelInterface g_MyPanel;
IMyPanel* mypanel = (IMyPanel*)&g_MyPanel;

ConVar cl_showmypanel("cl_showmypanel", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_showmypanel.GetBool()); //CL_SHOWMYPANEL / 1 BY DEFAULT
}

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);
	if (!Q_stricmp(pcCommand, "turnoff"))
		cl_showmypanel.SetValue(0);
}

CON_COMMAND(ToggleMyPanel, "Toggles myPanel on or off")
{
	cl_showmypanel.SetValue(!cl_showmypanel.GetBool());
};