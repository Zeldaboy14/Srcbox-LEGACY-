#include "cbase.h"
#include "vgui_controls/SectionedListPanel.h"
#include "vgui_controls/ImagePanel.h"
#include "engine/IEngineSound.h"
#include "filesystem.h"
#include "icommandline.h"
#include "music_system.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

int m_nSongGuid;

ConVar mainmenu_music("mainmenu_music", "1", FCVAR_ARCHIVE, "Toggle music in the main menu");

CPFMainMenuMusic music;


void CPFMainMenuMusic::GetRandomMusic(char* pszBuf, int iBufLength)
{
	Assert(iBufLength);

	char szPath[MAX_PATH];

	// Check that there's music available
	if (!g_pFullFileSystem->FileExists("sound/music/srcbox/srcbox_menu1.wav"))
	{
		Assert(false);
		*pszBuf = '\0';
	}

	// Discover tracks, 1 through n
	int iLastTrack = 0;
	do
	{
		Q_snprintf(szPath, sizeof(szPath), "sound/music/srcbox/srcbox_menu%d.wav", ++iLastTrack);
	} while (g_pFullFileSystem->FileExists(szPath));

	// Pick a random one
	Q_snprintf(szPath, sizeof(szPath), "*#music/srcbox/srcbox_menu%d.wav", RandomInt(1, iLastTrack - 1));
	Q_strncpy(pszBuf, szPath, iBufLength);
}

void CPFMainMenuMusic::Init()
{

	m_psMusicStatus = MUSIC_FIND;
	m_pzMusicLink[0] = '\0';
	m_nSongGuid = 0;

}



void CPFMainMenuMusic::OnTick()
{

	if (mainmenu_music.GetBool() && !engine->IsConnected() || mainmenu_music.GetBool() && engine->IsLevelMainMenuBackground())
	{
		if ((m_psMusicStatus == MUSIC_FIND || m_psMusicStatus == MUSIC_STOP_FIND) && !enginesound->IsSoundStillPlaying(m_nSongGuid))
		{
			GetRandomMusic(m_pzMusicLink, sizeof(m_pzMusicLink));
			m_psMusicStatus = MUSIC_PLAY;
		}
		else if ((m_psMusicStatus == MUSIC_PLAY || m_psMusicStatus == MUSIC_STOP_PLAY) && m_pzMusicLink[0] != '\0')
		{
			enginesound->StopSoundByGuid(m_nSongGuid);
			ConVar* snd_musicvolume = cvar->FindVar("snd_musicvolume");
			float fVolume = (snd_musicvolume ? snd_musicvolume->GetFloat() : 1.0f);
			enginesound->EmitAmbientSound(m_pzMusicLink, fVolume, PITCH_NORM, 0);
			m_nSongGuid = enginesound->GetGuidForLastSoundEmitted();
			m_psMusicStatus = MUSIC_FIND;
		}
	}
	else if (m_psMusicStatus == MUSIC_FIND)
	{
		enginesound->StopSoundByGuid(m_nSongGuid);
		m_psMusicStatus = (m_nSongGuid == 0 ? MUSIC_STOP_FIND : MUSIC_STOP_PLAY);
	}
}

void RandomMusicCommand()
{
	enginesound->StopSoundByGuid(m_nSongGuid);
}

ConCommand randommusic("randommusic", RandomMusicCommand , "play random music");