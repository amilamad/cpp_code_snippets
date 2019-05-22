#include <windows.h>	/* This enables access to Microsoft Windows specific data types that are required to use the Graphical User Interface of Windows. (HWND, WNDCLASSEX, etc.)	*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); /* A 'Forward Declaration' of a function that will mainly handle user interaction with our window.	*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) /* The 'entry' point of the program. Console programs use the simple main(...)		*/
{
	MSG msg = { 0 };

	static const char* class_name = "DUMMY_CLASS";
	WNDCLASSEX wx = { 0 };
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc	= WndProc;        // function which will handle messages
	wx.hInstance	= hInstance;
	wx.lpszClassName = class_name;

	if (RegisterClassEx(&wx)) 
	{
		auto hwnd = CreateWindowEx(0, class_name, "dummy_name", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);
	}

	while( GetMessage( &msg, NULL, 0, 0 ) > 0 )			/* This checks user 'messages', and if our window is for example clicked, the 'message' will go to our WndProc function.	*/
	{
		DispatchMessage( &msg );						/* Sends the messages that were relevant to our window to the WndProc function we have defined for our window class.		*/
	}

	return 0;											/* Everything went ok, return 0 as a sign of successful program termination.												*/
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) /* The implementation of our forward declaration. A common 'message loop' or, 'Window Procedure'	*/
{
	switch(message)										/* The message that our window has received can be caught using various 'defines' like: WM_LBUTTONUP, WM_KEYDOWN, etc. :)	*/
	{
	case WM_CLOSE:										/* The user clicked the 'X' at the top right of the window, we caught the message with GetMessage() and processed it here :)*/
		PostQuitMessage(0);								/* This requests Windows the permission to terminate (quit). GetMessage catches that, returns false, and ends the while loop*/
		break;

	case WM_CREATE:
	{
		#define MAX_DEVICE_COLLECTION_TYPES 3
		RAWINPUTDEVICE rid[MAX_DEVICE_COLLECTION_TYPES];
		memset(rid, 0, sizeof(RAWINPUTDEVICE) * MAX_DEVICE_COLLECTION_TYPES);

		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x04;	// Joystick
		rid[0].dwFlags = RIDEV_EXINPUTSINK | RIDEV_DEVNOTIFY;
		rid[0].hwndTarget = hWnd;

		rid[1].usUsagePage = 0x01;
		rid[1].usUsage = 0x06;	// Key board
		rid[1].dwFlags = RIDEV_EXINPUTSINK | RIDEV_DEVNOTIFY;
		rid[1].hwndTarget = hWnd;

		rid[2].usUsagePage = 0x01;
		rid[2].usUsage = 0x02;	// Mouse
		rid[2].dwFlags = RIDEV_EXINPUTSINK | RIDEV_DEVNOTIFY;
		rid[2].hwndTarget = hWnd;

		if (!RegisterRawInputDevices(rid, MAX_DEVICE_COLLECTION_TYPES, sizeof(RAWINPUTDEVICE)))
			return -1;
	}
	return 0L;

	case WM_INPUT:
	{
		
	}
	return 0L;

	default:											/* If no 'case' condition matched the content of the message received, return DefWindowProc(...)							*/
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam); /* Our application specified no action for the received message(say for WM_KEYDOWN),pass the message along to 'others'.*/
}