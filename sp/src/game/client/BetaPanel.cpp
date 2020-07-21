//The following include files are necessary to allow your BetaPanel.cpp to compile.
#include "cbase.h"
#include "IBetaPanel.h"
using namespace vgui;
#include <vgui_controls/TextEntry.h>
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui/ISurface.h>

//CBetaPanel class: Tutorial example class
class CBetaPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CBetaPanel, vgui::Frame);
	//CBetaPanel : This Class / vgui::Frame : BaseClass

	CBetaPanel(vgui::VPANEL parent); 	// Constructor
	~CBetaPanel(){};				// Destructor

protected:
	//VGUI overrides:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);
	virtual void Paint();
	virtual void PaintBackground(void);

private:
	//Other used VGUI control Elements:
	CPanelAnimationVar(float, m_flBackgroundAlpha, "BgAlpha", "0");
};

// Constuctor: Initializes the Panel
CBetaPanel::CBetaPanel(vgui::VPANEL parent)
	: BaseClass(NULL, "BetaPanel")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(false);
	SetMouseInputEnabled(false);


	SetProportional(false);
	SetTitleBarVisible(false);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);
	SetPaintBackgroundEnabled(false);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme"));

	LoadControlSettings("resource/UI/betapanel.res");


	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	DevMsg("BetaPanel has been constructed\n");
}

//Class: CBetaPanelInterface Class. Used for construction.
class CBetaPanelInterface : public IBetaPanel
{
private:
	CBetaPanel *BetaPanel;
public:
	CBetaPanelInterface()
	{
		BetaPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		BetaPanel = new CBetaPanel(parent);
	}
	void Destroy()
	{
		if (BetaPanel)
		{
			BetaPanel->SetParent((vgui::Panel *)NULL);
			delete BetaPanel;
		}
	}
};
static CBetaPanelInterface g_BetaPanel;
IBetaPanel* betapanel = (IBetaPanel*)&g_BetaPanel;

void CBetaPanel::OnTick()
{
	BaseClass::OnTick();
}

void CBetaPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);
}

void CBetaPanel::Paint(void)
{
	BaseClass::Paint();
	wchar_t* pText = L"PRE ALPHA";	// wide char text

	// get the right font handle for this scheme
	vgui::IScheme* pScheme = vgui::scheme()->GetIScheme(GetScheme());
	vgui::HFont hFont = pScheme->GetFont("MainMenu.Font");

	surface()->DrawSetTextFont(hFont); // set the font	
	surface()->DrawSetTextColor(255, 255, 255, 255); // full red
	surface()->DrawSetTextPos(10, 10); // x,y position
	surface()->DrawPrintText(pText, wcslen(pText)); // print text
}

void CBetaPanel::PaintBackground(void)
{
	Color bgColor = GetBgColor();
	bgColor[3] = m_flBackgroundAlpha;
}