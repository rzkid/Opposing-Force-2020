"Resource/HudLayout.res"
{
	HudHealth [$WIN32]
	{
		"fieldName"		"HudHealth"
		"xpos"	"5"
		"ypos"	"442"
		"wide"	"90"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
		
		"TextFont" "OF2HudIcons"
		
		"text_xpos" "5"
		"text_ypos" "-6"
		"text2_xpos" "5"
		"text2_ypos" "-8"
		"icon_xpos" "5"
		"icon_ypos" "-6"
		"digit_xpos" "30"
		"digit_ypos" "3"
	}
	
	HudSuit [$WIN32]
	{
		"fieldName"		"HudSuit"
		"xpos"	"100"
		"ypos"	"442"
		"wide"	"90"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"
		
		"TextFont" "OF2HudIcons"

		"PaintBackgroundType"	"2"
		
		"text_xpos" "5" //32
		"text_ypos" "-6" //23
		"text2_xpos" "57"
		"text2_ypos" "23"
		"icon_xpos" "5"
		"icon_ypos" "-6"
		"digit_xpos" "30"
		"digit_ypos" "3"
	}

	HudAmmo
	{
		"fieldName" "HudAmmo"
		"xpos"	"r146"
		"ypos"	"442"
		"wide"	"140"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "54" //67
		"text_ypos" "-3" //20
		"text2_xpos" "0" //72
		"text2_ypos" "0" //23
//		"text3_xpos" "36"
//		"text3_ypos" "5"
		"icon_xpos" "8"
		"icon_ypos" "20"
		"digit_xpos" "5"
		"digit_ypos" "2"
		"digit2_xpos" "74"
		"digit2_ypos" "2"
	}

	HudAmmoSecondary	[$WIN32]
	{
		"fieldName" "HudAmmoSecondary"
		"xpos"	"r48"
		"ypos"	"406"
		"wide"	"42"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "67"
		"text_ypos" "6"
		"text2_xpos" "72"
		"text2_ypos" "23"
//		"text3_xpos" "36"
//		"text3_ypos" "-5"
		"icon_xpos" "10"
		"icon_ypos" "30"
		"digit_xpos" "12"
		"digit_ypos" "2"
		"digit2_xpos" "-50"
//		"digit2_ypos" "5"
	}
	HudPosture	[$WIN32]
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"16"
		"ypos"	"316"
		"tall"  "36"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"10"
		"icon_ypos" 	"0"
	}
	HudPosture	[$X360]
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"48"
		"ypos"	"316"
		"tall"  "36"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"10"
		"icon_ypos" 	"2"
	}

	HudSuitPower	[$WIN32]
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"5"
		"ypos"	"390"
		"wide"	"102"
		"tall"	"26"
		
		"AuxPowerLowColor" "255 0 0 220"
		"AuxPowerHighColor" "255 220 0 220"
		"AuxPowerDisabledAlpha" "70"

		"BarInsetX" "8"
		"BarInsetY" "15"
		"BarWidth" "92"
		"BarHeight" "4"
		"BarChunkWidth" "6"
		"BarChunkGap" "3"

		"text_xpos" "8"
		"text_ypos" "4"
		"text2_xpos" "8"
		"text2_ypos" "22"
		"text2_gap" "10"

		"PaintBackgroundType"	"2"
	}
	HudSuitPower	[$X360]
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"48"
		"ypos"	"370"
		"wide"	"102"
		"tall"	"26"
		
		"AuxPowerLowColor" "255 100 0 220"
		"AuxPowerHighColor" "255 220 0 220"
		"AuxPowerDisabledAlpha" "70"

		"BarInsetX" "8"
		"BarInsetY" "19"
		"BarWidth" "92"
		"BarHeight" "5"
		"BarChunkWidth" "6"
		"BarChunkGap" "3"

		"text_xpos" "8"
		"text_ypos" "4"
		"text2_xpos" "8"
		"text2_ypos" "25"
		"text2_gap" "12"

		"PaintBackgroundType"	"2"
	}
	
	HudFlashlight
	{
		"fieldName" "HudFlashlight"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"270"		[$WIN32]
		"ypos"	"444"		[$WIN32]
		"xpos_hidef"	"306"		[$X360]		// aligned to left
		"xpos_lodef"	"c-18"		[$X360]		// centered in screen
		"ypos"	"428"		[$X360]				
		"tall"  "24"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		
		"icon_xpos"	"4"
		"icon_ypos" "-8"
		
		"BarInsetX" "4"
		"BarInsetY" "18"
		"BarWidth" "28"
		"BarHeight" "2"
		"BarChunkWidth" "2"
		"BarChunkGap" "1"
	}

	HudLocator
	{
		"fieldName" "HudLocator"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"c8"	[$WIN32]
		"ypos"	"r36"	[$WIN32]
		"xpos"	"c-32"	[$X360]
		"ypos_hidef"	"r52"	[$X360]
		"ypos_lodef"	"r95"	[$X360]		// 52 is aligned to bottom of HudSuit
		"wide"	"64"
		"tall"  "24"
	}

	HudDamageIndicator
	{
		"fieldName" "HudDamageIndicator"
		"visible" "1"
		"enabled" "1"
		"DmgColorLeft" "255 0 0 0"
		"DmgColorRight" "255 0 0 0"
		
		"dmg_xpos" "30"
		"dmg_ypos" "100"
		"dmg_wide" "36"
		"dmg_tall1" "240"
		"dmg_tall2" "200"
	}

	HudZoom
	{
		"fieldName" "HudZoom"
		"visible" "1"
		"enabled" "1"
		"Circle1Radius" "66"
		"Circle2Radius"	"74"
		"DashGap"	"16"
		"DashHeight" "4"	[$WIN32]
		"DashHeight" "6"	[$X360]		
		"BorderThickness" "88"
	}

	HudWeaponSelection
	{
		"fieldName" "HudWeaponSelection"
		"ypos" 	"16"	[$WIN32]
		"ypos" 	"32"	[$X360]
		"visible" "1"
		"enabled" "1"
		"SmallBoxSize" "32"
		"MediumBoxWide"	"95"
		"MediumBoxWide_hidef"	"78"
		"MediumBoxTall"	"50"
		"MediumBoxTall_hidef"	"50"
		"MediumBoxWide_lodef"	"74"
		"MediumBoxTall_lodef"	"50"
		"LargeBoxWide" "112"
		"LargeBoxTall" "80"
		"BoxGap" "8"
		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"
		"TextYPos" "64"
	}

	HudCrosshair
	{
		"fieldName" "HudCrosshair"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudDeathNotice
	{
		"fieldName" "HudDeathNotice"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudVehicle
	{
		"fieldName" "HudVehicle"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	ScorePanel
	{
		"fieldName" "ScorePanel"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudTrain
	{
		"fieldName" "HudTrain"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMOTD
	{
		"fieldName" "HudMOTD"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMessage
	{
		"fieldName" "HudMessage"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMenu
	{
		"fieldName" "HudMenu"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudCloseCaption
	{
		"fieldName" "HudCloseCaption"
		"visible"	"1"
		"enabled"	"1"
		"xpos"		"c-250"
		"ypos"		"276"
		"wide"		"500"
		"tall"		"136"
		
		"textAlignment"		"center"

		"BgAlpha"	"128"

		"GrowTime"		"0.25"
		"ItemHiddenTime"	"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"	"0.15"	// Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"	"0.3"
		"topoffset"		"0"		[$WIN32]
	}

	HudChat
	{
		"fieldName" "HudChat"
		"visible" "0"
		"enabled" "1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	 "4"
		"tall"	 "4"
	}

	HudHistoryResource	[$WIN32]
	{
		"fieldName" "HudHistoryResource"
		"visible" "1"
		"enabled" "1"
		"xpos"	"r252"
		"ypos"	"40"
		"wide"	 "248"
		"tall"	 "320"

		"history_gap"	"56"
		"icon_inset"	"38"
		"text_inset"	"36"
		"NumberFont"	"HudNumbersSmall"
	}

	HudGeiger
	{
		"fieldName" "HudGeiger"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HUDQuickInfo
	{
		"fieldName" "HUDQuickInfo"
		"visible" "0"
		"enabled" "0"
		"wide"	 "0"
		"tall"	 "0"
	}
	
	HudScope
	{
		"fieldName" "HudScope"
		"visible" "1"
		"enabled" "1"
		"wide"	 "1280"
		"tall"	 "720"
	}

	HudWeapon
	{
		"fieldName" "HudWeapon"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}
	HudAnimationInfo
	{
		"fieldName" "HudAnimationInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudPredictionDump
	{
		"fieldName" "HudPredictionDump"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudHintDisplay
	{
		"fieldName"	"HudHintDisplay"
		"visible"	"0"
		"enabled" "1"
		"Alpha"		"0"		// Remove this to enable hint hud element
		"xpos"	"r120"	[$WIN32]
		"ypos"	"r340"	[$WIN32]
		"xpos"	"r148"	[$X360]
		"ypos"	"r338"	[$X360]
		"wide"	"100"
		"tall"	"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}

	HudHintKeyDisplay
	{
		"fieldName"	"HudHintKeyDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"	[$WIN32]
		"ypos"		"r340"	[$WIN32]
		"xpos"		"r148"	[$X360]
		"ypos"		"r338"	[$X360]
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}


	HudSquadStatus
	{
		"fieldName"	"HudSquadStatus"
		"visible"	"1"
		"enabled" "1"
		"xpos"	"r120"
		"ypos"	"380"
		"wide"	"104"
		"tall"	"46"
		"text_xpos"	"8"
		"text_ypos"	"34"
		"IconInsetX"	"8"
		"IconInsetY"	"0"
		"IconGap"		"24"
		"font"	"SquadIcon"

		"PaintBackgroundType"	"2"
	}

	HudPoisonDamageIndicator	[$WIN32]
	{
		"fieldName"	"HudPoisonDamageIndicator"
		"visible"	"0"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"346"
		"wide"	"136"
		"tall"	"38"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_ygap" "14"
		"TextColor"	"255 170 0 220"
		"PaintBackgroundType"	"2"
	}

	HudCredits
	{
		"fieldName"	"HudCredits"
		"visible"	"1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"	"480"
		"fgcolor_override"	"201 255 0 255"
	}
	
	HUDAutoAim
	{
		"fieldName" "HUDAutoAim"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"	
		"tall"	 "480"
	}

	HudCommentary
	{
		"fieldName" "HudCommentary"
		"xpos"	"c-190"
		"ypos"	"350"
		"wide"	"380"
		"tall"  "40"
		"visible" "1"
		"enabled" "1"
		
		"PaintBackgroundType"	"2"
		
		"bar_xpos"		"50"
		"bar_ypos"		"20"
		"bar_height"	"8"
		"bar_width"		"320"
		"speaker_xpos"	"50"
		"speaker_ypos"	"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"	"8"
		
		"icon_texture"	"vgui/hud/icon_commentary"
		"icon_xpos"		"0"
		"icon_ypos"		"0"		
		"icon_width"	"40"
		"icon_height"	"40"
	}
	
	HudHDRDemo
	{
		"fieldName" "HudHDRDemo"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"
		
		"Alpha"	"255"
		"PaintBackgroundType"	"2"
		
		"BorderColor"	"0 0 0 255"
		"BorderLeft"	"16"
		"BorderRight"	"16"
		"BorderTop"		"16"
		"BorderBottom"	"64"
		"BorderCenter"	"0"
		
		"TextColor"		"255 255 255 255"
		"LeftTitleY"	"422"
		"RightTitleY"	"422"
	}
	"HudChatHistory"
	{
		"ControlName"		"RichText"
		"fieldName"		"HudChatHistory"
		"xpos"			"10"
		"ypos"			"290"
		"wide"	 		"300"
		"tall"			 "75"
		"wrap"			"1"
		"autoResize"		"1"
		"pinCorner"		"1"
		"visible"		"0"
		"enabled"		"1"
		"labelText"		""
		"textAlignment"		"south-west"
		"font"			"Default"
		"maxchars"		"-1"
	}

	AchievementNotificationPanel	
	{
		"fieldName"				"AchievementNotificationPanel"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"180"
		"wide"					"f10"	[$WIN32]
		"wide"					"f60"	[$X360]
		"tall"					"100"
	}
}