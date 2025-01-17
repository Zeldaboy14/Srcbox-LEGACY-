//The following include files are necessary to allow your MyPanel.cpp to compile.
/*#include "cbase.h"
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
	//#define Frame_Untitled "Play Sinleplayer"

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
	//SetTitle("Play Singleplayer", parent ? false : true);
	SetTitle("Play Singleplayer", true);


	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/UI/singleplayer_srcbox.res");

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

//originally cl_showmypanel
ConVar srcbox_map_menu("srcbox_map_menu", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(srcbox_map_menu.GetBool()); //CL_SHOWMYPANEL / 1 BY DEFAULT
}

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);
	if (!Q_stricmp(pcCommand, "turnoff"))
		srcbox_map_menu.SetValue(0);
}

CON_COMMAND(ToggleMyPanel, "Toggles myPanel on or off")
{
	srcbox_map_menu.SetValue(!srcbox_map_menu.GetBool());
};*/

//ORIGINAL CODE END

/*#include "cbase.h"
#include "IMyPanel.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/Button.h>
#include <filesystem.h>

using namespace vgui;

#define FILE_FIND_HANDLE_INVALID NULL  // Define FILE_FIND_HANDLE_INVALID as NULL in SDK 2013

// Forward declaration
class CMyPanel;

// Singleton-like interface for managing the panel
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel* m_pPanel;

public:
	CMyPanelInterface() : m_pPanel(nullptr) {}

	void Create(vgui::VPANEL parent);
	void Destroy();
	void Activate();
};

// Global interface instance
static CMyPanelInterface g_MyPanel;
IMyPanel* mypanel = (IMyPanel*)&g_MyPanel;

// CMyPanel class definition
class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);

	CMyPanel(vgui::VPANEL parent);
	~CMyPanel(){};

protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);

private:
	ComboBox* m_pMapSelector;
	Button* m_pStartButton;
	CUtlVector<char*> m_mapNames;

	void PopulateMapList();
	void StartMap();
};

// Constructor for CMyPanel
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

	m_pMapSelector = new ComboBox(this, "MapSelector", 10, false);
	m_pMapSelector->SetPos(10, 30);
	m_pMapSelector->SetSize(200, 20);

	m_pStartButton = new Button(this, "StartButton", "Start", this, "startmap");
	m_pStartButton->SetPos(10, 60);

	PopulateMapList();  // Populate the map list on panel initialization
}

void CMyPanel::PopulateMapList()
{
	const char* mapsDir = "maps/*.bsp"; // Look for BSP map files in the "maps" directory
	FileFindHandle_t findHandle;       // File handle for FindFirst/FindNext
	const char* fileName = g_pFullFileSystem->FindFirst(mapsDir, &findHandle);

	while (fileName != nullptr) // Process each found file
	{
		// Ensure the file ends with ".bsp" (basic validation for maps)
		if (strstr(fileName, ".bsp"))
		{
			char mapName[MAX_PATH];
			Q_strncpy(mapName, fileName, sizeof(mapName));
			mapName[strlen(mapName) - 4] = '\0'; // Strip ".bsp" extension

			// Add map name to the ComboBox
			m_pMapSelector->AddItem(mapName, nullptr);

			// Store the map name for later use
			char* mapNameDup = new char[strlen(mapName) + 1];
			Q_strncpy(mapNameDup, mapName, strlen(mapName) + 1);
			m_mapNames.AddToTail(mapNameDup);
		}

		// Continue to the next file in the directory
		fileName = g_pFullFileSystem->FindNext(findHandle);
	}

	// Close the file search handle after we're done
	g_pFullFileSystem->FindClose(findHandle);
}


void CMyPanel::StartMap()
{
	int selectedIndex = m_pMapSelector->GetActiveItem();  // Get the selected index from the ComboBox
	if (selectedIndex != -1)  // Ensure the selection is valid
	{
		const char* mapName = m_mapNames[selectedIndex];

		if (mapName && mapName[0] != '\0')
		{
			char cmd[MAX_PATH];
			Q_snprintf(cmd, sizeof(cmd), "map %s\n", mapName);
			engine->ClientCmd(cmd);
		}
	}
}

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);

	if (!Q_stricmp(pcCommand, "startmap"))
	{
		StartMap();
	}
}

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(true);  // Always visible when active
}

// CMyPanelInterface methods
void CMyPanelInterface::Create(vgui::VPANEL parent)
{
	if (!m_pPanel)
	{
		m_pPanel = new CMyPanel(parent);
	}
}

void CMyPanelInterface::Destroy()
{
	if (m_pPanel)
	{
		m_pPanel->SetParent((vgui::Panel*)nullptr);
		delete m_pPanel;
		m_pPanel = nullptr;
	}
}

void CMyPanelInterface::Activate()
{
	if (m_pPanel)
	{
		m_pPanel->Activate();
	}
}

// Console command to toggle the panel
CON_COMMAND(srcbox_map_menu, "Map Selection for Srcbox. Currently a dropdown")
{
	if (!mypanel)
		return;

	static bool isVisible = false;
	isVisible = !isVisible;

	if (isVisible)
	{
		mypanel->Activate();
	}
	else
	{
		g_MyPanel.Destroy();
	}
}

/*#include "cbase.h"
#include "IMyPanel.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Button.h>
#include <filesystem.h>

using namespace vgui;

#define FILE_FIND_HANDLE_INVALID NULL

// Forward declaration
class CMyPanel;

// Singleton-like interface for managing the panel
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel* m_pPanel;

public:
	CMyPanelInterface() : m_pPanel(nullptr) {}

	void Create(vgui::VPANEL parent);
	void Destroy();
	void Activate();
};

// Global interface instance
static CMyPanelInterface g_MyPanel;
IMyPanel* mypanel = (IMyPanel*)&g_MyPanel;

// MapEntryPanel class for individual map entries
class MapEntryPanel : public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE(MapEntryPanel, vgui::Panel);

	MapEntryPanel(Panel* parent, const char* mapName, const char* iconPath);
	const char* GetMapName() const { return m_mapName; }

protected:
	virtual void OnMousePressed(MouseCode code);
	virtual void OnMouseDoublePressed(MouseCode code);

private:
	char m_mapName[MAX_PATH];
	Label* m_pMapLabel;
	ImagePanel* m_pMapIcon;
};

MapEntryPanel::MapEntryPanel(Panel* parent, const char* mapName, const char* iconPath)
	: Panel(parent, mapName)
{
	Q_strncpy(m_mapName, mapName, sizeof(m_mapName));

	// Create the map label
	m_pMapLabel = new Label(this, "MapLabel", mapName);
	m_pMapLabel->SetPos(15, 127); // Position label below the icon
	m_pMapLabel->SetSize(148, 20); // Label size

	// Create the map icon (thumbnail) with adjusted size for 128x128 images
	m_pMapIcon = new ImagePanel(this, "MapIcon");
	m_pMapIcon->SetPos(15, 0); // Position icon
	m_pMapIcon->SetSize(128, 128); // Set icon size to 128x128

	// Load the map icon or fallback to the default
	if (g_pFullFileSystem->FileExists(iconPath, "GAME"))
	{
		m_pMapIcon->SetImage(iconPath); // Set the custom icon for the map
	}
	else
	{
		// Fallback icon if no custom icon is found
		m_pMapIcon->SetImage("maps/no_ico.vmt");
	}

	// Force repaint of the panel to make sure the image fits within the set bounds
	m_pMapIcon->InvalidateLayout(true);
}

void MapEntryPanel::OnMousePressed(MouseCode code)
{
	if (code == MOUSE_LEFT)
	{
		// Single-click: Notify parent panel of map selection
		KeyValues* event = new KeyValues("MapSelected");
		event->SetString("mapName", m_mapName);
		PostActionSignal(event);
	}
}

void MapEntryPanel::OnMouseDoublePressed(MouseCode code)
{
	if (code == MOUSE_LEFT)
	{
		// Double-click: Notify parent panel to start the map immediately
		KeyValues* event = new KeyValues("MapDoubleClicked");
		event->SetString("mapName", m_mapName);
		PostActionSignal(event);
	}
}


class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);

	CMyPanel(vgui::VPANEL parent);
	//~CMyPanel();

protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);
	virtual void OnActionSignal(KeyValues* params);  // Catch signals for map actions

private:
	Panel* m_pMapList;
	Button* m_pStartButton;
	char m_selectedMap[MAX_PATH];

	void PopulateMapList();
	void StartMap();
	void OnMapSelected(const char* mapName);
	void OnMapDoubleClicked(const char* mapName);
};

CMyPanel::CMyPanel(vgui::VPANEL parent)
	: BaseClass(nullptr, "MyPanel")
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
	LoadControlSettings("resource/UI/singleplayer.res");

	m_pMapList = new Panel(this, "MapList");
	m_pMapList->SetPos(121, 23);
	m_pMapList->SetSize(750, 750); // Adjust to the size of your panel

	m_pStartButton = new Button(this, "StartButton", "Start", this, "startmap");
	m_pStartButton->SetPos(1021, 763);

	PopulateMapList();
}

void CMyPanel::OnActionSignal(KeyValues* params)
{
	// Handle map selection and double-click signals
	const char* command = params->GetName();  // Get the name of the signal

	if (!Q_stricmp(command, "MapSelected"))
	{
		const char* mapName = params->GetString("mapName", "");
		OnMapSelected(mapName);
	}
	else if (!Q_stricmp(command, "MapDoubleClicked"))
	{
		const char* mapName = params->GetString("mapName", "");
		OnMapDoubleClicked(mapName);
	}
}

void CMyPanel::OnMapSelected(const char* mapName)
{
	Q_strncpy(m_selectedMap, mapName, sizeof(m_selectedMap));
	// Optionally provide feedback here that the map is selected
	Msg("Map selected: %s\n", mapName);
}

void CMyPanel::OnMapDoubleClicked(const char* mapName)
{
	Q_strncpy(m_selectedMap, mapName, sizeof(m_selectedMap));
	StartMap(); // Start the map immediately on double-click
}

void CMyPanel::StartMap()
{
	if (m_selectedMap[0] != '\0')
	{
		char cmd[MAX_PATH];
		Q_snprintf(cmd, sizeof(cmd), "map %s\n", m_selectedMap);
		engine->ClientCmd(cmd);
	}
}

void CMyPanel::PopulateMapList()
{
	const char* mapsDir = "maps/*.bsp";
	FileFindHandle_t findHandle;
	const char* fileName = g_pFullFileSystem->FindFirst(mapsDir, &findHandle);

	int x = 10, y = 10; // Starting position for map entries
	while (fileName)
	{
		if (strstr(fileName, ".bsp"))
		{
			char mapName[MAX_PATH];
			Q_strncpy(mapName, fileName, sizeof(mapName));
			mapName[strlen(mapName) - 4] = '\0'; // Remove ".bsp"

			char iconPath[MAX_PATH];
			// Explicitly define the path here (materials/maps/%s.vmt) without "vgui" getting involved
			Q_snprintf(iconPath, sizeof(iconPath), "maps/%s.vmt", mapName); // This should now point to the correct location

			// Create a new map entry panel with the map name and icon path
			MapEntryPanel* entry = new MapEntryPanel(m_pMapList, mapName, iconPath);
			entry->SetBounds(x, y, 148, 158); // Position and size for each entry
			entry->AddActionSignalTarget(this);  // Target this panel for action signals

			// Make sure the entry doesn't overlap and calculate the position of the next entry
			x += 185; // Move to the next column (width of icon + some padding)
			if (x + 120 > 720) // If it exceeds the width, move to the next row
			{
				x = 10;
				y += 180;  // Move down (height of icon + label)
			}
		}

		fileName = g_pFullFileSystem->FindNext(findHandle);
	}

	g_pFullFileSystem->FindClose(findHandle);
}


void CMyPanel::OnCommand(const char* pcCommand)
{
	if (!Q_stricmp(pcCommand, "startmap"))
	{
		StartMap();
	}
}

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(true);
}


void CMyPanelInterface::Create(vgui::VPANEL parent)
{
	if (!m_pPanel)
	{
		m_pPanel = new CMyPanel(parent);
	}
}

void CMyPanelInterface::Destroy()
{
	if (m_pPanel)
	{
		m_pPanel->SetParent(nullptr);
		delete m_pPanel;
		m_pPanel = nullptr;
	}
}

void CMyPanelInterface::Activate()
{
	if (m_pPanel)
	{
		m_pPanel->Activate();
	}
}

CON_COMMAND(srcbox_map_menu, "Map Selection for Srcbox")
{
	if (!mypanel)
		return;

	static bool isVisible = false;
	isVisible = !isVisible;

	if (isVisible)
	{
		mypanel->Activate();
	}
	else
	{
		g_MyPanel.Destroy();
	}
}*/

