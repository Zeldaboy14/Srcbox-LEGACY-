#pragma once
#include "cbase.h"
#include "vgui_basepanel.h"


enum MusicStatus
{
	MUSIC_STOP,
	MUSIC_FIND,
	MUSIC_PLAY,
	MUSIC_STOP_FIND,
	MUSIC_STOP_PLAY,
};

class CPFMainMenuMusic
{
public:
	void Init();
	void OnTick();
	void RandomMusicCommand();
	//int					m_nSongGuid;

private:
	void GetRandomMusic(char* pszBuf, int iBufLength);

	char				m_pzMusicLink[64];
	
	MusicStatus			m_psMusicStatus;
	int					m_iPlayGameStartupSound = 0;

};
