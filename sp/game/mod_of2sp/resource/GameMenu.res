"GameMenu"
{
	"1"
	{
		"label" "#GameUI_GameMenu_ResumeGame"
		"command" "ResumeGame"
		"InGameOrder" "10"
		"OnlyInGame" "1"
	}
	"2"
	{
		"label" "DEV MENU"
		"command" "engine debugsystemui"
		"InGameOrder" "20"
		"OnlyInGame" "1"
	}
	"5"	
	{
		"label" "#GameUI_GameMenu_NewGame"
		"command" "OpenNewGameDialog"
		"InGameOrder" "40"
		"notmulti" "1"
	}
	"6"
	{
		"label" "#GameUI_GameMenu_LoadGame"
		"command" "OpenLoadGameDialog"
		"InGameOrder" "30"
		"notmulti" "1"
	}
	"7"
	{
		"label" "#GameUI_GameMenu_SaveGame"
		"command" "OpenSaveGameDialog"
		"InGameOrder" "20"
		"notmulti" "1"
		"OnlyInGame" "1"
	}
	"8"
	{
		"label" "#GameUI_GameMenu_Achievements"
		"command" "OpenAchievementsDialog"
		"InGameOrder" "50"
	}
	"9"
	{
		"label" "#GameUI_Controller"
		"command" "OpenControllerDialog"
		"InGameOrder" "60"
		"ConsoleOnly" "1"
	}
	"10"
	{
		"label" "#GameUI_GameMenu_Options"
		"command" "OpenOptionsDialog"
		"InGameOrder" "70"
	}

	"14"
	{
		"label" "load of2_canals_02"
		"command" "engine map of2_canals_02"
		"InGameOrder" "90"
	}
	"15"
	{
		"label" "load of2_crossfire"
		"command" "engine map of2_crossfire"
		"InGameOrder" "100"
	}
	"16"
	{
		"label" "load of2_stadium"
		"command" "engine map of2_stadium"
		"InGameOrder" "110"
	}
	"17"
	{
		"label" "load of2_ichthyosaur_test"
		"command" "engine map of2_ichthyosaur_test"
		"InGameOrder" "120"
	}
	"18"
	{
		"label" "load dev_gauss_test"
		"command" "engine map dev_gauss_test"
		"InGameOrder" "130"
	}
	"19"
	{
		"label" "#GameUI_GameMenu_Quit"
		"command" "Quit"
		"InGameOrder" "140"
	}
	
}