/*#include "cbase.h"
#include "IMyPanel.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include <KeyValues.h>

// Forward declaration for map selection logic
void OnMapSelected(const char* mapName);
void StartSelectedMap(const char* mapName);

// MapEntryPanel: Represents individual map icons
class MapEntryPanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(MapEntryPanel, vgui::Panel);

public:
	MapEntryPanel(vgui::Panel* parent, const char* mapName, const char* iconMaterial)
		: vgui::Panel(parent, mapName)
	{
		m_pMapName = new vgui::Label(this, "MapName", mapName);
		m_pMapIcon = new vgui::ImagePanel(this, "MapIcon");

		// Set the material for the icon
		KeyValues* kv = new KeyValues("SetImage");
		kv->SetString("image", iconMaterial);
		m_pMapIcon->ApplySettings(kv); // Use KeyValues to set the image path dynamically

		m_pMapName->SetText(mapName);

		SetBounds(0, 0, 150, 180); // Adjust size
		m_pMapName->SetBounds(10, 140, 130, 30); // Position for map name
		m_pMapIcon->SetBounds(10, 10, 130, 120); // Position for map icon
	}

	virtual void OnMousePressed(vgui::MouseCode code) override
	{
		if (code == MOUSE_LEFT)
		{
			KeyValues* msg = new KeyValues("Command", "command", "MapSelected");
			//msg->SetString("mapName", m_pMapName->GetText());
			PostActionSignal(msg);
		}
		//else if (code == MOUSE_DOUBLE)
		//{
		//	KeyValues* msg = new KeyValues("Command", "command", "MapDoubleClicked");
		//	msg->SetString("mapName", m_pMapName->GetText());
		//	PostActionSignal(msg);
		//}
	}

private:
	vgui::Label* m_pMapName;
	vgui::ImagePanel* m_pMapIcon;
};

// CMyPanel class: Handles the main panel logic
class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);

	CMyPanel(vgui::VPANEL parent); // Constructor
	~CMyPanel() {}; // Destructor

protected:
	// VGUI overrides:
	virtual void OnCommand(const char* pcCommand) override;
	virtual void OnTick() override;

private:
	void PopulateMapList(); // Populate map entries
	void OnMapSelected(const char* mapName); // Handle selection
	void OnMapDoubleClicked(const char* mapName); // Handle double click

	vgui::Panel* m_pMapList; // Container for map entries
	const char* m_SelectedMap; // Current selected map
};

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

	m_pMapList = new vgui::Panel(this, "MapList");
	m_pMapList->SetBounds(10, 40, 700, 500); // Adjust dimensions for the map list
	m_pMapList->SetVisible(true);

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	m_SelectedMap = nullptr;

	DevMsg("MyPanel has been constructed\n");
	PopulateMapList(); // Populate the map entries
}

void CMyPanel::PopulateMapList()
{
	// Example hardcoded maps for testing
	const char* maps[] = { "gm_construct", "gm_flatgrass" };
	const char* icons[] = { "vgui/maps/gm_construct", "vgui/maps/gm_flatgrass" }; // Example materials

	int x = 10, y = 10;
	for (int i = 0; i < 2; ++i) // Replace with dynamic loading if needed
	{
		MapEntryPanel* entry = new MapEntryPanel(m_pMapList, maps[i], icons[i]);
		entry->SetBounds(x, y, 150, 180);
		entry->AddActionSignalTarget(this); // Send signals to the main panel

		x += 170; // Move to the next column
		if (x + 150 > 700) // Wrap to next row
		{
			x = 10;
			y += 190;
		}
	}
}

void CMyPanel::OnMapSelected(const char* mapName)
{
	m_SelectedMap = mapName;
	DevMsg("Map selected: %s\n", mapName);
}

void CMyPanel::OnMapDoubleClicked(const char* mapName)
{
	OnMapSelected(mapName);
	OnCommand("startmap");
}

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);

	if (!Q_stricmp(pcCommand, "startmap") && m_SelectedMap)
	{
		// Start the selected map
		StartSelectedMap(m_SelectedMap);
	}
}

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	//SetVisible(cl_showmypanel.GetBool()); // Show/hide based on console variable
}

// Class: CMyPanelInterface
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel* MyPanel;

public:
	CMyPanelInterface() { MyPanel = NULL; }

	void Create(vgui::VPANEL parent)
	{
		MyPanel = new CMyPanel(parent);
	}

	void Destroy()
	{
		if (MyPanel)
		{
			MyPanel->SetParent((vgui::Panel*)NULL);
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

CON_COMMAND(ToggleMyPanel, "Toggles myPanel on or off")
{
	//cl_showmypanel.SetValue(!cl_showmypanel.GetBool());
}*/


