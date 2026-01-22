#pragma once
class SceneBase
{

public:

	virtual ~SceneBase() = default;
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void PressButton() = 0;
	bool buttonPressed;
private:


};

