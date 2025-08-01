#include "Engine.h"

#include <iostream>
#include "Level/level.h"
#include "Utils/Utils.h"

Engine* Engine::instance = nullptr;

BOOL WINAPI ConsoleMessageProcedure(DWORD ctrlType)
{
	switch (ctrlType)
	{
	case CTRL_CLOSE_EVENT:
	{
		Engine::Get().CleanUp(); // Engine::Get().~Engine(); // 와 같다
		return false;
	}
	}
	return true;
}

Engine::Engine()
{
	instance = this;

	// 커서 설정
	_CONSOLE_CURSOR_INFO consoleCursorInfo;
	consoleCursorInfo.bVisible = false;
	consoleCursorInfo.dwSize = 1;

	// 커서가 보이지 않도록 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursorInfo);

	// 여기서 설정하면 모든 텍스트에 적용됨
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE); // 콘솔창 이벤트 등록

	LoadEngineSettings(); // 엔진 설정 로드

	// 랜덤 종자값(Seed) 생성
	srand(static_cast<unsigned int>(time(nullptr))); 
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::Run()
{
	//float currentTime = timeGetTime();
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LARGE_INTEGER previousTime = currentTime;

	// 하드웨어의 정밀도(주파수) 가져오기
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 타겟 프레임 설정
	float targetFrameRate = (settings.framerate != 0.0f) ? settings.framerate : 60.f;

	float oneFrameTime = 1.f / targetFrameRate;

	while (true)
	{
		if (isQuit)
		{
			break;
		}

		QueryPerformanceCounter(&currentTime);
		float deltaTime = (currentTime.QuadPart - previousTime.QuadPart) / (float)frequency.QuadPart;

		input.ProcessInput();

		if (deltaTime >= oneFrameTime) // 고정 프레임
		{
			// ========== EVENT ==========
			BeginPlaye();
			Tick(deltaTime);
			Render();
			// ========== EVENT ==========

			// 제목에 FPS 출력
			char title[50] = {};
			sprintf_s(title, 50, "(h: %d, w: %d)FPS: %f",settings.height, settings.width, (1.f / deltaTime));
			SetConsoleTitleA(title);

			previousTime = currentTime;

			input.SavePreviousKeyStates();
			
			if (mainLevel) // 이전 프레임에 추가 및 삭제 요청 처리
			{
				mainLevel->ProcessAddAndDestroyActors();
			}
		}
	}

	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7 /*= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED*/); // 출력 색상 정리.
	Utils::SetConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Engine::CleanUp()
{
	SafeDelete(mainLevel);
}

void Engine::Quit()
{
	isQuit = true;
}

void Engine::AddLevel(Level* newLevel)
{
	if (mainLevel)
	{
		delete mainLevel;
	}

	mainLevel = newLevel;
}

int Engine::Width() const
{
	return settings.width;
}

int Engine::Height() const
{
	return settings.height;
}

Engine& Engine::Get()
{
	return *instance;
}

void Engine::BeginPlaye()
{
	// LEVEL
	if (mainLevel)
	{
		mainLevel->BeginPlay();
	}
}

void Engine::Tick(float deltaTime)
{
	//std::cout << "DELTA TIME : " << deltaTime << ", ";
	//std::cout << " FPS : " << 1.0f / deltaTime << "\n"; // 순간치 계산

	//if (GetKey('A'))
	//{
	//	std::cout << "GetKey" << "\n";
	//}
	//if (GetKeyUp('A'))
	//{
	//	std::cout << "GetKeyUp" << "\n";
	//}
	//if (GetKeyDown('A'))
	//{
	//	std::cout << "GetKeyDown" << "\n";
	//}

	// LEVEL
	if (mainLevel)
	{
		mainLevel->Tick(deltaTime);
	}

	/*if (GetKeyDown(VK_ESCAPE))
	{
		Quit();
	}*/
}

void Engine::Render()
{
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7 /*= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED*/); // 출력 색상 정리.
	Utils::SetConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	// LEVEL
	if (mainLevel)
	{
		mainLevel->Render();
	}
}

void Engine::LoadEngineSettings()
{
	// 파일 열기
	FILE* file = nullptr;

	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");

	if (file == nullptr)
	{
		std::cout << "Failed to load engine settigs. \n";
		__debugbreak();
		return;
	}

	/// 로드 
	fseek(file, 0, SEEK_END); // FP(file position) 포인터를 끝위치로 옮기기
	size_t fileSize = ftell(file); // 위치 구하기
	rewind(file); // FP 포인터를 처음으로 

	char* buffer = new char[fileSize + 1];

	size_t readSize = fread(buffer, sizeof(char), fileSize, file); // 내용 읽기

	/// 파싱(구문 해석)
	char* context = nullptr;
	char* token = nullptr;

	token = strtok_s(buffer, "\n", &context);

	// 구문 분석
	while (token != nullptr)
	{
		// 키 = 값 분리
		char header[10] = {};

		// 아래 구문이 제대로 동작하려면 키와 값 사이의 빈칸이 있어야 함
		sscanf_s(token, "%s", header, 10);

		// 헤더 문자열 비교
		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}

		// 그 다음줄 분리
		token = strtok_s(nullptr, "\n", &context);
	}

	SafeDeleteArray(buffer); // 버퍼 해제

	// 파일 닫기
	fclose(file);
}
