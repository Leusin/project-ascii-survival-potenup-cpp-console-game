#pragma once

class DebugManager
{
public:
	bool IsDebugMode() const { return isDebugMode; }
	void ToggleDebugMode() { isDebugMode = !isDebugMode; }

	// 게임 일시 정지 관련 추가 
	bool IsGamePaused() const { return isGamePaused; }
	void ToggleGamePause() { isGamePaused = !isGamePaused; }

private:
	bool isDebugMode = false;
	bool isGamePaused = false; // 게임 일시 정지 플래그
};
