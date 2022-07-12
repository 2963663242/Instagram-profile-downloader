//#include <tchar.h>
//
//#include <windows.h>
//
//
//
//HINSTANCE _HInstance;                              // 应用程序句柄
//
//TCHAR _Title[] = _T("简单文本框");                 // 定义窗口的标题
//
//
//
//TCHAR _WindowClass[] = _T("MySimpleTextBoxApp");// 主窗口类名
//
//ATOM _RegisterClass();                             // 注册主窗口类
//
//HWND _CreateWindow(int nCmdShow);                  // 创建主窗口
//
//LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);   // 主窗口消息处理函数
//
//
//
//TCHAR _TextBoxClass[] = _T("MySimpleTextBox"); // 文本框的类名
//
//ATOM _RegisterTextBoxClass();                      // 注册文本框的类
//
//HWND _CreateTextBoxWindow(HWND hParentWnd);        // 创建文本框
//
//LRESULT CALLBACK _TextBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 文本框窗口消息处理函数
//
//void _DrawText(HDC hDC);                           // 绘制文本
//
//void _SetCaretPos(HWND hWnd);                      // 设置光标位置
//
//void _UpdateWindow(HWND hWnd);                     // 更新窗口
//
//
//
//
//
//// 一些常量定义
//
//#define MAINWINDOW_WIDTH    400      // 主窗口宽度
//
//#define MAINWINDOW_HEIGHT   200      // 主窗口高度
//
//#define TEXTBOX_WIDTH       300      // 文本框宽度
//
//#define TEXTBOX_HEIGHT      20       // 文本框高度
//
//#define TEXTBOX_MAXLENGTH   1024 // 文本框中文本的最大长度
//
//
//
//TCHAR _String[TEXTBOX_MAXLENGTH + 1] = _T("");     // 文本
//
//int    _StringPosition = ::_tcslen(_String);        // 光标插入点所在的位置
//
//
//
//int APIENTRY _tWinMain(HINSTANCE hInstance,        // 当前的应用程序句柄
//
//    HINSTANCE hPrevInstance, // 前一个应用程序实例的句柄（在Win32上，始终为NULL）
//
//    LPTSTR lpCmdLine,        // 命令行参数
//
//    int        nCmdShow     // 窗口的显示样式
//
//)
//
//{
//
//    _HInstance = hInstance;
//
//
//
//    _RegisterClass();                         // 注册窗口类
//
//    if (_CreateWindow(nCmdShow) == NULL)       // 创建窗口
//
//        return FALSE;
//
//
//
//    MSG msg;
//
//    while (::GetMessage(&msg, NULL, 0, 0))    // 从消息队列中获取消息
//
//    {
//
//        ::TranslateMessage(&msg);            // 转译一些特殊的消息
//
//        ::DispatchMessage(&msg);             // 执行消息处理
//
//    }
//
//
//
//    return (int)msg.wParam;
//
//}
//
//
//
//
//
//// 注册应用程序窗口类
//
//ATOM _RegisterClass()
//
//{
//
//    WNDCLASSEX wc;
//
//    ::ZeroMemory(&wc, sizeof(wc));                 // 作为一步清空，是为了让未赋值的字段的默认值为（或NULL）
//
//
//
//    wc.cbSize = sizeof(wc);
//
//    wc.style = CS_HREDRAW | CS_VREDRAW;  // 指定当窗口横向和纵向的尺寸发生变化时都会重绘窗口
//
//    wc.hInstance = _HInstance;
//
//    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);  // 指定主窗口背景为“工作区域”系统颜色
//
//    wc.lpszClassName = _WindowClass;          // 此为要注册的类名，创建窗口时要以此类名为标识符
//
//    wc.lpfnWndProc = _WndProc;                      // 此为处理窗口消息的函数
//
//
//
//    return ::RegisterClassEx(&wc);                 // 调用API函数注册窗口类
//
//}
//
//
//
//// 创建窗口
//
//HWND _CreateWindow(int nCmdShow)
//
//{
//
//    HWND hWnd = ::CreateWindow(_WindowClass, _Title, WS_OVERLAPPEDWINDOW,
//
//        CW_USEDEFAULT, CW_USEDEFAULT, MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT, NULL, NULL, _HInstance, NULL);
//
//
//
//    if (hWnd == NULL)
//
//        return NULL;
//
//
//
//    ::ShowWindow(hWnd, nCmdShow);
//
//    ::UpdateWindow(hWnd);
//
//
//
//    return hWnd;
//
//}
//
//
//
//// 窗口处理过程
//
//LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//{
//
//    static HWND hTextBoxWnd;
//
//
//
//    switch (message)
//
//    {
//
//    case WM_CREATE: {
//
//        _RegisterTextBoxClass();    // 注册文本框的类
//
//        hTextBoxWnd = _CreateTextBoxWindow(hWnd); // 创建文本框
//
//    } break;
//
//
//
//    case WM_ACTIVATE:                // 当窗口被激活时，将焦点设置在文本框上
//
//        ::SetFocus(hTextBoxWnd);
//
//        break;
//
//
//
//    case WM_SETCURSOR: {  // 设置光标形状
//
//        static HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
//
//        ::SetCursor(hCursor);
//
//    } break;
//
//
//
//    case WM_DESTROY:   // 应用程序被关闭
//
//        ::PostQuitMessage(0);
//
//        break;
//
//
//
//    default:
//
//        return ::DefWindowProc(hWnd, message, wParam, lParam);
//
//    }
//
//
//
//    return (LRESULT)0;
//
//}
//
//
//
//// 注册文本框的类
//
//ATOM _RegisterTextBoxClass()
//
//{
//
//    WNDCLASSEX wc;
//
//    ::ZeroMemory(&wc, sizeof(wc));
//
//
//
//    wc.cbSize = sizeof(wc);
//
//    wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;   // 指定当窗口尺寸发生变化时重绘窗口，并且响应鼠标双击事件
//
//    wc.hInstance = _HInstance;
//
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 指定窗口背景颜色为系统颜色“窗口背景”
//
//    wc.lpszClassName = _TextBoxClass;                  // 指定要注册的窗口类名，创建窗口时要以此类名为标识符
//
//    wc.lpfnWndProc = _TextBoxWndProc;               // 处理窗口消息的函数
//
//
//
//    return ::RegisterClassEx(&wc);                     // 调用API函数注册文本框窗口
//
//}
//
//
//
//
//
//// 创建文本框
//
//HWND _CreateTextBoxWindow(HWND hParentWnd)
//
//{
//
//    // 之下代码是为了让文本框显示在父窗口中央，而计算位置
//
//    RECT parentWndRect;
//
//    ::GetClientRect(hParentWnd, &parentWndRect);  // 获取父窗口客户区的位置
//
//    int left = (parentWndRect.right - TEXTBOX_WIDTH) / 2, top = (parentWndRect.bottom - TEXTBOX_HEIGHT) / 2;
//
//
//
//    // 创建文本框
//
//    HWND hWnd = ::CreateWindow(_TextBoxClass, NULL, WS_CHILDWINDOW | WS_VISIBLE,
//
//        left, top, TEXTBOX_WIDTH, TEXTBOX_HEIGHT,
//
//        hParentWnd, NULL, _HInstance, NULL);
//
//
//
//    return hWnd;
//
//}
//
//
//
//// 文本框消息的处理过程
//
//LRESULT CALLBACK _TextBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//{
//
//    switch (message)
//
//    {
//
//    case WM_PAINT: {  // 绘制这里之所以加一对大括号，是为了让之下定义的变量局部化
//
//
//
//        static PAINTSTRUCT ps;
//
//        static RECT rect;
//
//        HDC hDC = ::BeginPaint(hWnd, &ps);  // 开始绘制操作
//
//
//
//        ::GetClientRect(hWnd, &rect);        // 获取客户区的尺寸
//
//        ::DrawEdge(hDC, &rect, EDGE_SUNKEN, BF_RECT);  // 绘制边框，EDGE_SUNKEN表示绘制样式为内嵌样式，BF_RECT表示绘制矩形边框
//
//        _DrawText(hDC);                      // 绘制文本
//
//        ::EndPaint(hWnd, &ps);               // 结束绘制操作
//
//
//
//    } break;
//
//
//
//    case WM_SETFOCUS: {    // 获得焦点
//
//        ::CreateCaret(hWnd, (HBITMAP)NULL, 1, TEXTBOX_HEIGHT - 5);     // 创建光标
//
//        _SetCaretPos(hWnd);                            // 设置光标位置
//
//        ::ShowCaret(hWnd);                   // 显示光标
//
//    } break;
//
//
//
//    case WM_KILLFOCUS: // 失去焦点
//
//        ::HideCaret(hWnd);                   // 隐藏光标
//
//        ::DestroyCaret();                    // 销毁光标
//
//        break;
//
//
//
//    case WM_SETCURSOR: {  // 设置光标形状
//
//        static HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
//
//        ::SetCursor(hCursor);
//
//    } break;
//
//
//
//    case WM_CHAR: {    // 字符消息
//
//        TCHAR code = (TCHAR)wParam;
//
//        int len = ::_tcslen(_String);
//
//        if (code < (TCHAR)' ' || len >= TEXTBOX_MAXLENGTH)
//
//            return 0;
//
//
//
//        ::MoveMemory(_String + _StringPosition + 1, _String + _StringPosition, (len - _StringPosition + 1) * sizeof(TCHAR));
//
//        _String[_StringPosition++] = code;
//
//
//
//        _UpdateWindow(hWnd);
//
//        _SetCaretPos(hWnd);
//
//
//
//    } break;
//
//
//
//    case WM_KEYDOWN: {  // 键按下消息
//
//        TCHAR code = (TCHAR)wParam;
//
//
//
//        switch (code)
//
//        {
//
//        case VK_LEFT: // 左光标键
//
//            if (_StringPosition > 0)
//
//                _StringPosition--;
//
//            break;
//
//
//
//        case VK_RIGHT:     // 右光标键
//
//            if (_StringPosition < (int)::_tcslen(_String))
//
//                _StringPosition++;
//
//            break;
//
//
//
//        case VK_HOME: // HOME 键
//
//            _StringPosition = 0;
//
//            break;
//
//
//
//        case VK_END:  // END 键
//
//            _StringPosition = ::_tcslen(_String);
//
//            break;
//
//
//
//        case VK_BACK: // 退格键
//
//            if (_StringPosition > 0)
//
//            {
//
//                ::MoveMemory(_String + _StringPosition - 1, _String + _StringPosition, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
//
//                _StringPosition--;
//
//                _UpdateWindow(hWnd);
//
//            }
//
//            break;
//
//
//
//        case VK_DELETE: {  // 删除键
//
//            int len = ::_tcslen(_String);
//
//            if (_StringPosition < len)
//
//            {
//
//                ::MoveMemory(_String + _StringPosition, _String + _StringPosition + 1, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
//
//                _UpdateWindow(hWnd);
//
//            }
//
//
//
//        } break;
//
//
//
//        }
//
//
//
//        _SetCaretPos(hWnd);
//
//
//
//    } break;
//
//
//
//    case WM_LBUTTONDOWN: {  // 鼠标单击，设置光标位置
//
//        int x = LOWORD(lParam);
//
//        HDC hDc = ::GetDC(hWnd);
//
//
//
//        int strLen = ::_tcslen(_String), strPos = 0;
//
//        SIZE size;
//
//
//
//        for (strPos = 0; strPos < strLen; strPos++)
//
//        {
//
//            ::GetTextExtentPoint(hDc, _String, strPos, &size);
//
//
//
//            if (size.cx + 4 >= x)
//
//                break;
//
//        }
//
//
//
//        _StringPosition = strPos;
//
//        ::GetTextExtentPoint(hDc, _String, strPos, &size);
//
//        ::SetCaretPos(size.cx + 4, 3);
//
//
//
//        ::ReleaseDC(hWnd, hDc);
//
//
//
//    } break;
//
//
//
//    default:
//
//        return ::DefWindowProc(hWnd, message, wParam, lParam);
//
//    }
//
//
//
//    return (LRESULT)0;
//
//}
//
//
//
//// 更新窗口
//
//void _UpdateWindow(HWND hWnd)
//
//{
//
//    RECT rect;
//
//    ::GetClientRect(hWnd, &rect);
//
//    ::InvalidateRect(hWnd, &rect, TRUE);
//
//    ::UpdateWindow(hWnd);
//
//}
//
//
//
//// 绘制文本
//
//void _DrawText(HDC hDC)
//
//{
//
//    int len = ::_tcslen(_String);
//
//    ::TextOut(hDC, 4, 2, _String, len);
//
//}
//
//
//
//// 设置光标位置
//
//void _SetCaretPos(HWND hWnd)
//
//{
//
//    HDC hDC = ::GetDC(hWnd);
//
//
//
//    SIZE size;
//
//    ::GetTextExtentPoint(hDC, _String, _StringPosition, &size);
//
//    ::SetCaretPos(4 + size.cx, 3);
//
//
//
//    ::ReleaseDC(hWnd, hDC);
//
//
//
//}
