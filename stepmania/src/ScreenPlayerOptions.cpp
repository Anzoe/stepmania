#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: ScreenPlayerOptions

 Desc: Select a song.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "ScreenPlayerOptions.h"
#include "RageTextureManager.h"
#include "RageUtil.h"
#include "RageMusic.h"
#include "ScreenManager.h"
#include "GameConstantsAndTypes.h"
#include "PrefsManager.h"
#include "RageLog.h"
#include "GameManager.h"
#include "GameState.h"
#include "RageSound.h"


enum {
	PO_SPEED = 0,
	PO_EFFECT,
	PO_APPEAR,
	PO_TURN,
	PO_LITTLE,
	PO_SCROLL,
	PO_COLOR,
	PO_HOLD_NOTES,
	PO_DARK,
	NUM_PLAYER_OPTIONS_LINES
};
OptionLineData g_PlayerOptionsLines[NUM_PLAYER_OPTIONS_LINES] = {
	{ "Speed",	9, {"x0.5","x0.75","x1","x1.5","x2","x3","x4","x5","x8"} },	
	{ "Effect", 7, {"OFF","BOOST","WAVE", "DRUNK", "DIZZY","SPACE","MINI"} },	
	{ "Appear", 4, {"VISIBLE","HIDDEN","SUDDEN","STEALTH"} },	
	{ "Turn",	5, {"OFF","MIRROR","LEFT","RIGHT","SHUFFLE"} },	
	{ "Little", 2, {"OFF","ON"} },	
	{ "Scroll", 2, {"STANDARD","REVERSE"} },	
	{ "Color",	4, {"VIVID","NOTE","FLAT","PLAIN"} },	
	{ "Holds",	2, {"OFF","ON"} },	
	{ "Dark",	2, {"OFF","ON"} },	
};


ScreenPlayerOptions::ScreenPlayerOptions() :
	ScreenOptions(
		THEME->GetPathTo("Graphics","player options background"),
		THEME->GetPathTo("Graphics","player options page"),
		THEME->GetPathTo("Graphics","player options top edge")
		)
{
	LOG->Trace( "ScreenPlayerOptions::ScreenPlayerOptions()" );
	
	Init( 
		INPUTMODE_PLAYERS, 
		g_PlayerOptionsLines, 
		NUM_PLAYER_OPTIONS_LINES
		);

	SOUND->PlayOnceStreamedFromDir( ANNOUNCER->GetPathTo("player options intro") );
}


void ScreenPlayerOptions::ImportOptions()
{
	for( int p=0; p<NUM_PLAYERS; p++ )
	{
		PlayerOptions &po = GAMESTATE->m_PlayerOptions[p];
		
		if(		 po.m_fArrowScrollSpeed == 0.5f )	m_iSelectedOption[p][PO_SPEED] = 0;
		else if( po.m_fArrowScrollSpeed == 0.75f)	m_iSelectedOption[p][PO_SPEED] = 1;
		else if( po.m_fArrowScrollSpeed == 1.0f )	m_iSelectedOption[p][PO_SPEED] = 2;
		else if( po.m_fArrowScrollSpeed == 1.5f )	m_iSelectedOption[p][PO_SPEED] = 3;
		else if( po.m_fArrowScrollSpeed == 2.0f )	m_iSelectedOption[p][PO_SPEED] = 4;
		else if( po.m_fArrowScrollSpeed == 3.0f )	m_iSelectedOption[p][PO_SPEED] = 5;
		else if( po.m_fArrowScrollSpeed == 4.0f )	m_iSelectedOption[p][PO_SPEED] = 6;
		else if( po.m_fArrowScrollSpeed == 5.0f )	m_iSelectedOption[p][PO_SPEED] = 7;
		else if( po.m_fArrowScrollSpeed == 8.0f )	m_iSelectedOption[p][PO_SPEED] = 8;
		else										m_iSelectedOption[p][PO_SPEED] = 2;

		switch(po.m_EffectType) 
		{
		case PlayerOptions::EFFECT_NONE:	m_iSelectedOption[p][PO_EFFECT]=0; break;
		case PlayerOptions::EFFECT_BOOST:	m_iSelectedOption[p][PO_EFFECT]=1; break;
		case PlayerOptions::EFFECT_WAVE:	m_iSelectedOption[p][PO_EFFECT]=2; break;
		case PlayerOptions::EFFECT_DRUNK:	m_iSelectedOption[p][PO_EFFECT]=3; break;
		case PlayerOptions::EFFECT_DIZZY:	m_iSelectedOption[p][PO_EFFECT]=4; break;
		case PlayerOptions::EFFECT_SPACE:	m_iSelectedOption[p][PO_EFFECT]=5; break;
		case PlayerOptions::EFFECT_MINI:	m_iSelectedOption[p][PO_EFFECT]=6; break;
		default:			; break;
		}

		m_iSelectedOption[p][PO_APPEAR]		= po.m_AppearanceType;
		m_iSelectedOption[p][PO_TURN]		= po.m_TurnType;
		m_iSelectedOption[p][PO_LITTLE]		= po.m_bLittle ? 1 : 0;
		m_iSelectedOption[p][PO_SCROLL]		= po.m_bReverseScroll ? 1 : 0 ;
		m_iSelectedOption[p][PO_COLOR]		= po.m_ColorType;
		m_iSelectedOption[p][PO_HOLD_NOTES]	= po.m_bHoldNotes ? 1 : 0;
		m_iSelectedOption[p][PO_DARK]		= po.m_bDark ? 1 : 0;
	}
}

