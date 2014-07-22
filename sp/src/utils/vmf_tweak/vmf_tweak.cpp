//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// vmf_tweak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "chunkfile.h"
#include "utllinkedlist.h"
#include <stdlib.h>
#include "tier1/strtools.h"
#include "tier0/icommandline.h"
#include "tier1/utlrbtree.h"
#include "tier1/utlbuffer.h"
#include "tier1/keyvalues.h"
#include "FileSystem.h"
#include "FileSystem_Tools.h"
#include "cmdlib.h"

#include <conio.h>

#include <windows.h>

char const *g_pInFilename = NULL;

char *CopyString( char const *pStr );

unsigned long g_CurLoadOrder = 0;


enum  colors { DARKBLUE = 1, 
			 DARKGREEN, 
			 DARKTEAL, 
			 DARKRED, 
			 DARKPINK, 
			 DARKYELLOW, 
			 GRAY, 
			 DARKGRAY, 
			 BLUE, 
			 GREEN, 
			 TEAL, 
			 RED, 
			 PINK, 
			 YELLOW, 
			 WHITE };

HANDLE hCon;

//-----------------------------------------------------------------------------
// default spec function
//-----------------------------------------------------------------------------
SpewRetval_t VMFTweakSpewFunc( SpewType_t spewType, char const *pMsg )
{
	OutputDebugString( pMsg );
	printf( pMsg );
	switch( spewType )
	{
	case SPEW_MESSAGE:
	case SPEW_WARNING:
	case SPEW_LOG:
		return SPEW_CONTINUE;

	case SPEW_ASSERT:
	case SPEW_ERROR:
	default:
		return SPEW_DEBUGGER;
	}
}

void logprint( char const *logfile, const char *fmt, ... )
{
	char string[ 8192 ];
	va_list va;
	va_start( va, fmt );
	vsprintf( string, fmt, va );
	va_end( va );

	FILE *fp = NULL;
	static bool first = false;
	if ( first )
	{
		first = false;
		fp = fopen( logfile, "wb" );
	}
	else
	{
		fp = fopen( logfile, "ab" );
	}
	if ( fp )
	{
		char *p = string;
		while ( *p )
		{
			if ( *p == '\n' )
			{
				fputc( '\r', fp );
			}
			fputc( *p, fp );
			p++;
		}
		fclose( fp );
	}
}


class CChunkKeyBase
{
public:
	unsigned long m_LoadOrder;	// Which order it appeared in the file.
};


class CKeyValue : public CChunkKeyBase
{
public:
	void SetKey( const char *pStr )
	{
		delete [] m_pKey;
		m_pKey = CopyString( pStr );
	}

	void SetValue( const char *pStr )
	{
		delete [] m_pValue;
		m_pValue = CopyString( pStr );
	}
	

public:
	char	*m_pKey;
	char	*m_pValue;
};


class CChunk : public CChunkKeyBase
{
public:
	// Returns true if the chunk has the specified key and if its value
	// is equal to pValueStr (case-insensitive).
	bool			CompareKey( char const *pKeyName, char const *pValueStr );

	// Look for a key by name.
	CKeyValue*		FindKey( char const *pKeyName );
	CKeyValue*		FindKey( char const *pKeyName, const char *pValue );

	CChunk*			FindChunk( char const *pKeyName );

	// Find a key by name, and replace any occurences with the new name.
	void			RenameKey( const char *szOldName, const char *szNewName );

public:

	char										*m_pChunkName;
	CUtlLinkedList<CKeyValue*, unsigned short>	m_Keys;
	CUtlLinkedList<CChunk*, unsigned short>		m_Chunks;
};


CChunk* ParseChunk( char const *pChunkName, bool bOnlyOne );


CChunk		*g_pCurChunk = 0;
CChunkFile	*g_pChunkFile = 0;
int g_DotCounter = 0;



// --------------------------------------------------------------------------------- //
// CChunk implementation.
// --------------------------------------------------------------------------------- //

bool CChunk::CompareKey( char const *pKeyName, char const *pValueStr )
{
	CKeyValue *pKey = FindKey( pKeyName );

	if( pKey && stricmp( pKey->m_pValue, pValueStr ) == 0 )
		return true;
	else
		return false;
}	


