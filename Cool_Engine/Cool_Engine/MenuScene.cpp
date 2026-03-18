#include "MenuScene.h"
#include "SceneManager.h"
#include "GameEngine.h"
#include "BilliardScene.h"
#include "raylib/raylib.h"
#include "include/raygui.h"
#include <cmath>

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

    sw = GetScreenWidth();
    sh = GetScreenHeight();

    float btnW = 300.f;
    float btnH = 52.f;
    float gap = 18.f;
    float totalH = btnH * 3 + gap * 2;
    float startY = sh * 0.52f - totalH * 0.5f;
    float startX = sw * 0.5f - btnW * 0.5f;

    btnPlay = { startX, startY,              btnW, btnH };
    btnSettings = { startX, startY + btnH + gap, btnW, btnH };
    btnQuit = { startX, startY + (btnH + gap) * 2, btnW, btnH };

    applyMenuStyle();
}

void MenuScene::UnLoad()
{
    GuiLoadStyleDefault();
}

void MenuScene::update()
{
    pulseTimer += GetFrameTime();

    if (goToBilliard)
    {
        goToBilliard = false;
        SceneManager::instance().changeScene(&BilliardScene::instance());
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

    const char* title = "BILLIARDS";
    int tsize = 72;
    int tw = MeasureText(title, tsize);
    // sombra
    DrawText(title, sw / 2 - tw / 2 + 4, (int)(sh * 0.25f) + 4, tsize,
        { 0, 0, 0, 140 });
    DrawText(title, sw / 2 - tw / 2, (int)(sh * 0.25f), tsize, GOLD);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1A6B30FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x28A045FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x0F4A20FF);
    if (GuiButton(btnPlay, "#131# Play"))
        goToBilliard = true;

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x2C3E50FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x3D5166FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x1A252FFF);
    GuiButton(btnSettings, "#141# Settings");

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x7B1010FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xA01515FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x4A0A0AFF);
    if (GuiButton(btnQuit, "#113# Quit"))
        goToQuit = true;
}