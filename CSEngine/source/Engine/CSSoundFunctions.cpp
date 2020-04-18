#include "CSEngine.hpp"
static void PlayLocalSoundExport(const char* name)
{

}
static void PlayBackgroundTrackExport(const char* name, const char* loop)
{

}
void CSSound::FunctionToC()
{
	GMenuEngineFunctions.PlayLocalSound = PlayLocalSoundExport;
	GMenuEngineFunctions.PlayBackgroundTrack = PlayBackgroundTrackExport;
}