/*#include "cbase.h"
#include "IMyPanel.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/ListPanel.h>
#include <filesystem.h>
#include <KeyValues.h>

using namespace vgui;

class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);

public:
	CMyPanel(vgui::VPANEL parent);
	~CMyPanel() {}

protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand) override;

private:
	PropertySheet* m_pTabPanel;
	ListPanel* m_pBrowseAllList;

	void PopulateBrowseAll();
};

CMyPanel::CMyPanel(vgui::VPANEL parent)
	: BaseClass(nullptr, "MyPanel")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(false);
	SetTitleBarVisible(true);
	SetSizeable(false);
	SetMoveable(true);
	SetTitle("Play Singleplayer", false);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/UI/singleplayer_srcbox.res");

	// Create PropertySheet (Tab Panel)
	m_pTabPanel = new PropertySheet(this, "TabPanel");
	m_pTabPanel->SetBounds(10, 40, 456, 714); // Adjust to fit your window dimensions

	// Placeholder for "Game" Tab
	Panel* pGamePanel = new Panel(m_pTabPanel, "GamePanel");
	pGamePanel->SetPaintBackgroundEnabled(true);
	pGamePanel->SetBgColor(Color(50, 50, 50, 255)); // Background color for the "Game" tab
	m_pTabPanel->AddPage(pGamePanel, "Game");

	// "Browse All" Tab
	m_pBrowseAllList = new ListPanel(m_pTabPanel, "BrowseAllList");
	m_pBrowseAllList->SetPaintBackgroundEnabled(true);
	m_pBrowseAllList->SetBgColor(Color(50, 50, 50, 255));

	// Define the columns for the table
	m_pBrowseAllList->AddColumnHeader(0, "MapName", "Map Name", 350, ListPanel::COLUMN_FIXEDSIZE);
	m_pBrowseAllList->AddColumnHeader(1, "Game", "Game", 100, ListPanel::COLUMN_FIXEDSIZE);

	m_pTabPanel->AddPage(m_pBrowseAllList, "Browse All");

	// Populate "Browse All" Tab
	PopulateBrowseAll();

	SetVisible(true);
}

void CMyPanel::PopulateBrowseAll()
{
	m_pBrowseAllList->RemoveAll(); // Clear any existing entries

	// Scan maps directory
	const char* mapsDir = "maps/*.bsp";
	FileFindHandle_t findHandle;
	const char* fileName = g_pFullFileSystem->FindFirst(mapsDir, &findHandle);

	while (fileName)
	{
		if (strstr(fileName, ".bsp"))
		{
			// Remove the ".bsp" extension
			char mapName[MAX_PATH];
			Q_strncpy(mapName, fileName, sizeof(mapName));
			mapName[strlen(mapName) - 4] = '\0';

			// Create a row for the ListPanel
			KeyValues* kv = new KeyValues("MapData");
			kv->SetString("MapName", mapName);
			kv->SetString("Game", "Srcbox"); // Hardcoded for now

			m_pBrowseAllList->AddItem(kv, 0, false, false); // Add the map as a new row
			kv->deleteThis(); // Clean up
		}

		fileName = g_pFullFileSystem->FindNext(findHandle);
	}

	g_pFullFileSystem->FindClose(findHandle);
}

ConVar srcbox_map_menu("srcbox_map_menu", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);

	if (!Q_stricmp(pcCommand, "turnoff"))
		srcbox_map_menu.SetValue(0);

	// Handle additional commands here
}

// Class for managing panel instance
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel* m_pPanel;

public:
	CMyPanelInterface()
		: m_pPanel(nullptr) {}

	void Create(vgui::VPANEL parent) override
	{
		if (!m_pPanel)
		{
			m_pPanel = new CMyPanel(parent);
		}
	}

	void Destroy() override
	{
		if (m_pPanel)
		{
			m_pPanel->SetParent(nullptr);
			delete m_pPanel;
			m_pPanel = nullptr;
		}
	}

	void Activate() override
	{
		if (m_pPanel)
		{
			m_pPanel->Activate();
		}
	}
};

static CMyPanelInterface g_MyPanel;
IMyPanel* mypanel = (IMyPanel*)&g_MyPanel;

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(srcbox_map_menu.GetBool());
}

CON_COMMAND(srcbox_singleplayer, "Toggle the panel for Singleplayer")
{
	srcbox_map_menu.SetValue(!srcbox_map_menu.GetBool());
	mypanel->Activate();
}*/