CKeyValue* CChunk::FindKey( char const *pKeyName )
{
	for( unsigned short i=m_Keys.Head(); i != m_Keys.InvalidIndex(); i=m_Keys.Next(i) )
	{
		CKeyValue *pKey = m_Keys[i];

		if( stricmp( pKey->m_pKey, pKeyName ) == 0 )
			return pKey;
	}

	return NULL;
}


CChunk* CChunk::FindChunk( char const *pChunkName )
{
	for( unsigned short i=m_Chunks.Head(); i != m_Chunks.InvalidIndex(); i=m_Chunks.Next(i) )
	{
		CChunk *pChunk = m_Chunks[i];

		if( stricmp( pChunk->m_pChunkName, pChunkName ) == 0 )
			return pChunk;
	}

	return NULL;
}

CKeyValue* CChunk::FindKey( char const *pKeyName, const char *pValue )
{
	for( unsigned short i=m_Keys.Head(); i != m_Keys.InvalidIndex(); i=m_Keys.Next(i) )
	{
		CKeyValue *pKey = m_Keys[i];

		if( stricmp( pKey->m_pKey, pKeyName ) == 0 && stricmp( pKey->m_pValue, pValue ) == 0 )
			return pKey;
	}

	return NULL;
}


void CChunk::RenameKey( const char *szOldName, const char *szNewName )
{
	if ((!szOldName) || (!szNewName))
		return;

	CKeyValue *pKey = FindKey( szOldName );
	if ( pKey )
	{
		delete pKey->m_pKey;
		pKey->m_pKey = CopyString( szNewName );
	}
}

// --------------------------------------------------------------------------------- //
// Util functions.
// --------------------------------------------------------------------------------- //
char *CopyString( char const *pStr )
{
	char *pRet = new char[ strlen(pStr) + 1 ];
	strcpy( pRet, pStr );
	return pRet;
}

ChunkFileResult_t MyDefaultHandler( CChunkFile *pFile, void *pData, char const *pChunkName )
{
	CChunk *pChunk = ParseChunk( pChunkName, true );
	g_pCurChunk->m_Chunks.AddToTail( pChunk );
	return ChunkFile_Ok;
}


ChunkFileResult_t MyKeyHandler( const char *szKey, const char *szValue, void *pData )
{
	// Add the key to the current chunk.
	CKeyValue *pKey = new CKeyValue;
	pKey->m_LoadOrder = g_CurLoadOrder++;
	
	pKey->m_pKey   = CopyString( szKey );
	pKey->m_pValue = CopyString( szValue );

	g_pCurChunk->m_Keys.AddToTail( pKey );
	return ChunkFile_Ok;
}


CChunk* ParseChunk( char const *pChunkName, bool bOnlyOne )
{
	// Add the new chunk.
	CChunk *pChunk = new CChunk;
	pChunk->m_pChunkName = CopyString( pChunkName );
	pChunk->m_LoadOrder = g_CurLoadOrder++;

	// Parse it out..
	CChunk *pOldChunk = g_pCurChunk;
	g_pCurChunk = pChunk;

	//if( ++g_DotCounter % 16 == 0 )
	//	printf( "." );

	while( 1 )
	{
		if( g_pChunkFile->ReadChunk( MyKeyHandler ) != ChunkFile_Ok )
			break;
	
		if( bOnlyOne )
			break;
	}

	g_pCurChunk = pOldChunk;
	return pChunk;
}


CChunk* ReadChunkFile( char const *pInFilename )
{
	CChunkFile chunkFile;

	if( chunkFile.Open( pInFilename, ChunkFile_Read ) != ChunkFile_Ok )
	{
		SetConsoleTextAttribute(hCon, RED);
		printf( "Error opening chunk file %s for reading.\n", pInFilename );
		SetConsoleTextAttribute(hCon, GRAY);
		return NULL;
	}

	printf( "Reading.." );
	chunkFile.SetDefaultChunkHandler( MyDefaultHandler, 0 );
	g_pChunkFile = &chunkFile;
	
	CChunk *pRet = ParseChunk( "***ROOT***", false );
	printf( "\n" );

	return pRet;
}

class CChunkHolder
{
public:
	static bool SortChunkFn( const CChunkHolder &a, const CChunkHolder &b )
	{
		return a.m_LoadOrder < b.m_LoadOrder;
	}

	unsigned long m_LoadOrder;
	CKeyValue *m_pKey;
	CChunk *m_pChunk;
};


