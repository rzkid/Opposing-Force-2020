//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"

#include <Color.h>
#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/IVGui.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudNumericDisplay::CHudNumericDisplay(vgui::Panel *parent, const char *name) : BaseClass(parent, name)
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_iValue = 0;
//	m_iBGValue = 000;
	m_LabelText[0] = 0;
	m_iSecondaryValue = 0;
	m_bDisplayValue = true;
	m_bDisplaySecondaryValue = false;
	m_bIndent = false;
	m_bIsTime = false;
}

//-----------------------------------------------------------------------------
// Purpose: Resets values on restore/new map
//-----------------------------------------------------------------------------
void CHudNumericDisplay::Reset()
{
	m_flBlur = 0.0f;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetDisplayValue(int value)
{
	m_iValue = value;
//	m_iBGValue = 000;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetSecondaryValue(int value)
{
	m_iSecondaryValue = value;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetShouldDisplayValue(bool state)
{
	m_bDisplayValue = state;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetShouldDisplaySecondaryValue(bool state)
{
	m_bDisplaySecondaryValue = state;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetLabelText(const wchar_t *text)
{
	wcsncpy(m_LabelText, text, sizeof(m_LabelText) / sizeof(wchar_t));
	m_LabelText[(sizeof(m_LabelText) / sizeof(wchar_t)) - 1] = 0;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetIndent(bool state)
{
	m_bIndent = state;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void CHudNumericDisplay::SetIsTime(bool state)
{
	m_bIsTime = state;
}

//-----------------------------------------------------------------------------
// Purpose: paints a number at the specified position
//-----------------------------------------------------------------------------
void CHudNumericDisplay::PaintNumbers(HFont font, int xpos, int ypos, int value)
{
	surface()->DrawSetTextFont(font);
	wchar_t unicode[6];
	if ( !m_bIsTime )
	{
		V_snwprintf(unicode, ARRAYSIZE(unicode), L"%d", value);
	}
	else
	{
		int iMinutes = value / 60;
		int iSeconds = value - iMinutes * 60;

		if ( iSeconds < 10 )
			V_snwprintf( unicode, ARRAYSIZE(unicode), L"%d`0%d", iMinutes, iSeconds );
		else
			V_snwprintf( unicode, ARRAYSIZE(unicode), L"%d`%d", iMinutes, iSeconds );
	}

	// adjust the position to take into account 3 characters
	int charWidth = surface()->GetCharacterWidth(font, '0');

	if (value < 100)
	{
		xpos += charWidth;
	}
	if (value < 10)
	{
		xpos += charWidth;
	}

	surface()->DrawSetTextPos(xpos, ypos);
	surface()->DrawUnicodeString( unicode );
}

//-----------------------------------------------------------------------------
// Purpose: draws the text
//-----------------------------------------------------------------------------
void CHudNumericDisplay::PaintLabel( void )
{
//	surface()->DrawSetTextFont(m_hTextGlowFont);
//	surface()->DrawSetTextColor(GetFgColor());
//	surface()->DrawSetTextPos(text_xpos, text_ypos);
//	surface()->DrawUnicodeString(m_LabelText);
	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(GetFgColor());
	surface()->DrawSetTextPos(text_xpos, text_ypos);
	surface()->DrawUnicodeString( m_LabelText );

}

//-----------------------------------------------------------------------------
// Purpose: renders the vgui panel
//-----------------------------------------------------------------------------
void CHudNumericDisplay::Paint()
{
	if (m_bDisplayValue)
	{	
//		surface()->DrawSetTextColor(Color(76, 76, 76, 128));
		//PaintNumbers(m_hNumberFont, digit_xpos, digit_ypos, m_iBGValue);

		//bg grey zero's not small 1
		m_iconZero = gHUD.GetIcon("bg_zero");
		m_iconZero->DrawSelf(digit_xpos, digit_ypos, Color(76, 76, 76, 128));

		//bg grey zero's not small 2
		m_iconZero = gHUD.GetIcon("bg_zero");
		m_iconZero->DrawSelf(digit_xpos + 30, digit_ypos, Color(76, 76, 76, 128));

		//bg grey zero's not small 3
		m_iconZero = gHUD.GetIcon("bg_zero");
		m_iconZero->DrawSelf(digit_xpos + 63, digit_ypos, Color(76, 76, 76, 128));

		//set color for glow
		surface()->DrawSetTextColor(Color(0, 255, 0, 48));
		//make that glow
		PaintNumbers(m_hNumberGlowFont, digit_xpos, digit_ypos, m_iValue);
		//set color to FgColor
		surface()->DrawSetTextColor(GetFgColor());
		Color col = GetFgColor();
		col[3] *= 1.0f;
		surface()->DrawSetTextColor(col);
		//make normal numbers
		PaintNumbers(m_hNumberFont, digit_xpos, digit_ypos, m_iValue);
	}

	// total ammo
	if (m_bDisplaySecondaryValue)
	{
//		surface()->DrawSetTextColor(Color(76, 76, 76, 128));
//		PaintNumbers(m_hSmallNumberFont, digit2_xpos, digit2_ypos, m_iBGValue);

		//bg grey zero's small 1
		m_iconZero = gHUD.GetIcon("bg_zero_small");
		m_iconZero->DrawSelf(digit2_xpos, digit2_ypos, Color(76, 76, 76, 128));

		//bg grey zero's small 2
		m_iconZero = gHUD.GetIcon("bg_zero_small");
		m_iconZero->DrawSelf(digit2_xpos + 14, digit2_ypos, Color(76, 76, 76, 128));

		//bg grey zero's small 3
		m_iconZero = gHUD.GetIcon("bg_zero_small");
		m_iconZero->DrawSelf(digit2_xpos + 28, digit2_ypos, Color(76, 76, 76, 128));

		//repeat that but for secondary (smol) numbers
		surface()->DrawSetTextColor(Color(0, 255, 0, 48));
		PaintNumbers(m_hSmallNumberGlowFont, digit2_xpos - 5, digit2_ypos + 1, m_iSecondaryValue);
		surface()->DrawSetTextColor(GetFgColor());
		Color col = GetFgColor();
		col[3] *= 1.0f;
		surface()->DrawSetTextColor(col);
		PaintNumbers(m_hSmallNumberFont, digit2_xpos, digit2_ypos, m_iSecondaryValue);
	}

	PaintLabel();
}



