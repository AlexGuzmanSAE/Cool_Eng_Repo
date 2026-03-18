#include "raylib/raylib.h"
#include "AudioHelper.h"

void Audio_PlaySound(Sound sound) { PlaySound(sound); }
void Audio_StopSound(Sound sound) { StopSound(sound); }
void Audio_PlayMusic(Music& music) { PlayMusicStream(music); }
void Audio_StopMusic(Music& music) { StopMusicStream(music); }
void Audio_UpdateMusic(Music& music) { UpdateMusicStream(music); }
void Audio_SetMusicVolume(Music& music, float vol) { SetMusicVolume(music, vol); }