void WriteChunks_R( CChunkFile *pFile, CChunk *pChunk, bool bRoot )
{
	if( !bRoot )
	{
		pFile->BeginChunk( pChunk->m_pChunkName );
	}

	// Sort them..
	CUtlRBTree<CChunkHolder,int> sortedStuff( 0, 0, &CChunkHolder::SortChunkFn );
	
	// Write keys.
	for( unsigned short i=pChunk->m_Keys.Head(); i != pChunk->m_Keys.InvalidIndex(); i = pChunk->m_Keys.Next( i ) )
	{
		CChunkHolder holder;
		holder.m_pKey = pChunk->m_Keys[i];
		holder.m_LoadOrder = holder.m_pKey->m_LoadOrder;
		holder.m_pChunk = NULL;
		sortedStuff.Insert( holder );
	}

	// Write subchunks.
	for( int i=pChunk->m_Chunks.Head(); i != pChunk->m_Chunks.InvalidIndex(); i = pChunk->m_Chunks.Next( i ) )
	{
		CChunkHolder holder;
		holder.m_pChunk = pChunk->m_Chunks[i];
		holder.m_LoadOrder = holder.m_pChunk->m_LoadOrder;
		holder.m_pKey = NULL;
		sortedStuff.Insert( holder );
	}

	// Write stuff in sorted order.
	int i = sortedStuff.FirstInorder();
	if ( i != sortedStuff.InvalidIndex() )
	{
		while ( 1 )
		{
			CChunkHolder &h = sortedStuff[i];
			if ( h.m_pKey )
			{
				pFile->WriteKeyValue( h.m_pKey->m_pKey, h.m_pKey->m_pValue );
			}
			else
			{
				WriteChunks_R( pFile, h.m_pChunk, false );
			}
			if ( i == sortedStuff.LastInorder() )
				break;
			i = sortedStuff.NextInorder( i );
		}
	}

	if( !bRoot )
	{
		pFile->EndChunk();
	}
}


bool WriteChunkFile( char const *pOutFilename, CChunk *pRoot )
{
	CChunkFile chunkFile;

	if( chunkFile.Open( pOutFilename, ChunkFile_Write ) != ChunkFile_Ok )
	{
		SetConsoleTextAttribute(hCon, RED);
		printf( "Error opening chunk file %s for writing.\n", pOutFilename );
		SetConsoleTextAttribute(hCon, GRAY);
		return false;
	}

	printf( "Writing.." );
	WriteChunks_R( &chunkFile, pRoot, true );
	printf( "\n\n" );

	return true;
}


//
//
// EXAMPLE
//
//
int roundfloat(float x) 
{ 
	if (x > 0.0f)
		x += 0.5f;
	else
		x-= 0.5f;
	return (int)x;
}
void do_stuff( CChunk *pChunk )
{
	if( stricmp( pChunk->m_pChunkName, "side" ) == 0 )
	{
		CKeyValue *pKey = pChunk->FindKey( "plane" );
		if( pKey )
		{
			float test1,test2,test3,test4,test5,test6,test7,test8,test9;
			printf("%s\n",pKey->m_pValue);
			bool roundNum = true;
			if ( roundNum )
			{
					sscanf(pKey->m_pValue, "(%f %f %f) (%f %f %f) (%f %f %f)",&test1, &test2, &test3, &test4, &test5, &test6, &test7, &test8, &test9 );
					char rounded[128];
					_snprintf(rounded, 128, "(%d %d %d) (%d %d %d) (%d %d %d)", roundfloat(test1), roundfloat(test2), roundfloat(test3), roundfloat(test4), roundfloat(test5), roundfloat(test6), roundfloat(test7), roundfloat(test8), roundfloat(test9));
				pKey->m_pValue = CopyString(rounded);
			}
		}
		CKeyValue *pKey2 = pChunk->FindKey( "material" );
		if( pKey2 )
		{
			printf("%s\n",pKey2->m_pValue);
		}
		CKeyValue *pKey3 = pChunk->FindKey( "uaxis" );
		if( pKey3 )
		{
			printf("%s\n",pKey3->m_pValue);
		}
		CKeyValue *pKey4 = pChunk->FindKey( "vaxis" );
		if( pKey4 )
		{
			printf("%s\n",pKey4->m_pValue);
		}
		printf("\n");
	}
}
void ScanChunks( CChunk *pChunk, void (*fn)(CChunk *) );
void ScanRopeSlack( CChunk *pChunk )
{
	if( stricmp( pChunk->m_pChunkName, "solid" ) == 0 )
	{
		ScanChunks(pChunk, do_stuff);
	}
}