#include "cbase.h"
#include "IMyPanel.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/ListPanel.h>
#include <filesystem.h>
#include <KeyValues.h>
#include "ienginevgui.h"
#include <vgui_controls/PanelListPanel.h>
#include <vgui_controls/ImagePanel.h>

using namespace vgui;

class CMyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame);

public:
	CMyPanel(vgui::VPANEL parent);
	~CMyPanel() {}

protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand) override;

private:
	PropertySheet* m_pTabPanel;
	ListPanel* m_pBrowseAllList;
	Button* m_pPlayButton;
	PanelListPanel* m_pBrowseAllPanel; // For "Browse All" tab
	PanelListPanel* m_pGameMapPanel;  // For "Game" tab to hold icons

	void PopulateBrowseAll();
	void PopulateGameTab();
	void PlaySelectedMap(); // Function to handle playing the selected map
	void CreateGameIcon(const char* mapName, const char* imagePath, const char* command);
};

CMyPanel::CMyPanel(vgui::VPANEL parent)
	: BaseClass(nullptr, "MyPanel")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(false);
	SetTitleBarVisible(true);
	SetSizeable(false);
	SetMoveable(true);
	SetTitle("Play Singleplayer", false);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/UI/singleplayer_srcbox.res");

	// Create PropertySheet (Tab Panel)
	m_pTabPanel = new PropertySheet(this, "TabPanel");
	m_pTabPanel->SetBounds(10, 40, 456, 714); // Adjust to fit your window dimensions

	// Placeholder for "Game" Tab
	Panel* pGamePanel = new Panel(m_pTabPanel, "GamePanel");
	pGamePanel->SetPaintBackgroundEnabled(true);
	pGamePanel->SetBgColor(Color(50, 50, 50, 255)); // Background color for the "Game" tab

	// Container for map icons in the Game tab
	m_pGameMapPanel = new PanelListPanel(pGamePanel, "GameMapPanel");
	m_pGameMapPanel->SetBounds(10, 10, 430, 700); // Adjust to fit your window dimensions
	m_pGameMapPanel->SetFirstColumnWidth(200); // Layout adjustments

	// Add the Game tab to the PropertySheet
	//m_pTabPanel->AddPage(pGamePanel, "Game");

	// "Game" Tab
	PopulateGameTab();
	m_pTabPanel->AddPage(m_pGameMapPanel, "Game");

	// "Browse All" Tab
	m_pBrowseAllList = new ListPanel(m_pTabPanel, "BrowseAllList");
	m_pBrowseAllList->SetPaintBackgroundEnabled(true);
	m_pBrowseAllList->SetBgColor(Color(50, 50, 50, 255));

	// Define the columns for the table
	m_pBrowseAllList->AddColumnHeader(0, "MapName", "Map Name", 350, ListPanel::COLUMN_FIXEDSIZE);
	m_pBrowseAllList->AddColumnHeader(1, "Game", "Game", 100, ListPanel::COLUMN_FIXEDSIZE);

	m_pTabPanel->AddPage(m_pBrowseAllList, "Browse All");

	// Populate "Browse All" Tab
	PopulateBrowseAll();


	// Add the Play button
	m_pPlayButton = new Button(this, "PlayButton", "Play");
	m_pPlayButton->SetCommand("play_selected_map");
	m_pPlayButton->SetBounds(300, 760, 64, 24); // Adjust position and size
	m_pPlayButton->SetVisible(true);

	SetVisible(true);
}

