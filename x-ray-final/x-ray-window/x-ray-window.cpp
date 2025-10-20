#define NOMINMAX

#include "stdafx.h"



//#include "stdlib.h"
//
//#include <limits>
//#include <tchar.h>
//#include <iostream>
//
//#include "guicomm.h"
//#include "options.h"
//#include "ppm.h"
//#include "resource.h"
//#include "x-ray.h"
//#include "x-ray-window2.h"


#define WNDCLASSNAME _T("X-Ray")


//_____________________________________________________________________________
static HINSTANCE hInst;
static HWND hWnd;
static bool saveAndCancelSwitch = false;
static SaveAndCancel saveAndCancelFunctor(&saveAndCancelSwitch);


//_____________________________________________________________________________
static DWORD* buffer;
//static DWORD* dirIllBuffer;
//static DWORD* globIllBuffer;


//void SaveFileBox(HWND hwnd)
//{
//   OPENFILENAME ofn;
//   wchar_t szFileName[MAX_PATH] = _T("");
//
//   ZeroMemory(&ofn, sizeof(ofn));
//
//   ofn.lStructSize = sizeof(ofn);
//   ofn.hwndOwner = hwnd;
//   ofn.lpstrFilter = _T("PPM-File (*.ppm)\0*.ppm\0")
//                     _T("All Files (*.*)\0*.*\0");
//   ofn.lpstrFile = szFileName;
//   ofn.nMaxFile = MAX_PATH;
//   ofn.lpstrDefExt = _T("ppm");
//   ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST |
//               OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
//
//   if(GetSaveFileName(&ofn))
//      {
//        PPM::writeFile(ofn.lpstrFile,imageInfo.xres, imageInfo.yres, imageInfo.colorDepth, imageInfo.spec2d);
//      }
//}

void spawnConsole()
{
  AllocConsole();

  HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
  int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
  FILE* hf_out = _fdopen(hCrt, "w");
  setvbuf(hf_out, NULL, _IONBF, 1);
  *stdout = *hf_out;

  HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
  hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
  FILE* hf_in = _fdopen(hCrt, "r");
  setvbuf(hf_in, NULL, _IONBF, 128);
  *stdin = *hf_in;

  HANDLE handle_eout = GetStdHandle(STD_ERROR_HANDLE);
  hCrt = _open_osfhandle((long) handle_eout, _O_TEXT);
  FILE* hf_eout = _fdopen(hCrt, "w");
  *stderr = *hf_eout;
  setvbuf( stderr, NULL, _IONBF, 0 );

}


//void fit(HWND hWnd){
//  //not resizable window
//  SetWindowPos(hWnd, 0, 0, 0,
//               imageInfo.xres + 10, imageInfo.yres + 52,
//               SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE);
//  //for resizable window
//  /*SetWindowPos(hWnd, 0, 0, 0,
//               imageInfo.xres + 20, imageInfo.yres + 62,
//               SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE);*/
//}


//_____________________________________________________________________________
void updateBitmap(size_t xIdx, size_t yIdx, const Spectrum& spectrum)
{
  float max = -std::numeric_limits<float>::max();
  float rgb[3];
  Options& options = Options::getInstance();
  unsigned int xResolution = options.xResolution;
  unsigned int yResolution = options.yResolution;
   
  spectrum.ToRGB(rgb);
  // Normalisiere die RGB Werte
  //for (size_t i=0; i<3; i++)
  //  max = rgb[i] > max ? rgb[i] : max;
  //if (max > 255)
  //{
  //  for (size_t i=0; i<3; i++)
  //   rgb[i] *= 255.f / max;
  //}
  
  buffer[(yResolution-yIdx-1) * xResolution+xIdx] = 
    (static_cast<int>(rgb[2]) << 16) | 
    (static_cast<int>(rgb[1]) << 8) | 
    static_cast<int>(rgb[0]);

  //buffer[(yResolution-yIdx-1) * xResolution+xIdx] = 
  //  dirIllBuffer[(yResolution-yIdx-1) * xResolution+xIdx] +
  //  globIllBuffer[(yResolution-yIdx-1) * xResolution+xIdx];

  //if (buffer[(yResolution-yIdx-1) * xResolution+xIdx] > 0xFFFFFF)
  //  buffer[(yResolution-yIdx-1) * xResolution+xIdx] = 0xFFFFFF;


}