void ChangeSound( CChunk *pChunk )
{
	if( stricmp( pChunk->m_pChunkName, "entity" ) == 0 )
	{
		if( pChunk->CompareKey( "classname", "ambient_generic" ))
		{
			CKeyValue *pKey = pChunk->FindKey( "message" );
			if( pKey )
			{
				if ( strcmp( pKey->m_pValue, "Metal_SeafloorCar.BulletImpact") == 0 )
					pKey->m_pValue = "Airboat_impact_hard";
			}
		}
	}
}

void ConvertSynthSoldier( CChunk *pChunk )
{
	if( stricmp( pChunk->m_pChunkName, "entity" ) == 0 )
	{
		if( pChunk->CompareKey( "classname", "npc_combine_s" ))
		{
			SetConsoleTextAttribute(hCon, BLUE);
			Msg("npc_combine_s found! replaced with npc_synthsoldier\n");
			SetConsoleTextAttribute(hCon, GRAY);
			CKeyValue *pKey = pChunk->FindKey( "classname" );
			pKey->m_pValue = "npc_synthsoldier";
		}
	}
}

void ConvertGlock( CChunk *pChunk )
{
	if( stricmp( pChunk->m_pChunkName, "entity" ) == 0 )
	{
		if( pChunk->CompareKey( "classname", "weapon_pistol" ))
		{
			SetConsoleTextAttribute(hCon, BLUE);
			Msg("weapon_pistol found! replaced with weapon_glock\n");
			SetConsoleTextAttribute(hCon, GRAY);
			CKeyValue *pKey = pChunk->FindKey( "classname" );
			pKey->m_pValue = "weapon_glock";
		}
	}
}

void ScanChunks( CChunk *pChunk, void (*fn)(CChunk *) )
{
	fn( pChunk );

	// Recurse into the children.
	for( unsigned short i=pChunk->m_Chunks.Head(); i != pChunk->m_Chunks.InvalidIndex(); i = pChunk->m_Chunks.Next( i ) )
	{
		ScanChunks( pChunk->m_Chunks[i], fn );
	}
}

int ProcessVMF(int argc, const char *inFile, const char *outfile)
{
	CChunk *pRoot = ReadChunkFile( inFile );
	if( !pRoot )
		return 2;

	ScanChunks( pRoot, ConvertSynthSoldier );
	ScanChunks( pRoot, ConvertGlock );
	ScanChunks( pRoot, ChangeSound );
	CChunk *test = pRoot->FindChunk("world");
	ScanChunks( test, ScanRopeSlack );

	if( !WriteChunkFile( outfile, pRoot ) )
			return 3;
	
	return 0;
}

int main(int argc, char* argv[])
{
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CommandLine()->CreateCmdLine( argc, argv );
	SpewOutputFunc( VMFTweakSpewFunc );
	
	if( argc < 2 )
	{
		printf( "vmf_tweak <input file> [output file]\n" );
		return 1;
	}
	g_pInFilename = argv[1];


	if ( strstr(g_pInFilename, "*."))
	{
		char	drive[ MAX_PATH ];
		char	dir[MAX_PATH];
		_splitpath( g_pInFilename, drive, dir, NULL, NULL );

		char mapDir[MAX_PATH];
		V_snprintf( mapDir, sizeof(mapDir), "%s%s", drive, dir );
		
		WIN32_FIND_DATA wfd;
		HANDLE hResult;
		memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

		hResult = FindFirstFile( g_pInFilename, &wfd );
		while ( hResult != INVALID_HANDLE_VALUE )
		{
			char filename[256];
			//add map directory plus name of file to a char array
			V_snprintf(filename, sizeof(filename),"%s%s", mapDir,wfd.cFileName);
			
			SetConsoleTextAttribute(hCon, YELLOW);
			Msg("file: %s\n",filename);
			SetConsoleTextAttribute(hCon, GRAY);
			ProcessVMF(argc, filename, filename);
			if ( !FindNextFile( hResult, &wfd) )
				break;
		}
		/*return 0;*/
	}
	else
		/*return */ProcessVMF(argc, g_pInFilename, g_pInFilename);	
	system("pause");
}