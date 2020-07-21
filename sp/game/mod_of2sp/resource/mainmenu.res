"MainMenu"
{	
	"ResumeGame"
	{
		"text"			"#GameUI_GameMenu_ResumeGame"
		"description"	"#GameUI_ResumeGameDescription"
		"command"		"cvar gamemenucommand resumegame"
		"priority"		"6"
		"specifics"		"ingame"
	}
	
	"SaveGame"
	{
		"text"			"#GameUI_GameMenu_SaveGame"
		"description"	"#GameUI_SaveGameDescription"
		"command"		"cvar gamemenucommand opensavegamedialog"
		"priority"		"5"
		"specifics"		"ingame"
	}
	
	"NewGame"
	{
		"text"			"#GameUI_GameMenu_NewGame"
		"description"	"#GameUI_NewGameDescription"
		"command"		"cvar gamemenucommand opennewgamedialog"
		"priority"		"5"
		"specifics"		"mainmenu"
	}
	
	"LoadGame"
	{
		"text"			"#GameUI_GameMenu_LoadGame"
		"description"	"#GameUI_LoadGameDescription"
		"command"		"cvar gamemenucommand openloadgamedialog"
		"priority"		"4"
		"specifics"		"shared"
	}
	
	"Options"
	{
		"text"			"#GameUI_GameMenu_Options"
		"description"	"#GameUI_OptionsDescription"
		"command"		"cvar gamemenucommand openoptionsdialog"
	//	"command"		"cvar gameui2_openoptionsdialog"
		"priority"		"3"
		"specifics"		"shared"
	}

	"Bonus"
	{
		"text"			"#GameUI_GameMenu_BonusMaps"
		"description"	"#GameUI_BonusMapsDescription"
		"command"		"cvar gamemenucommand OpenBonusMapsDialog"
		"priority"		"2"
		"specifics"		"mainmenu"
	}

	"Quit"
	{
		"text"			"#GameUI_GameMenu_Quit"
		"description"	"#GameUI_QuitDescription"
		"command"		"cvar gamemenucommand quit"
	//	"command"		"cvar gameui2_openquitgamedialog"
		"priority"		"1"
		"specifics"		"shared"
	}
}