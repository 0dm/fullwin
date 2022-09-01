#include <stdio.h>
#include <windows.h>

// dumb but colourful console output 
void reportError(char reason[], char msg[]){
    HANDLE cstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(cstdout, &consoleInfo);
    WORD saved = consoleInfo.wAttributes;
    SetConsoleTextAttribute(cstdout, (FOREGROUND_RED));
    printf("error: ");
    SetConsoleTextAttribute(cstdout, saved);
    printf("%s", reason);
    SetConsoleTextAttribute(cstdout, (FOREGROUND_INTENSITY));
    printf("\n%s", msg);
    SetConsoleTextAttribute(cstdout, saved);
}

void fullscreen(char window[]) {

  // Find Process
  DWORD PID;
  HWND hWnd = FindWindowA(NULL, window);
  if (hWnd == NULL) {
    reportError("error", "could not locate window");
    return;
  }
  GetWindowThreadProcessId(hWnd, &PID);
  HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

  // rectangle
  RECT final;
  final.right = GetSystemMetrics(SM_CXSCREEN);
  final.bottom = GetSystemMetrics(SM_CYSCREEN);
  final.left = 0;
  final.top = 0;

  // align and stretch
  SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
  AdjustWindowRect(&final, GetWindowLong(hWnd, GWL_STYLE), FALSE);
  SetWindowLong(hWnd, GWL_EXSTYLE, (GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST));
  MoveWindow(hWnd, final.left, final.top, final.right - final.left, final.bottom - final.top, TRUE);
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
        reportError("invalid usage", "usage: fullwin <app title>");
        return 0;
  }
  fullscreen(argv[1]);
}