///////////////////////////////////////////////////////////
// Tracker scheme resource file
//
// sections:
//		Colors			- all the colors used by the scheme
//		BaseSettings	- contains settings for app to use to draw controls
//		Fonts			- list of all the fonts used by app
//		Borders			- description of all the borders
//
///////////////////////////////////////////////////////////
Scheme
{
	//////////////////////// COLORS ///////////////////////////
	// color details
	// this is a list of all the colors used by the scheme
	Colors
	{
	}
	
	///////////////////// BASE SETTINGS ////////////////////////
	//
	// default settings for all panels
	// controls use these to determine their settings
	BaseSettings
	{
		"FgColor"			"201 255 0 128"
		"BgColor"			"0 0 0 0" //76
		"DgColor"			"0 255 0 99"

		"Panel.FgColor"			"201 255 0 128"
		"Panel.BgColor"			"0 0 0 0" //76
		
		"BrightFg"		"201 255 0 255"

		"DamagedBg"			"0 0 0 0"
		"DamagedFg"			"180 0 0 255"
		"BrightDamagedFg"		"255 0 0 255"

		// weapon selection colors
		"SelectionNumberFg"		"201 255 0 255"
		"SelectionTextFg"		"255 220 0 255"
		"SelectionEmptyBoxBg" 	"0 0 0 80"
		"SelectionBoxBg" 		"0 0 0 80"
		"SelectionSelectedBoxBg" "0 0 0 80"
		
		"ZoomReticleColor"	"255 220 0 255"

		// HL1-style HUD colors
		"Yellowish"			"201 255 0 128"
		"Normal"			"201 255 0 255"
		"Caution"			"255 48 0 255"

		// Top-left corner of the "Half-Life 2" on the main screen
		"Main.Title1.X"		"16" // 32
		"Main.Title1.Y"		"316" // 316
		"Main.Title1.Color"	"255 255 255 255" // 255 255 255 255
		
		// Top-left corner of the "DEATHMATCH" on the main screen
		"Main.Title2.X"		"16" //32
		"Main.Title2.Y"		"348" //348
		"Main.Title2.Color"	"255 255 255 180" // 255 255 255 180

		// Top-left corner of the menu on the main screen
		"Main.Menu.X"		"16" // 32
		"Main.Menu.Y"		"396" // 396

		// Blank space to leave beneath the menu on the main screen
		"Main.BottomBorder"	"14" // 14
	}

	//////////////////////// BITMAP FONT FILES /////////////////////////////
	//
	// Bitmap Fonts are ****VERY*** expensive static memory resources so they are purposely sparse
	BitmapFontFiles
	{
		// UI buttons, custom font, (256x64)
		"Buttons"		"materials/vgui/fonts/buttons_32.vbf"
	}

	
	//////////////////////// FONTS /////////////////////////////
	//
	// describes all the fonts
	Fonts
	{
		// fonts are used in order that they are listed
		// fonts are used in order that they are listed
		"DebugFixed"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts are used in order that they are listed
		"DebugFixedSmall"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts listed later in the order will only be used if they fulfill a range not already filled
		// if a font fails to load then the subsequent fonts will replace
		Default
		{
			"1"	[$WIN32]
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"9"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
				"scanlines" "1"
			}
			"2"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"12"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
				"scanlines" "1"
			}
			"3"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
				"scanlines" "1"
			}
			"4"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"20"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
				"scanlines" "1"
			}
			"5"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"24"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1200 10000"
				"additive"	"1"
				"scanlines" "1"
			}
		}
		"DefaultSmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Arial Narrow"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
		}
		"DefaultVerySmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"11"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
		}
		WeaponIcons
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"32"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		NightvisionIcon
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"48"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelected
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"32"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"blur"		"5"
				"scanlines"	"2"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSmall
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"24"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		Crosshairs
		{
			"1"
			{
				"name"		"crosshairs"
				"tall"		"60"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"scanlines" "1"
				"custom"	"1"
				"yres"		"1 10000"
			}
		}
		QuickInfo
		{
			"1"	[$X360]
			{
				"name"		"HL2cross"
				"tall"		"57"
				"weight"	"0"
				"antialias" 	"1"
				"additive"	"1"
				"custom"	"1"
			}
			"1"	[$WIN32]
			{
				"name"		"HL2cross"
				"tall"		"28"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudNumbers
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"24"	[$WIN32]
				"weight"	"0"
				"antialias" "1"
				"scanlines" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudNumbersGlow
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"24"	[$WIN32]
				"weight"	"0"
				"blur"		"5"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudNumbersSmall
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"10"	[$WIN32]
				"weight"	"200"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersSmallGlow
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"14"
				"weight"	"0"
				"blur"		"5"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudText
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"10"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudTextGlow
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"10"
				"weight"	"0"
				"blur"		"5"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		OF2HudIcons
		{
			"1"
			{
				"name"		"Counter-Strike"
				"tall"		"41"	[$WIN32]
				"weight"	"0"
				"additive"	"1"
				"antialias" "1"
				"scanlines" "1"
				"custom"	"1"
			}
		}
		OF2HudIconsGlow
		{
			"1"
			{
				"name"		"Counter-Strike"
				"tall"		"41"	[$WIN32]
				"weight"	"0"
				"blur"		"5"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbers
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"24"	[$WIN32]
				"weight"	"0"
				"scanlines" "1"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		SquadIcon
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"50"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersGlow
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"24"	
				"weight"	"0"
				"blur"		"5"
			  //"scanlines" "2"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudIconsGlow
		{
			"1"
			{
				"name"		"Counter-Strike"
				"tall"		"41"	
				"blur"		"5"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersSmall
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"12"	[$WIN32]
				"weight"	"1000"
				"additive"	"1"
				//"scanlines" "1"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudSelectionNumbers
		{
			"1"
			{
				"name"		"Eurostile"
				"tall"		"11"
				"weight"	"700"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextLarge
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"1.0"
				"scaley"	"1.0"
			}
			"1"	[$WIN32]
			{
				"name"		"Eurostile"
				"tall"		"14"
				"weight"	"1000"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextSmall
		{
			"1"	[$WIN32]
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"11"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
			}
			"1"	[$X360]
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudSelectionText
		{
			"1"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"8"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"10"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"12"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"16"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"OCR A Becker RUS-LAT"
				"tall"		"17"
				"weight"	"1000"
				"antialias" "1"
				"yres"	"1200 10000"
			}
		}
		GameUIButtons
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"0.63"
				"scaley"	"0.63"
				"scalex_hidef"	"1.0"
				"scaley_hidef"	"1.0"
			}
		}
		BudgetLabel
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
		}
		DebugOverlay
		{
			"1"	[$WIN32]
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
			"1"	[$X360]
			{
				"name"		"Tahoma"
				"tall"		"18"
				"weight"	"200"
				"outline"	"1"
			}
		}
		"CloseCaption_Normal"
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"32"
				"weight"	"500"
				"antialias" "1"
				"russian"
				{
					"range" "0x0000 0xFFFF"
				}
				"english"
				{
					"range" "0x0000 0xFFFF"
				}
			}
		}
		"CloseCaption_Italic"
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"32"
				"weight"	"500"
				"italic"	"1"
				"antialias" "1"
				"range"		"0x0000 0xFFFF"
			}
		}
		"CloseCaption_Bold"
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"32"
				"weight"	"900"
				"antialias" "1"
				"range"		"0x0000 0xFFFF"
			}
		}
		"CloseCaption_BoldItalic"
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"32"
				"weight"	"900"
				"italic"	"1"
				"antialias" "1"
				"range"		"0x0000 0xFFFF"
			}
		}
		"CloseCaption_Small"
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"16"
				"tall_hidef"	"16"
				"weight"	"900"
				"antialias" "1"
				"range"		"0x0000 0xFFFF" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
			}
		}
		// this is the symbol font
		"Marlett"
		{
			"1"
			{
				"name"		"Marlett"
				"tall"		"14"
				"weight"	"0"
				"symbol"	"1"
			}
		}
		"Trebuchet24"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x007F"	//	Basic Latin
				"antialias" "1"
				"additive"	"1"
			}
		}
		"Trebuchet18"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"18"
				"weight"	"900"
			}
		}
		ClientTitleFont
		{
			"1"
			{
				"name"  "K12HL2"
				"tall"			"28"
				"tall_hidef"	"34"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
			}
		}
		CreditsLogo
		{
			"1"
			{
				"name"		"K12HL2"
				"tall"		"128"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsText
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"32"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		CreditsOutroLogos
		{
			"1"
			{
				"name"		"K12HL2"
				"tall"		"48"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsOutroText
		{
			"1"
			{
				"name"		"Arial Narrow"
				"tall"		"12"
				"weight"	"900"
				"antialias" "1"
			}
		}
		CenterPrintText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HDRDemoText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		"AchievementNotification"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
			}
		}
		"CommentaryDefault"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"	[$WIN32]
				"tall"		"20"	[$X360]
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"900"
			}
			"7"
			{
				"name"		"Arial Narrow"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"800"
			}
			
		}
		
		 "InstructorTitle"
		{
			"1"
			{
				"name"			"Evropa Normal"
				"tall"			"20"
				"weight"		"400"
				"antialias"		"1"
				"dropshadow"	"1"
			}
		}

		"InstructorKeyBindings"
		{
			"1"
			{
				"name"			"Evropa Normal"
				"tall"			"18"
				"weight"		"600"
				"antialias"		"1"
				"dropshadow"	"0"
			}
		}

		"InstructorButtons"
		{
			"1"
			{
				"name"			"Arial Narrow"
				"tall"			"15"
				"weight"		"600"
				"antialias"		"1"
				"dropshadow"	"1"
			}
		}

		"InstructorTitleGlow"
		{
			"1"
			{
				"name"			"Evropa Normal"
				"tall"			"20"
				"weight"		"400"
				"antialias"		"1"
				"dropshadow"	"1"
			}
		}
	}

	
	//////////////////////// CUSTOM FONT FILES /////////////////////////////
	//
	// specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
	CustomFontFiles
	{
		"1"		"resource/fonts/HALFLIFE2.ttf"
		"2"		"resource/HL2crosshairs.ttf"
		"3"	    "resource/fonts/HL2EP2.ttf"		
		"4"		"resource/fonts/Cstrike.ttf"
		"5"		"resource/fonts/gautami.ttf"
		"6"	    "resource/fonts/Eurosti.ttf"	
		"7"	    "resource/fonts/K12HL2.ttf"	
		"8"	    "resource/fonts/12619.otf"
		"9"	    "resource/fonts/OCRAEXT.ttf"
		"10"    "resource/fonts/crosshairs.ttf"
		"11"    
		{
		  "font" "resource/fonts/arial-narrow.ttf"
		  "name" "Arial Narrow"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
		"12"    
		{
		  "font" "resource/fonts/arial-narrow-bold.ttf"
		  "name" "Arial Narrow"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
		"13"
		{
		"font" "resource/fonts/arial-narrow-bold-italic.ttf"
		  "name" "Arial Narrow"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
		"14"
		"font" "resource/fonts/arial-narrow-italic.ttf"
		  "name" "Arial Narrow"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
		"15"   
		{
		  "font" "resource/fonts/dsarmy.ttf"
		  "name" "DS Army Cyr"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
		"16"   
		{
		  "font" "resource/fonts/evropa.ttf"
		  "name" "Evropa Normal"
		  "russian"
		  {
		     "range" "0x0000 0xFFFF"
		  }
		  "english"
		  {
		    "range" "0x0000 0xFFFF"
		  }
		}
	}

}