"GameMenu" [$WIN32]
{
	"1"
	{
		"label" "#GameUI_GameMenu_ResumeGame"
		"command" "ResumeGame"
		"OnlyInGame" "1"
	}
	"2"
	{
		"label" "#GameUI_GameMenu_Disconnect"
		"command" "Disconnect"
		"OnlyInGame" "1"
	} 
	
	"3"
	{
		"label" "Singleplayer Map (gm_construct)"
		"command" "engine map gm_construct"
		"notmulti" "1"
	}
	
	"4"
	{
		"label" "------------------------"
		"OnlyInGame" "1"
		"command" "spacer"
	}
	
	"5"
	{
		"label" "#GameUI_GameMenu_SinglePlayer"
		"command" "engine ToggleMyPanel"
	}
	"6"
	{
		"label" "#GameUI_GameMenu_LoadGame"
		"command" "OpenLoadGameDialog"
	}
	
	"7"
	{
		"label" "------------------------"
		"command" "spacer"
	}
	
	"12"
	{
		"label" "#GameUI_GameMenu_Options"
		"command" "OpenOptionsDialog"
	}
	
	"13"
	{
		"label" "------------------------"
		"command" "spacer"
	}
	
	"14"
	{
		"label" "#GameUI_GameMenu_Quit"
		"command" "Quit"
	}
}