void CMyPanel::PopulateBrowseAll()
{
	m_pBrowseAllList->RemoveAll(); // Clear any existing entries

	// Define prefix categories
	struct GameCategory
	{
		const char* gameName;
		const char* prefixes[30]; // Up to 10 prefixes per category (adjust as needed)
	};

	GameCategory categories[] = {
		{ "Gmod", { "gm_", nullptr } },
		{ "TF2", { "ctf_", "koth_", "cp_", "pl_", "mvm_", "pass_", "pd_", "sd_", "tc_", "vsh_", "zi_", "tr_", nullptr } },
		{ "CS:S", { "de_", "cs_", nullptr } },
		{ "HL2 Beta", { "d1_garage_", nullptr } },
		{ "HL:S", { "t0", nullptr } },
		{ "HL2", { "d1_trainstation", "d2_", "c17_", "ep1_", "ep1_", nullptr } },
		{ "HL2:DM", { "dm_", nullptr } },
		{ "L4D", { "l4d_", nullptr } },
		{ "L4D2", { "c1m1_hotel", "c1m2_streets", "c1m3_mall", "c2m1_highway", nullptr } },
		{ "Unknown Game", { nullptr } } // Default fallback
	};

	// Scan maps directory
	const char* mapsDir = "maps/*.bsp";
	FileFindHandle_t findHandle;
	const char* fileName = g_pFullFileSystem->FindFirst(mapsDir, &findHandle);

	while (fileName)
	{
		if (strstr(fileName, ".bsp"))
		{
			// Remove the ".bsp" extension
			char mapName[MAX_PATH];
			Q_strncpy(mapName, fileName, sizeof(mapName));
			mapName[strlen(mapName) - 4] = '\0';

			// Determine the game category based on prefixes
			const char* detectedGame = "Srcbox";
			for (const auto& category : categories)
			{
				for (int i = 0; category.prefixes[i] != nullptr; ++i)
				{
					if (Q_strnicmp(mapName, category.prefixes[i], strlen(category.prefixes[i])) == 0)
					{
						detectedGame = category.gameName;
						break;
					}
				}
				//if (strcmp(detectedGame, "Unknown Game") != 0)
					//break; // Stop searching if we found a match
			}

			// Create a row for the ListPanel
			KeyValues* kv = new KeyValues("MapData");
			kv->SetString("MapName", mapName);
			kv->SetString("Game", detectedGame);

			m_pBrowseAllList->AddItem(kv, 0, false, false); // Add the map as a new row
			kv->deleteThis(); // Clean up
		}

		fileName = g_pFullFileSystem->FindNext(findHandle);
	}

	g_pFullFileSystem->FindClose(findHandle);
}

