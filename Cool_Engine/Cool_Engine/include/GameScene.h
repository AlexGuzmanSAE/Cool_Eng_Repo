#pragma once
#include "EventTypes.h"
#include "SceneBase.h"

#include <lua.hpp>
#include <raylib.h>
#include <sol/sol.hpp>

class GameScene : public SceneBase {
public:
  static GameScene &instance();

  void Load() override;
  void UnLoad() override;
  void UpdateScene() override;
  void Draw() override;
  void update() override;
  void PressButton();
  void OnButtonPress(const ClickButtonEvent &e);

private:
  Rectangle button;
  ClickButtonEvent event;

  void onCollision(const CollisionEvent &event);

  // LUA
  void bindRaylib();

  sol::state lua;
  sol::function luaUpdate;
  sol::function luaDraw;
};
