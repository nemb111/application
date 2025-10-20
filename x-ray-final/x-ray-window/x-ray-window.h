
#ifndef X_RAY_WINDOW_H_
#define X_RAY_WINDOW_H_
// TODO: WICHTIG hier wird der memory Leak test auch für den release mode
// forciert. Dies kostet ziemlich viel Rechenleistung.
//#define VLD_FORCE_ENABLE


//#include <vld.h>
#include <Windows.h>


#define XOFFSET 20
#define YOFFSET 62

class SaveAndCancel
{

public:
  SaveAndCancel(bool* saveAndCancel) : _saveAndCancel(saveAndCancel) {}

  bool operator()() {return *_saveAndCancel;}
  void setTrue() {*_saveAndCancel = true;}

private:

  bool* _saveAndCancel;
};

void fit(HWND hWnd);
void updateBitmap(size_t xIdx, size_t yIdx, const Spectrum& spectrum);
//void updateGlobIllBitmap(size_t xIdx, size_t yIdx, const Spectrum& spectrum);
DWORD WINAPI ctStart_xray(LPVOID lpParam);
DWORD WINAPI ctUpdateWindow(LPVOID lpParam);
LRESULT CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam,
                            LPARAM lParam );



#endif//X_RAY_WINDOW_H_