void ScreenPlayerOptions::ExportOptions()
{
	for( int p=0; p<NUM_PLAYERS; p++ )
	{
		PlayerOptions &po = GAMESTATE->m_PlayerOptions[p];

		switch( m_iSelectedOption[p][PO_SPEED] )
		{
		case 0:	po.m_fArrowScrollSpeed = 0.5f;	break;
		case 1:	po.m_fArrowScrollSpeed = 0.75f;	break;
		case 2:	po.m_fArrowScrollSpeed = 1.0f;	break;
		case 3:	po.m_fArrowScrollSpeed = 1.5f;	break;
		case 4:	po.m_fArrowScrollSpeed = 2.0f;	break;
		case 5:	po.m_fArrowScrollSpeed = 3.0f;	break;
		case 6:	po.m_fArrowScrollSpeed = 4.0f;	break;
		case 7:	po.m_fArrowScrollSpeed = 5.0f;	break;
		case 8:	po.m_fArrowScrollSpeed = 8.0f;	break;
		}

		switch(m_iSelectedOption[p][PO_EFFECT])
		{
		case 0: po.m_EffectType = PlayerOptions::EFFECT_NONE; break;
		case 1: po.m_EffectType = PlayerOptions::EFFECT_BOOST; break;
		case 2: po.m_EffectType = PlayerOptions::EFFECT_WAVE; break;
		case 3: po.m_EffectType = PlayerOptions::EFFECT_DRUNK; break;
		case 4: po.m_EffectType = PlayerOptions::EFFECT_DIZZY; break;
		case 5: po.m_EffectType = PlayerOptions::EFFECT_SPACE; break;
		case 6: po.m_EffectType = PlayerOptions::EFFECT_MINI; break;
		}

		po.m_AppearanceType	= (PlayerOptions::AppearanceType)m_iSelectedOption[p][PO_APPEAR];
		po.m_TurnType		= (PlayerOptions::TurnType)m_iSelectedOption[p][PO_TURN];
		po.m_bLittle		= m_iSelectedOption[p][PO_LITTLE] == 1;
		po.m_bReverseScroll	= (m_iSelectedOption[p][PO_SCROLL] == 1);
		po.m_ColorType		= (PlayerOptions::ColorType)m_iSelectedOption[p][PO_COLOR];
		po.m_bHoldNotes		= (m_iSelectedOption[p][PO_HOLD_NOTES] == 1);
		po.m_bDark			= (m_iSelectedOption[p][PO_DARK] == 1);
	}
}

void ScreenPlayerOptions::GoToPrevState()
{
	if(GAMESTATE->m_PlayMode == PLAY_MODE_ONI ||
		GAMESTATE->m_PlayMode == PLAY_MODE_ENDLESS)
		SCREENMAN->SetNewScreen( "ScreenSelectCourse" );
	else
		SCREENMAN->SetNewScreen( "ScreenSelectMusic" );
}

void ScreenPlayerOptions::GoToNextState()
{
	SCREENMAN->SetNewScreen( "ScreenSongOptions" );
}