void CMyPanel::PopulateGameTab()
{
	m_pGameMapPanel->DeleteAllItems(); // Clear previous entries

	// Scan maps directory for ".bsp" files
	const char* mapsDir = "maps/*.bsp";
	FileFindHandle_t findHandle;
	const char* fileName = g_pFullFileSystem->FindFirst(mapsDir, &findHandle);

	// Clear previous content
	if (m_pGameMapPanel)
	{
		m_pGameMapPanel->DeletePanel();
	}

	// Create the Game Panel to hold map icons
	m_pGameMapPanel = new vgui::PanelListPanel(m_pTabPanel, "GameMapPanel");
	m_pGameMapPanel->SetBounds(10, 40, 456, 714);
	m_pGameMapPanel->SetPaintBackgroundEnabled(true);
	m_pGameMapPanel->SetBgColor(Color(50, 50, 50, 255));

	// Add example maps
	CreateGameIcon("gm_construct", "maps/gm_construct", "map gm_construct");
	CreateGameIcon("gm_flatgrass", "maps/gm_flatgrass", "map gm_flatgrass");

	while (fileName)
	{
		if (strstr(fileName, ".bsp"))
		{
			// Remove the ".bsp" extension
			char mapName[MAX_PATH];
			Q_strncpy(mapName, fileName, sizeof(mapName));
			mapName[strlen(mapName) - 4] = '\0';

			// Create a container panel for each map entry
			Panel* pMapContainer = new Panel(m_pGameMapPanel, mapName);
			pMapContainer->SetSize(200, 200); // Adjust icon container size

			// Add a clickable ImagePanel for the map thumbnail
			ImagePanel* pImagePanel = new ImagePanel(pMapContainer, "MapThumbnail");
			pImagePanel->SetBounds(5, 5, 190, 120); // Adjust image size
			pImagePanel->SetImage("maps/no_ico"); // Use a default thumbnail

			// Set the map name below the thumbnail
			Label* pMapLabel = new Label(pMapContainer, "MapNameLabel", mapName);
			pMapLabel->SetBounds(5, 130, 190, 20);
			pMapLabel->SetContentAlignment(Label::a_center);

			// Make the panel respond to clicks
			//pImagePanel->SetCommand(new KeyValues("StartMap", "map", mapName));
			pImagePanel->AddActionSignalTarget(this);

			// Add container to the GameMapPanel
			m_pGameMapPanel->AddItem(nullptr, pMapContainer);

			pMapContainer->InvalidateLayout(true, true);
		}

		fileName = g_pFullFileSystem->FindNext(findHandle);
	}

	g_pFullFileSystem->FindClose(findHandle);
}