//_____________________________________________________________________________
//void updateGlobIllBitmap(size_t xIdx, size_t yIdx, const Spectrum& spectrum)
//{
//  float max = -std::numeric_limits<float>::max();
//  float rgb[3];
//  Options& options = Options::getInstance();
//  unsigned int xResolution = options.xResolution;
//  unsigned int yResolution = options.yResolution;
//   
//  spectrum.ToRGB(rgb);
//  // Normalisiere die RGB Werte
//  for (size_t i=0; i<3; i++)
//    max = rgb[i] > max ? rgb[i] : max;
//  if (max > 255)
//  {
//    for (size_t i=0; i<3; i++)
//     rgb[i] *= 255.f / max;
//  }
//  
//  globIllBuffer[(yResolution-yIdx-1) * xResolution+xIdx] = 
//    (static_cast<int>(rgb[2]) << 16) | 
//    (static_cast<int>(rgb[1]) << 8) | 
//    static_cast<int>(rgb[0]);
//
//  buffer[(yResolution-yIdx-1) * xResolution+xIdx] = 
//    dirIllBuffer[(yResolution-yIdx-1) * xResolution+xIdx] +
//    globIllBuffer[(yResolution-yIdx-1) * xResolution+xIdx];
//
//  if (buffer[(yResolution-yIdx-1) * xResolution+xIdx] > 0xFFFFFF)
//    buffer[(yResolution-yIdx-1) * xResolution+xIdx] = 0xFFFFFF;
//}


//_____________________________________________________________________________
DWORD WINAPI
ctStart_xray(LPVOID lpParam)
{
  return start_xray(static_cast<GUIcomm*>(lpParam) );
}


//_____________________________________________________________________________
DWORD WINAPI
ctUpdateWindow(LPVOID lpParam)
{
  while(true)
  {
   
    Sleep(200);
   
    InvalidateRect (hWnd, NULL, TRUE);
    UpdateWindow (hWnd);
  }

  return 0;
}


//_____________________________________________________________________________
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
                         WPARAM wParam, LPARAM lParam)
{
  BITMAP bm;
  HBITMAP hbmOld;
  HBITMAP pic;
  HDC hdc;
  HDC hdcMem;
  PAINTSTRUCT ps;
  DWORD dwThreadId;
  Options& options = Options::getInstance();


  switch (msg)
  {
    case WM_CLOSE:
      DestroyWindow(hWnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_PAINT :
      pic = CreateBitmap(options.xResolution, options.yResolution,
                         1, 32, buffer);
      hdc = BeginPaint(hWnd, &ps);
      hdcMem = CreateCompatibleDC(hdc);
      hbmOld = (HBITMAP) SelectObject(hdcMem, pic);
      GetObject(pic, sizeof(bm), &bm);
      BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
      SelectObject(hdcMem, hbmOld);
      DeleteDC(hdcMem);
      EndPaint(hWnd, &ps);

      DeleteObject(pic);
      break;
    case WM_ERASEBKGND:
      return TRUE;
    case WM_KEYDOWN:
      switch(wParam)
      {
        /*case _T('S'):
          if(GetKeyState(VK_CONTROL))
            SaveFileBox(hWnd);
          break;*/
        case VK_F11:
          CreateThread(NULL, //Choose default security
                       0, //Default stack size
                       (LPTHREAD_START_ROUTINE)&spawnConsole,//Routine to execute
                       (LPVOID) 0, //Thread parameter
                       0, //Immediately run the thread
                       &dwThreadId //Thread Id 
                       ); 
          break;
        case VK_ESCAPE:
          DestroyWindow(hWnd);
          break;
        /*case VK_F12:
          if(imageInfo.xres > 0 && imageInfo.yres > 0)
          {
            fit(hWnd);
          }*/
        default: break;
      }
      //Hier werden die Aktion auf Menüelemente behandelt
      case WM_COMMAND:
        switch(LOWORD(wParam))
        {
          case ID_FILE_RUN:
            CreateThread(NULL, //Choose default security
                         0, //Default stack size
                         (LPTHREAD_START_ROUTINE)&spawnConsole,//Routine to execute
                         (LPVOID) 0, //Thread parameter
                         0, //Immediately run the thread
                         &dwThreadId //Thread Id
                         ); 
            break;
          /*case ID_FILE_SAVE:
            SaveFileBox(hWnd);
          break;*/

          case ID_FILE_EXIT:
            DestroyWindow(hWnd);
            break;

          /*case ID_WINDOW_FIT:
            if(imageInfo.xres > 0 && imageInfo.yres > 0)
            {
              fit(hWnd);
            }
            break;*/

          case ID_AUFL40001:
            DialogBox(hInst,
                      MAKEINTRESOURCE(IDD_DIALOG3),
                      hWnd,
                      (DLGPROC)DialogProc);

          case ID_SAVE_AND_CANCEL:
            saveAndCancelFunctor.setTrue();
            break;
        }
        break;

  }

  return DefWindowProc(hWnd,msg,wParam,lParam);
}


//_____________________________________________________________________________
LRESULT CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam,
                            LPARAM lParam )
{
  Options& options = Options::getInstance();

  switch(uMsg)
  {
    case WM_INITDIALOG :
      SetDlgItemInt( hDlg, IDC_EDIT1, options.xResolution, FALSE);
      SetDlgItemInt( hDlg, IDC_EDIT2, options.yResolution, FALSE);
      break;
    case WM_COMMAND :
      switch( LOWORD (wParam) )
      {
        BOOL btran;
        unsigned int maxX, maxY;
        RECT desktop;
        HWND hDesktop;

        case IDOK : //OK-Button wurde gedrückt
           // x-Auflösung aus dem ersten Eingabefeld einlesen
          options.xResolution = GetDlgItemInt(hDlg, IDC_EDIT1,
                                              &btran, FALSE);
          // y-Auflösung aus dem zweiten Eingefeld auslesen
          options.yResolution = GetDlgItemInt(hDlg, IDC_EDIT2,
                                              &btran, FALSE);
          // Ermittle die maximale Verfügbare Auflösung des Desktops
          hDesktop = GetDesktopWindow();
          // Get the size of screen to the variable desktop
          GetWindowRect(hDesktop, &desktop);
          maxX = desktop.right;
          maxY = desktop.bottom;

          // Passe Das Eltern-Fenster auf die angegebene Größe an
          SetWindowPos(GetParent(hDlg), 0, 0, 0,
                       options.xResolution + XOFFSET > maxX ? maxX : 
                            options.xResolution + XOFFSET,
                       options.yResolution + YOFFSET > maxY ? maxY :
                            options.yResolution + YOFFSET,
                       SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE);
          //Dialog beenden
          EndDialog(hDlg, IDOK);
          break;
        case IDCANCEL : //Abbrechen-Button gedrückt
          //Dialog beenden
          EndDialog(hDlg, IDCANCEL);
          break;
      }
      break;
    default:
      return FALSE;
  }

  return DefWindowProc(hDlg, uMsg, wParam, lParam);
}


