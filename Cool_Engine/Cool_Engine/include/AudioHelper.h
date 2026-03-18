#pragma once
#include "raylib/raylib.h"

void Audio_PlaySound(Sound sound);
void Audio_StopSound(Sound sound);
void Audio_PlayMusic(Music& music);
void Audio_StopMusic(Music& music);
void Audio_UpdateMusic(Music& music);
void Audio_SetMusicVolume(Music& music, float vol);