// Start Game icon stuff

void CMyPanel::CreateGameIcon(const char* mapName, const char* imagePath, const char* command)
{
	// Create a Button that acts as the clickable area
	vgui::Button* pImageButton = new vgui::Button(m_pGameMapPanel, mapName, "");
	pImageButton->SetSize(160, 90); // Set the size of the button
	pImageButton->SetCommand(command);
	pImageButton->AddActionSignalTarget(this);

	// Add an ImagePanel inside the button to display the image
	vgui::ImagePanel* pImage = new vgui::ImagePanel(pImageButton, "GameImage");
	pImage->SetImage(imagePath);
	pImage->SetShouldScaleImage(true);
	pImage->SetSize(160, 90); // Set the image size same as button
}



void CMyPanel::PlaySelectedMap()
{
	// Get the selected item from the list
	int selectedItemID = m_pBrowseAllList->GetSelectedItem(0); // Get the first selected item
	if (selectedItemID == -1)
	{
		// No map selected
		Msg("No map selected.\n");
		return;
	}

	// Retrieve the map name from the selected item
	KeyValues* kv = m_pBrowseAllList->GetItem(selectedItemID);
	const char* mapName = kv->GetString("MapName");

	// Construct the map load command
	char command[256];
	Q_snprintf(command, sizeof(command), "map %s\n", mapName);

	// Execute the command to load the map
	engine->ClientCmd(command);
}

