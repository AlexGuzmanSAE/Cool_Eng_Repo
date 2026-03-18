#include "MenuScene.h"
#include "SceneManager.h"
#include "GameEngine.h"
#include "BilliardScene.h"
#include "raylib/raylib.h"
#include "include/raygui.h"
#include <cmath>
#include "AudioHelper.h"
#include "SaveSistem.h"

static void applyMenuStyle()
{
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x0D0D0DFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x1A1A2EFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x16213EFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x0F3460FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xE2E2E2FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x2E4057FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x4A90D9FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x74B9FFFF);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
}

void MenuScene::Load()
{
    pulseTimer = 0.f;
    goToBilliard = false;
    goToQuit = false;
    goContinue = false;

    sw = GetScreenWidth();
    sh = GetScreenHeight();

    hasSave = SaveSistem::instance().hasSave();

    float btnW = 300.f;
    float btnH = 52.f;
    float gap = 18.f;
    int   count = hasSave ? 4 : 3;
    float totalH = btnH * count + gap * (count - 1);
    float startX = sw * 0.5f - btnW * 0.5f;
    float startY = sh * 0.52f - totalH * 0.5f;

    int i = 0;
    if (hasSave)
    {
        btnContinue = { startX, startY + (btnH + gap) * i++, btnW, btnH };
    }
    btnPlay = { startX, startY + (btnH + gap) * i++, btnW, btnH };
    btnSettings = { startX, startY + (btnH + gap) * i++, btnW, btnH };
    btnQuit = { startX, startY + (btnH + gap) * i,   btnW, btnH };

    sfxClick = ResourceManager::instance().load_sounds("selectUI.wav");
    bgMusic = ResourceManager::instance().load_music("menu.ogg");

    if (bgMusic)
    {
        Audio_SetMusicVolume(*bgMusic, 0.5f);
        PlayMusicStream(*bgMusic);
    }

    applyMenuStyle();
}

void MenuScene::UnLoad()
{
    if (bgMusic) Audio_StopMusic(*bgMusic);
    GuiLoadStyleDefault();
}

void MenuScene::update()
{
    pulseTimer += GetFrameTime();
    if (bgMusic) Audio_UpdateMusic(*bgMusic);

    if (goToBilliard)
    {
        goToBilliard = false;
        SceneManager::instance().changeScene(&BilliardScene::instance());
        return;
    }
    if (goContinue)
    {
        goContinue = false;
        auto save = SaveSistem::instance().loadBilliard();
        if (save)
        {
            BilliardScene::instance().applyState(*save);
            SceneManager::instance().changeScene(&BilliardScene::instance());
        }
        return;
    }
    if (goToQuit)
    {
        goToQuit = false;
        GameEngine::shouldQuit = true;
        return;
    }
}

void MenuScene::Draw()
{
    
    DrawRectangleGradientV(0, 0, sw, sh,
        { 10, 60, 20, 255 }, { 5, 25, 10, 255 });

    float pulse = 0.5f + 0.5f * sinf(pulseTimer * 2.5f);
    Color dotCol = { 0, 0, 0, (unsigned char)(160 + (int)(70 * pulse)) };
    int   dotR = 22;
    DrawCircle(dotR + 4, dotR + 4, dotR, dotCol);
    DrawCircle(sw / 2, dotR + 4, dotR, dotCol);
    DrawCircle(sw - dotR - 4, dotR + 4, dotR, dotCol);
    DrawCircle(dotR + 4, sh - dotR - 4, dotR, dotCol);
    DrawCircle(sw / 2, sh - dotR - 4, dotR, dotCol);
    DrawCircle(sw - dotR - 4, sh - dotR - 4, dotR, dotCol);

    
    const char* title = "COOL BILLAR";
    int tsize = 72;
    int tw = MeasureText(title, tsize);
    DrawText(title, sw / 2 - tw / 2 + 4, (int)(sh * 0.12f) + 4, tsize, { 0,0,0,140 });
    DrawText(title, sw / 2 - tw / 2, (int)(sh * 0.12f), tsize, GOLD);

    
    if (hasSave)
    {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x0F4A6AFF);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x1A7AAAFF);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x083040FF);
        if (GuiButton(btnContinue, "#131# Continuar partida"))
        {
            if (sfxClick) Audio_PlaySound(*sfxClick);
            goContinue = true;
        }
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1A6B30FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x28A045FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x0F4A20FF);
    if (GuiButton(btnPlay, "#131# Play"))
    {
        if (sfxClick) Audio_PlaySound(*sfxClick);
        goToBilliard = true;
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x2C3E50FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x3D5166FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x1A252FFF);
    if (GuiButton(btnSettings, "#141# Settings"))
        if (sfxClick) Audio_PlaySound(*sfxClick);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x7B1010FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xA01515FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x4A0A0AFF);
    if (GuiButton(btnQuit, "#113# Quit"))
    {
        if (sfxClick) Audio_PlaySound(*sfxClick);
        goToQuit = true;
    }
}