#pragma once
#include "SceneBase.h"
class GameScene :
    public SceneBase
{
public:

    void Load() override;
    void UnLoad() override;
    void Update() override;
    void Draw() override;
};

