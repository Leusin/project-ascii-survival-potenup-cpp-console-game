#include "Game/Game.h"

#include <iostream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	game.Run();


	// TEST
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) {
        std::cout << "콘솔 화면 버퍼 정보:" << std::endl;
        std::cout << "  커서 위치: (" << consoleInfo.dwCursorPosition.X << ", " << consoleInfo.dwCursorPosition.Y << ")" << std::endl;
        std::cout << "  화면 크기: (" << consoleInfo.dwSize.X << ", " << consoleInfo.dwSize.Y << ")" << std::endl;
        std::cout << "  문자 속성: " << consoleInfo.wAttributes << std::endl;
        // 필요한 다른 정보들도 사용 가능합니다.
    }
    else {
        std::cerr << "GetConsoleScreenBufferInfo 함수 호출 실패" << std::endl;
        // 에러 처리
    }



	return 0;
}