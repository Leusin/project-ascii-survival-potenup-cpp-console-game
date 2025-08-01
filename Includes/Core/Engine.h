#pragma once

#include<Windows.h>
#include "Core.h"
#include "Input.h"

/** 
 * TODO: 언젠가 Engine의 싱글턴 지우고 싶다.
 */


/// <summary>
/// 엔진 설정 구조체
/// </summary>
struct EngineSettings
{
	int width = 0; // 콘솔 화면 가로 너비
	int height = 0; // 콘솔 화면 세로 너비
	float framerate = 0.0f; // 타겟 프레임 속도
};


class Engine_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run(); // 엔진 루프
	virtual void CleanUp(); // 메모라 해제
	void Quit(); // 종료

	void AddLevel(class Level* newLevel);

	int Width() const;
	int Height() const;

	static Engine& Get();

protected:
	class Level* mainLevel = nullptr;

private:
	void BeginPlaye();
	void Tick(float deltaTime = 0.0f);
	void Render();

	void LoadEngineSettings(); // 엔진 설정

private:
	bool isQuit = false;

	Input input; // 입력 관리자

	EngineSettings settings; // 엔진 설정
	
	static Engine* instance; // 싱글턴 변수
};
