#include <windows.h>
#include <iostream>
#include "utils.h"
#include "profile.h"
#include "resource.h"


INT_PTR CALLBACK _WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,        // ��ǰ��Ӧ�ó�����

    HINSTANCE hPrevInstance, // ǰһ��Ӧ�ó���ʵ���ľ������Win32�ϣ�ʼ��ΪNULL��

    LPSTR lpCmdLine,        // �����в���

    int        nCmdShow     // ���ڵ���ʾ��ʽ

)

{
    ::DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, _WndProc);

    return 0;

}





INT_PTR CALLBACK _WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)

{
    BOOL bRet = TRUE;
    int screenW = 0, screenH = 0;
    RECT rect = { 0, };
   
    switch (message) {
    case WM_INITDIALOG:
        ::GetClientRect(hwnd, &rect);
        screenW = ::GetSystemMetrics(SM_CXFULLSCREEN);
        screenH = ::GetSystemMetrics(SM_CYFULLSCREEN);
        shower = new PhotoShower(hwnd, screenW / 2 - rect.right / 2, screenH / 2 - rect.bottom / 2, rect.right, rect.bottom);
       // shower->setImages(std::vector<std::wstring>({ TEXT("download/CfjkDYrOrQC_post_0.jpg"), TEXT("download/CfjkDYrOrQC_post_1.jpg") , TEXT("download/CfjkDYrOrQC_post_2.jpg") }));
        shower->setPosition(screenW / 2 - rect.right / 2, screenH / 2 - rect.bottom / 2);
        shower->setVisiable(false);
        ::SetDlgItemText(hwnd, IDC_SHOW_DIR, TEXT("download"));

        MoveWindow(hwnd, screenW / 2 - rect.right / 2, screenH / 2 - rect.bottom / 2, rect.right, rect.bottom,TRUE);
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    case WM_COMMAND:
       
        switch (LOWORD(wParam))
        {
        case IDC_SELECT_DIR: {
            wstring dir = TEXT("");
            dir = utils::selectFolder();
            if (dir != TEXT(""))
                ::SetDlgItemText(hwnd, IDC_SHOW_DIR, dir.c_str());
        }
            break;
        case IDC_OPEN_DIR: {

            TCHAR buffer[MAX_PATH] = TEXT("");
            ::GetDlgItemText(hwnd, IDC_SHOW_DIR, buffer, MAX_PATH);
            utils::openFolder(buffer);
        }
            break;
        case IDC_PROFILE_POPUP:
            onProfileButtonClick(hwnd);
            break;
        case IDC_POST_POPUP:
            onPostButtonClick(hwnd);
            break;
        case IDC_STORY_POPUP:
            onStoryButtonClick(hwnd);
            break;
        }

        break;
    case WM_SHOWIMAGE:
        if (lParam > 0)
            shower->setVisiable(TRUE);
        else
            MessageBox(0, TEXT("����ʧ��"), 0, 0);
        
        break;
    default:
        bRet = FALSE;
    }
    return bRet;
}