ConVar srcbox_map_menu("srcbox_map_menu", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);

	if (FStrEq(pcCommand, "play_selected_map"))
	{
		PlaySelectedMap(); // Handle the Play button command
	}
	else if (FStrEq(pcCommand, "turnoff"))
	{
		srcbox_map_menu.SetValue(0);
	}
}

// Class for managing panel instance
class CMyPanelInterface : public IMyPanel
{
private:
	CMyPanel* m_pPanel;

public:
	CMyPanelInterface()
		: m_pPanel(nullptr) {}

	void Create(vgui::VPANEL parent) override
	{
		if (!m_pPanel)
		{
			m_pPanel = new CMyPanel(parent);
		}
	}

	void Destroy() override
	{
		if (m_pPanel)
		{
			m_pPanel->SetParent(nullptr);
			delete m_pPanel;
			m_pPanel = nullptr;
		}
	}

	void Activate() override
	{
		if (m_pPanel)
		{
			m_pPanel->Activate();
		}
	}
};

static CMyPanelInterface g_MyPanel;
IMyPanel* mypanel = (IMyPanel*)&g_MyPanel;

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(srcbox_map_menu.GetBool());
}

CON_COMMAND(srcbox_singleplayer, "Toggle the panel for Singleplayer")
{
	srcbox_map_menu.SetValue(!srcbox_map_menu.GetBool());
	mypanel->Activate();
};

































