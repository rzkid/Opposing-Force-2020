"MainMenu"
{	
	"ResumeGame"
	{
		"text"			"#GameUI_ResumeGame"
		"description"	"#GameUI_ResumeGameDescription"
		"command"		"cvar gamemenucommand resumegame"
		"priority"		"5"
		"specifics"		"ingame"
	}
	
	"SaveGame"
	{
		"text"			"#GameUI_SaveGame"
		"description"	"#GameUI_SaveGameDescription"
		"command"		"cvar gamemenucommand opensavegamedialog"
		"priority"		"4"
		"specifics"		"ingame"
	}
	
	"NewGame"
	{
		"text"			"#GameUI_NewGame"
		"description"	"#GameUI_NewGameDescription"
		"command"		"cvar gamemenucommand opennewgamedialog"
		"priority"		"4"
		"specifics"		"mainmenu"
	}
	
	"LoadGame"
	{
		"text"			"#GameUI_LoadGame"
		"description"	"#GameUI_LoadGameDescription"
		"command"		"cvar gamemenucommand openloadgamedialog"
		"priority"		"3"
		"specifics"		"shared"
	}
	
	"Options"
	{
		"text"			"#GameUI_Options"
		"description"	"#GameUI_OptionsDescription"
		"command"		"cvar gamemenucommand openoptionsdialog"
	//	"command"		"cvar gameui2_openoptionsdialog"
		"priority"		"2"
		"specifics"		"shared"
	}

	"Quit"
	{
		"text"			"#GameUI_Quit"
		"description"	"#GameUI_QuitDescription"
		"command"		"cvar gamemenucommand quit"
	//	"command"		"cvar gameui2_openquitgamedialog"
		"priority"		"1"
		"specifics"		"shared"
	}
}