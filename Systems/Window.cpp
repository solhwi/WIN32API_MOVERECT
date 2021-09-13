#include "stdafx.h"
#include "Window.h"
#include "Program.h"

WinDesc Window::desc;
Program* Window::program = nullptr;

Window::Window(WinDesc desc) // Window Descriptiow(설명, 묘사)
{
	WNDCLASSEX wndClass; // 윈도우 창을 만들기 위한 클래스
	ZeroMemory(&wndClass, sizeof(wndClass)); // 해당 주소를 0으로 초기화

	//cbClsExtra, cbWndExtra는 쓸일이 없으므로 0으로 세팅
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 바탕색, HBRUSH - BRUSH Handle
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW); // 마우스커서, IDC_ARROW - 일반커서, 해당 값을 바꿔주면 다른 커서로 변경됨
	wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO); // 왼쪽상단의 아이콘
	wndClass.hIconSm = wndClass.hIcon; // 작은아이콘
	wndClass.hInstance = desc.instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc; //윈도우 프로시저 - 윈도우 메시지 처리함수
	wndClass.lpszClassName = desc.AppName.c_str(); // 윈도우 클래스 네임
	wndClass.lpszMenuName = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // 클래스 스타일
	wndClass.cbSize = sizeof(WNDCLASSEX); // 버전확인을 위해 사이즈 체크

	WORD wHr = RegisterClassEx(&wndClass); // 레지스터에 등록
	assert(wHr != 0);

	desc.handle = CreateWindowEx // 윈도우창 생성
	(
		WS_EX_APPWINDOW,
		desc.AppName.c_str(),
		desc.AppName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_TABSTOP, // 윈도우 스타일
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		(HMENU)nullptr,
		desc.instance, // 실제로 생성할 인스턴스
		nullptr
	);

	RECT rect = { 0,0,(LONG)desc.width, (LONG)desc.height };

	//GetSystemMetrics - 화면의 실제 값을 가져옴
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	MoveWindow
	(
		desc.handle,
		centerX,
		centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		true
	);

	ShowWindow(desc.handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.handle);
	SetFocus(desc.handle);

	ShowCursor(true);

	Window::desc = desc;

}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(), desc.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	Keyboard::Create();
	Mouse::Create();
	Time::Create();

	Time::Get()->Start();

	program = new Program();
	while (true)
	{
		//GetMessage - 아무런 메시지가 없다면 새로운 메시지가 올때까지 계속 대기
		//PeekMessage - 메시지가 없다면 함수호출이 바로 종료됨
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SAFE_DELETE(program);

	Time::Delete();
	Mouse::Delete();
	Keyboard::Delete();

	return msg.wParam;
}

HDC hdc;
HWND handle;

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	PAINTSTRUCT ps;
	ZeroMemory(&ps, sizeof(ps));

	switch (message)
	{
	case WM_CREATE:
	{
		::handle = handle;
		SetTimer(handle, 1, 0, nullptr);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(handle, &ps);

		Time::Get()->Print();
		Mouse::Get()->Print();

		if (program)
			program->Render();

		EndPaint(handle, &ps);

		break;
	}

	case WM_TIMER:
	{
		Time::Get()->Update();
		Keyboard::Get()->Update();
		Mouse::Get()->Update();

		program->Update();

		InvalidateRect(handle, nullptr, true);

		break;
	}
	}


	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		KillTimer(handle, NULL);
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