//_____________________________________________________________________________
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR szCmdLine, int iCmdShow)
{

  WNDCLASSEX wc;
  MSG wmsg;
  /*HWND hWnd;*/
  DWORD   dwThreadIdArray[2];
  HANDLE  hThreadArray[2];
  Options& options = Options::getInstance();
  GUIcomm guicomm;


  parseOptions(__argc, __wargv, options);

  buffer = new DWORD[options.xResolution * options.yResolution];
  //dirIllBuffer = new DWORD[options.xResolution * options.yResolution];
  //globIllBuffer = new DWORD[options.xResolution * options.yResolution];
  for (size_t i=0; i<options.xResolution*options.yResolution; i++)
  {
    buffer[i] = 0;
  /*  dirIllBuffer[i] = 0;
    globIllBuffer[i] = 0;*/
  }

      
  if (options.verbose)
    spawnConsole();


  guicomm.updateBitmap = updateBitmap;
  //guicomm.updateGlobIllBitmap= updateGlobIllBitmap;
  guicomm.saveAndCancel = saveAndCancelFunctor;

  hThreadArray[0] = CreateThread( 
                                 NULL, // default security attributes
                                 0,    // use default stack size  
                                 ctStart_xray, // thread function name
                                 static_cast<LPVOID>(&guicomm), // argument to thread function 
                                 0,    // use default creation flags 
                                 &dwThreadIdArray[0]);   // returns the thread identifier 



 
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = 0;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetModuleHandle(NULL);
  wc.hIcon = LoadIcon(GetModuleHandle(NULL),
                      MAKEINTRESOURCE(ID_ICON)); 
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
  wc.lpszClassName = WNDCLASSNAME;
  wc.hIconSm       = (HICON)LoadImage(GetModuleHandle(NULL),
                                      MAKEINTRESOURCE(ID_ICON),
                                      IMAGE_ICON, 16, 16, 0);  

  if(!RegisterClassEx(&wc) )
  {
    MessageBox(NULL, _T("Windows Registration Error"), _T("Error!"),
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                        WNDCLASSNAME,
                        _T("X-Ray"),
                        WS_OVERLAPPED|WS_CAPTION/*|WS_THICKFRAME*/|WS_SYSMENU|WS_MINIMIZEBOX|
                        WS_VISIBLE|SS_CENTERIMAGE|SS_ICON|WS_SIZEBOX,
                        CW_USEDEFAULT, 
                        CW_USEDEFAULT,
                        options.xResolution + XOFFSET,
                        options.yResolution + YOFFSET,
                        NULL,
                        NULL, 
                        GetModuleHandle(NULL), 
                        NULL);

  if(hWnd == NULL)
  {
    MessageBox(NULL, _T("Error creating the window"),
    _T("Error!"),        MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  hInst = hInstance;

  // Erzeuge einen Thread der dafür sorgt, dass das Fenster geupdatet wird
  hThreadArray[1] = CreateThread( 
                                 NULL, // default security attributes
                                 0,    // use default stack size  
                                 ctUpdateWindow, // thread function name
                                 static_cast<LPVOID>(&hWnd), // argument to thread function 
                                 0,    // use default creation flags 
                                 &dwThreadIdArray[1]);   // returns the thread identifier 

  while(GetMessage(&wmsg,NULL,0,0))
  {
    TranslateMessage(&wmsg);
    DispatchMessage(&wmsg);
  }

  delete[] buffer;
  //delete[] dirIllBuffer;
  //delete[] globIllBuffer;


  return (int) wmsg.wParam;
}
