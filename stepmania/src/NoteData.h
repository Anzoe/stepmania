#ifndef NOTEDATA_H
#define NOTEDATA_H
/*
-----------------------------------------------------------------------------
 File: NoteData.h

 Desc: Holds data about the notes that the player is supposed to hit.  NoteData
	is organized by:
	track - corresponds to different columns of notes on the screen
	row/index - corresponds to subdivisions of beats

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

//#include "GameConstantsAndTypes.h"
#include "NoteTypes.h"

// '1' = tap note
// '2' = hold note begin
// '3' = hold note end  ('1' can also end a HoldNote) ('3' without a matching '2' is ignored
// ... for future expansion

class NoteData
{
	/* Keep this aligned, so that they all have the same size. */
	vector<TapNote> m_TapNotes[MAX_NOTE_TRACKS];
	int m_iNumTracks;

	vector<HoldNote>	m_HoldNotes;

	/* Pad m_TapNotes so it includes the row "rows". */
	void PadTapNotes(int rows);

public:
	/* Set up to hold the data in From; same number of tracks, same
	 * divisor.  Doesn't allocate or copy anything. */
	void Config( const NoteData &From );

	NoteData();
	~NoteData();
	void Init();
	
	int GetNumTracks() const;
	void SetNumTracks( int iNewNumTracks );

	/* Return the note at the given track and row.  Row may be out of
	 * range; pretend the song goes on with TAP_EMPTYs indefinitely. */
	inline TapNote GetTapNote(unsigned track, int row) const
	{
		if(row < 0 || row >= (int) m_TapNotes[track].size()) return TapNote(TAP_EMPTY);
		return m_TapNotes[track][row];
	}
	void MoveTapNoteTrack(int dest, int src);
	void SetTapNote(int track, int row, TapNote t);

	void ClearRange( int iNoteIndexBegin, int iNoteIndexEnd );
	void ClearAll();
	void CopyRange( const NoteData* pFrom, int iFromIndexBegin, int iFromIndexEnd, int iToIndexBegin = -1 );
	void CopyAll( const NoteData* pFrom );
	
	inline bool IsRowEmpty( int index ) const
	{
		for( int t=0; t<m_iNumTracks; t++ )
			if( GetTapNote(t, index) != TAP_EMPTY )
				return false;
		return true;
	}
	inline int GetNumTapNonEmptyTracks( int index ) const
	{
		int iNum = 0;
		for( int t=0; t<m_iNumTracks; t++ )
			if( GetTapNote(t, index) != TAP_EMPTY )
				iNum++;
		return iNum;
	}
	inline int GetFirstNonEmptyTrack( int index ) const
	{
		for( int t=0; t<m_iNumTracks; t++ )
			if( GetTapNote(t, index) != TAP_EMPTY )
				return t;
		return -1;
	}
	inline int GetNumTracksWithTap( int index ) const
	{
		int iNum = 0;
		for( int t=0; t<m_iNumTracks; t++ )
			if( GetTapNote(t, index) == TAP_TAP )
				iNum++;
		return iNum;
	}
	inline int GetFirstTrackWithTap( int index ) const
	{
		for( int t=0; t<m_iNumTracks; t++ )
			if( GetTapNote(t, index) == TAP_TAP )
				return t;
		return -1;
	}

	// used in edit/record
	void AddHoldNote( HoldNote newNote );	// add note hold note merging overlapping HoldNotes and destroying TapNotes underneath
	void RemoveHoldNote( int index );
	HoldNote &GetHoldNote( int index ) { return m_HoldNotes[index]; }
	const HoldNote &GetHoldNote( int index ) const { return m_HoldNotes[index]; }

	// statistics
	bool IsThereATapAtRow( int iRow ) const;

	/* Return the highest beat/row that might contain notes.  (Use GetLastBeat if you need
	 * accuracy.) */
	float GetMaxBeat() const { return NoteRowToBeat(GetMaxRow()); }
	int GetMaxRow() const { return int(m_TapNotes[0].size()); }

	float GetFirstBeat() const;	// return the beat number of the first note
	int GetFirstRow() const;
	float GetLastBeat() const;	// return the beat number of the last note
	int GetLastRow() const;
	int GetNumTapNotes( const float fStartBeat = 0, const float fEndBeat = -1 ) const;
	int GetNumRowsWithTaps( const float fStartBeat = 0, const float fEndBeat = -1 ) const;
	int GetNumDoubles( const float fStartBeat = 0, const float fEndBeat = -1 ) const;
	/* optimization: for the default of start to end, use the second (faster) */
	int GetNumHoldNotes( const float fStartBeat, const float fEndBeat = -1 ) const;
	int GetNumHoldNotes() const { return m_HoldNotes.size(); }

	// Transformations
	void LoadTransformed( NoteData* pOriginal, int iNewNumTracks, const int iOriginalTrackToTakeFrom[] );	// -1 for iOriginalTracksToTakeFrom means no track
	void LoadTransformedSlidingWindow( NoteData* pOriginal, int iNewNumTracks );	// used by autogen

	// Convert between HoldNote representation and '2' and '3' markers in TapNotes
	void Convert2sAnd3sToHoldNotes();
	void ConvertHoldNotesTo2sAnd3s();
	void To2sAnd3s( const NoteData &out );
	void From2sAnd3s( const NoteData &out );

	void Convert4sToHoldNotes();
	void ConvertHoldNotesTo4s();
	void To4s( const NoteData &out );
	void From4s( const NoteData &out );

	// True if no notes in row that aren't true in the mask
	bool RowPassesValidMask( int row, bool bValidMask[] );

	void EliminateAllButOneTap( int row ); 
};


#endif
