#include "framework.h"
#include "ORCS AND HUMANS.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "battleeng.h"
#include "FCheck.h"
#include "ErrH.h"
#include "D2BMPLOADER.h"
#include "gifresizer.h"
#include <chrono>
#include <clocale>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"battleeng.lib")
#pragma comment(lib,"fcheck.lib")
#pragma comment(lib,"errh.lib")
#pragma comment(lib,"d2bmploader.lib")
#pragma comment(lib,"gifresizer.lib")

constexpr wchar_t bWinClassName[]{ L"ORCWar" };
constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t snd_file[]{ L".\\res\\snd\\main.wav" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };

constexpr int mNew{ 1001 };
constexpr int mExit{ 1002 };
constexpr int mLoad{ 1003 };
constexpr int mSave{ 1004 };
constexpr int mHoF{ 1005 };

constexpr int no_record{ 2001 };
constexpr int first_record{ 2002 };
constexpr int record{ 2003 };

//////////////////////////////////////////////////////

WNDCLASS bWinClass{};
HINSTANCE bIns{ nullptr };
HICON mainIco{ nullptr };;
HCURSOR mainCur{ nullptr };
HCURSOR outCur{ nullptr };
HWND bHwnd{ nullptr };
HMENU bBar{ nullptr };
HMENU bMain{ nullptr };
HMENU bStore{ nullptr };
HDC PaintDC{ nullptr };
PAINTSTRUCT bPaint{};
MSG bMsg{};
BOOL bRet{ 0 };

POINT cur_pos{};

D2D1_RECT_F b1Rect{ 40.0f, 0, scr_width / 3 - 50.0f, 50.0f };
D2D1_RECT_F b2Rect{ scr_width / 3 + 40.0f, 0, scr_width * 2 / 3 - 50.0f, 50.0f };
D2D1_RECT_F b3Rect{ scr_width * 2 / 3 + 40.0f, 0, scr_width - 50.0f, 50.0f };

D2D1_RECT_F b1TxtRect{ 80.0f, 10.0f, scr_width / 3 - 50.0f, 50.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3 + 75.0f, 10.0f, scr_width * 2 / 3 - 50.0f, 50.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2 / 3 + 70.0f, 10.0f, scr_width - 50.0f, 50.0f };

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };
ID2D1RadialGradientBrush* b1BckgBrush{ nullptr };
ID2D1RadialGradientBrush* b2BckgBrush{ nullptr };
ID2D1RadialGradientBrush* b3BckgBrush{ nullptr };
ID2D1SolidColorBrush* statBckgBrush{ nullptr };
ID2D1SolidColorBrush* txtBrush{ nullptr };
ID2D1SolidColorBrush* hgltBrush{ nullptr };
ID2D1SolidColorBrush* inactBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmTextFormat{ nullptr };
IDWriteTextFormat* midTextFormat{ nullptr };
IDWriteTextFormat* bigTextFormat{ nullptr };

ID2D1Bitmap* bmpField{ nullptr };
ID2D1Bitmap* bmpGold{ nullptr };
ID2D1Bitmap* bmpLumber{ nullptr };
ID2D1Bitmap* bmpOCastle{ nullptr };
ID2D1Bitmap* bmpRock{ nullptr };
ID2D1Bitmap* bmpStart{ nullptr };
ID2D1Bitmap* bmpTree1{ nullptr };
ID2D1Bitmap* bmpTree2{ nullptr };
ID2D1Bitmap* bmpTree3{ nullptr };
ID2D1Bitmap* bmpArtillery{ nullptr };

ID2D1Bitmap* bmpIntro[60]{ nullptr };

ID2D1Bitmap* bmpExplosion[24]{ nullptr };
ID2D1Bitmap* bmpFireball[4]{ nullptr };
ID2D1Bitmap* bmpHCastle[8]{ nullptr };
ID2D1Bitmap* bmpMine[125]{ nullptr };

ID2D1Bitmap* bmpHArcherL[42]{ nullptr };
ID2D1Bitmap* bmpHArcherR[42]{ nullptr };
ID2D1Bitmap* bmpHKnightL[25]{ nullptr };
ID2D1Bitmap* bmpHKnightR[25]{ nullptr };
ID2D1Bitmap* bmpHWarriorL[4]{ nullptr };
ID2D1Bitmap* bmpHWarriorR[4]{ nullptr };
ID2D1Bitmap* bmpHWorkerL[20]{ nullptr };
ID2D1Bitmap* bmpHWorkerR[20]{ nullptr };

ID2D1Bitmap* bmpOArcherL[19]{ nullptr };
ID2D1Bitmap* bmpOArcherR[19]{ nullptr };
ID2D1Bitmap* bmpOKnightL[31]{ nullptr };
ID2D1Bitmap* bmpOKnightR[31]{ nullptr };
ID2D1Bitmap* bmpOWarriorL[10]{ nullptr };
ID2D1Bitmap* bmpOWarriorR[10]{ nullptr };
ID2D1Bitmap* bmpOWorkerL[40]{ nullptr };
ID2D1Bitmap* bmpOWorkerR[40]{ nullptr };

//////////////////////////////////////////////////////

bool pause{ false };
bool show_help{ false };
bool sound{ true };
bool in_client{ true };
bool b1Hglt{ false };
bool b2Hglt{ false };
bool b3Hglt{ false };
bool name_set{ false };

wchar_t current_player[16]{ L"ONE KING" };

dll::RandIt RandGen;

int score = 0;
int human_gold = 0;
int orc_gold = 0;
int human_lumber = 0;
int orc_lumber = 0;

int active_unit_number = -1;

int scale_x{ 0 };
int scale_y{ 0 };

////////////////////////////////////////////////////////

template<typename T>concept HasRelease = requires(T var)
{
    var.Release();
};
template<HasRelease T>bool ClearMem(T** var)
{
    if ((*var))
    {
        (*var)->Release();
        (*var) = nullptr;
        return true;
    }
    return false;
}
void LogErr(LPCWSTR what)
{
    std::wofstream log(L".\\res\\data\\error.log", std::ios::app);
    log << what << L" Timestamp: " << std::chrono::system_clock::now() << std::endl;
    log.close();
}
void ReleaseResources()
{
    if (!ClearMem(&iFactory))LogErr(L"Error releasing iFactory !");
    if (!ClearMem(&Draw))LogErr(L"Error releasing Draw !");
    if (!ClearMem(&b1BckgBrush))LogErr(L"Error releasing b1BckgBrush !");
    if (!ClearMem(&b2BckgBrush))LogErr(L"Error releasing b2BckgBrush !");
    if (!ClearMem(&b3BckgBrush))LogErr(L"Error releasing b3BckgBrush !");
    if (!ClearMem(&statBckgBrush))LogErr(L"Error releasing statBckgBrush !");
    if (!ClearMem(&txtBrush))LogErr(L"Error releasing txtBrush !");
    if (!ClearMem(&hgltBrush))LogErr(L"Error releasing hgltBrush !");
    if (!ClearMem(&inactBrush))LogErr(L"Error releasing inactBrush !");

    if (!ClearMem(&iWriteFactory))LogErr(L"Error releasing iWriteFactory !");
    if (!ClearMem(&nrmTextFormat))LogErr(L"Error releasing nrmTextFormat !");
    if (!ClearMem(&midTextFormat))LogErr(L"Error releasing midTextFormat !");
    if (!ClearMem(&bigTextFormat))LogErr(L"Error releasing bigTextFormat !");

    if (!ClearMem(&bmpField))LogErr(L"Error releasing bmpField !");
    if (!ClearMem(&bmpGold))LogErr(L"Error releasing bmpGold !");
    if (!ClearMem(&bmpLumber))LogErr(L"Error releasing bmpLumber !");
    if (!ClearMem(&bmpOCastle))LogErr(L"Error releasing bmpOCastle !");
    if (!ClearMem(&bmpRock))LogErr(L"Error releasing bmpRock !");
    if (!ClearMem(&bmpStart))LogErr(L"Error releasing bmpStart !");
    if (!ClearMem(&bmpTree1))LogErr(L"Error releasing bmpTree1 !");
    if (!ClearMem(&bmpTree2))LogErr(L"Error releasing bmpTree2 !");
    if (!ClearMem(&bmpTree3))LogErr(L"Error releasing bmpTree3 !");
    if (!ClearMem(&bmpArtillery))LogErr(L"Error releasing bmpArtillery !");

    for (int i = 0; i < 60; ++i)if (!ClearMem(&bmpIntro[i]))LogErr(L"Error releasing bmpIntro !");

    for (int i = 0; i < 24; ++i)if (!ClearMem(&bmpExplosion[i]))LogErr(L"Error releasing bmpExplosion !");
    for (int i = 0; i < 4; ++i)if (!ClearMem(&bmpFireball[i]))LogErr(L"Error releasing bmpFireball !");
    for (int i = 0; i < 8; ++i)if (!ClearMem(&bmpHCastle[i]))LogErr(L"Error releasing bmpHCastle !");
    for (int i = 0; i < 125; ++i)if (!ClearMem(&bmpMine[i]))LogErr(L"Error releasing bmpMine !");

    for (int i = 0; i < 42; ++i)if (!ClearMem(&bmpHArcherL[i]))LogErr(L"Error releasing bmpHArcherL !");
    for (int i = 0; i < 42; ++i)if (!ClearMem(&bmpHArcherR[i]))LogErr(L"Error releasing bmpHArcherR !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpHKnightL[i]))LogErr(L"Error releasing bmpHKnightL !");
    for (int i = 0; i < 25; ++i)if (!ClearMem(&bmpHKnightR[i]))LogErr(L"Error releasing bmpHKnightR !");
    for (int i = 0; i < 4; ++i)if (!ClearMem(&bmpHWarriorL[i]))LogErr(L"Error releasing bmpHWarriorL !");
    for (int i = 0; i < 4; ++i)if (!ClearMem(&bmpHWarriorR[i]))LogErr(L"Error releasing bmpHWarriorR !");
    for (int i = 0; i < 20; ++i)if (!ClearMem(&bmpHWorkerL[i]))LogErr(L"Error releasing bmpHWorkerL !");
    for (int i = 0; i < 20; ++i)if (!ClearMem(&bmpHWorkerR[i]))LogErr(L"Error releasing bmpHWorkerR !");

    for (int i = 0; i < 19; ++i)if (!ClearMem(&bmpOArcherL[i]))LogErr(L"Error releasing bmpOArcherL !");
    for (int i = 0; i < 19; ++i)if (!ClearMem(&bmpOArcherR[i]))LogErr(L"Error releasing bmpOArcherR !");
    for (int i = 0; i < 31; ++i)if (!ClearMem(&bmpOKnightL[i]))LogErr(L"Error releasing bmpOKnightL !");
    for (int i = 0; i < 31; ++i)if (!ClearMem(&bmpOKnightR[i]))LogErr(L"Error releasing bmpOKnightR !");
    for (int i = 0; i < 10; ++i)if (!ClearMem(&bmpOWarriorL[i]))LogErr(L"Error releasing bmpOWarriorL !");
    for (int i = 0; i < 10; ++i)if (!ClearMem(&bmpOWarriorR[i]))LogErr(L"Error releasing bmpOWarriorR !");
    for (int i = 0; i < 40; ++i)if (!ClearMem(&bmpOWorkerL[i]))LogErr(L"Error releasing bmpOWorkerL !");
    for (int i = 0; i < 40; ++i)if (!ClearMem(&bmpOWorkerR[i]))LogErr(L"Error releasing bmpOWorkerR !");
}
void ErrExit(int what)
{
    MessageBeep(MB_ICONERROR);
    MessageBox(NULL, ErrHandle(what), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

    ReleaseResources();
    std::remove(tmp_file);
    exit(1);
}
int GetIntroFrame()
{
    static int frame{ -1 };

    ++frame;
    if (frame > 59)frame = 0;
    return frame;
}

void GameOver()
{
    PlaySound(NULL, NULL, NULL);





    bMsg.message = WM_QUIT;
    bMsg.wParam = 0;
}
void InitGame()
{
    wcscpy_s(current_player, L"ONE KING");

    score = 0;
    human_gold = 0;
    orc_gold = 0;
    human_lumber = 0;
    orc_lumber = 0;

    active_unit_number = -1;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_INITDIALOG:
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIco));
        return true;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;

        case IDOK:
            if (GetDlgItemText(hwnd, IDC_NAME, current_player, 16) < 1)
            {
                wcscpy_s(current_player, L"ONE KING");
                if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
                MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Ха, ха, ха ! Забраватор !",
                    MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
                EndDialog(hwnd, IDCANCEL);
                break;
            }
            EndDialog(hwnd, IDOK);
            break;
        }
        break;
    }

    return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_CREATE:
        if (bIns)
        {
            bBar = CreateMenu();
            bMain = CreateMenu();
            bStore = CreateMenu();

            AppendMenuW(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
            AppendMenuW(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

            AppendMenuW(bMain, MF_STRING, mNew, L"Нова игра");
            AppendMenuW(bMain, MF_SEPARATOR, NULL, NULL);
            AppendMenuW(bMain, MF_STRING, mExit, L"Изход");

            AppendMenuW(bStore, MF_STRING, mSave, L"Запази игра");
            AppendMenuW(bStore, MF_STRING, mLoad, L"Зареди игра");
            AppendMenuW(bStore, MF_SEPARATOR, NULL, NULL);
            AppendMenuW(bStore, MF_STRING, mHoF, L"Зала на славата");

            SetMenu(hwnd, bBar);
            InitGame();
        }
        break;

    case WM_CLOSE:
        pause = true;
        if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
        if (MessageBox(hwnd, L"Ако излезеш, ще загубиш тази игра !\n\nНаистина ли излизаш ?",
            L"Изход !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
        {
            pause = false;
            break;
        }
        GameOver();
        break;

    case WM_PAINT:
        PaintDC = BeginPaint(hwnd, &bPaint);
        FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(10, 10, 10)));
        EndPaint(hwnd, &bPaint);
        break;

    case WM_SETCURSOR:
        GetCursorPos(&cur_pos);
        ScreenToClient(hwnd, &cur_pos);
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (!in_client)
            {
                in_client = true;
                pause = false;
            }

            if (cur_pos.y * scale_y <= 50)
            {
                if (cur_pos.x * scale_x >= b1Rect.left && cur_pos.x * scale_x <= b1Rect.right)
                {
                    if (!b1Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = true;
                        b2Hglt = false;
                        b3Hglt = false;
                    }
                }
                if (cur_pos.x * scale_x >= b2Rect.left && cur_pos.x * scale_x <= b2Rect.right)
                {
                    if (!b2Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b2Hglt = true;
                        b3Hglt = false;
                    }
                }
                if (cur_pos.x * scale_x >= b3Rect.left && cur_pos.x * scale_x <= b3Rect.right)
                {
                    if (!b3Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b2Hglt = false;
                        b3Hglt = true;
                    }
                }

                SetCursor(outCur);
                return true;
            }
            else
            {
                if (b1Hglt || b2Hglt || b3Hglt)
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b1Hglt = false;
                    b2Hglt = false;
                    b3Hglt = false;
                }
                SetCursor(mainCur);
                return true;
            }
        }
        else
        {
            if (in_client)
            {
                in_client = false;
                pause = true;
            }

            if (b1Hglt || b2Hglt || b3Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b1Hglt = false;
                b2Hglt = false;
                b3Hglt = false;
            }

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return true;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case mNew:
            pause = true;
            if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
            if (MessageBox(hwnd, L"Ако рестартираш, ще загубиш тази игра !\n\nНаистина ли рестартираш ?",
                L"Рестарт !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
            {
                pause = false;
                break;
            }
            InitGame();
            break;

        case mExit:
            SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            break;

        }
        break;



    default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
    }

    return (LRESULT)(FALSE);
}

void CreateResources()
{
    int winx = (int)(GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2));
    int winy = 10;

    if (winx + (int)(scr_width) > GetSystemMetrics(SM_CXSCREEN) || winy + (int)(scr_height) > GetSystemMetrics(SM_CYSCREEN))
        ErrExit(eScreen);

    mainIco = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
    if (!mainIco)ErrExit(eIcon);
    mainCur = LoadCursorFromFileW(L".\\res\\main.ani");
    outCur = LoadCursorFromFileW(L".\\res\\out.ani");
    if (!mainCur || !outCur)ErrExit(eCursor);

    bWinClass.lpszClassName = bWinClassName;
    bWinClass.hInstance = bIns;
    bWinClass.lpfnWndProc = &WinProc;
    bWinClass.hbrBackground = CreateSolidBrush(RGB(10, 10, 10));
    bWinClass.hCursor = mainCur;
    bWinClass.hIcon = mainIco;
    bWinClass.style = CS_DROPSHADOW;

    if (!RegisterClass(&bWinClass))ErrExit(eClass);

    bHwnd = CreateWindow(bWinClassName, L"ХОРА И ОРКИ !", WS_CAPTION | WS_SYSMENU, winx, winy, (int)(scr_width),
        (int)(scr_height), NULL, NULL, bIns, NULL);
    if (!bHwnd)ErrExit(eWindow);
    else
    {
        ShowWindow(bHwnd, SW_SHOWDEFAULT);

        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
        if (hr != S_OK)
        {
            LogErr(L"Error creating D2D1 Factory !");
            ErrExit(eD2D);
        }

        if (iFactory)
        {
            hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
                D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
            if (hr != S_OK)
            {
                LogErr(L"Error creating D2D1 Factory !");
                ErrExit(eD2D);
            }
        }

        if (Draw)
        {
            RECT clR{};
            GetClientRect(bHwnd, &clR);

            D2D1_SIZE_F HwndSize = Draw->GetSize();

            scale_x = (int)(HwndSize.width) / (clR.right - clR.left);
            scale_y = (int)(HwndSize.height) / (clR.bottom - clR.top);

            D2D1_GRADIENT_STOP gStops[2]{};
            ID2D1GradientStopCollection* gColl{ nullptr };

            gStops[0].position = 0;
            gStops[0].color = D2D1::ColorF(D2D1::ColorF::MediumTurquoise);
            gStops[1].position = 1.0f;
            gStops[1].color = D2D1::ColorF(D2D1::ColorF::MidnightBlue);

            hr = Draw->CreateGradientStopCollection(gStops, 2, &gColl);
            if (hr != S_OK)
            {
                LogErr(L"Error creating D2D1 GradientStopCollection !");
                ErrExit(eD2D);
            }

            if (gColl)
            {
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left +
                    (b1Rect.right - b1Rect.left) / 2, 25.0f), D2D1::Point2F(0, 0), (b1Rect.right - b1Rect.left) / 2, 25.0f),
                    gColl, &b1BckgBrush);
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left +
                    (b2Rect.right - b2Rect.left) / 2, 25.0f), D2D1::Point2F(0, 0), (b2Rect.right - b2Rect.left) / 2, 25.0f),
                    gColl, &b2BckgBrush);
                hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left +
                    (b3Rect.right - b3Rect.left) / 2, 25.0f), D2D1::Point2F(0, 0), (b3Rect.right - b3Rect.left) / 2, 25.0f),
                    gColl, &b3BckgBrush);
                if (hr != S_OK)
                {
                    LogErr(L"Error creating D2D1 RadialGradientBckg brushes !");
                    ErrExit(eD2D);
                }

                ClearMem(&gColl);
            }

            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond), &statBckgBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Lime), &txtBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &hgltBrush);
            hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OliveDrab), &inactBrush);

            if (hr != S_OK)
            {
                LogErr(L"Error creating D2D1 SolidColorBrushes !");
                ErrExit(eD2D);
            }

            bmpField = Load(L".\\res\\img\\field\\field.png", Draw);
            if (!bmpField)
            {
                LogErr(L"Error loading bmpField !");
                ErrExit(eD2D);
            }
            bmpGold = Load(L".\\res\\img\\field\\gold.png", Draw);
            if (!bmpGold)
            {
                LogErr(L"Error loading bmpGold !");
                ErrExit(eD2D);
            }
            bmpLumber = Load(L".\\res\\img\\field\\lumber.png", Draw);
            if (!bmpLumber)
            {
                LogErr(L"Error loading bmpLumber !");
                ErrExit(eD2D);
            }
            bmpOCastle = Load(L".\\res\\img\\field\\ocastle.png", Draw);
            if (!bmpOCastle)
            {
                LogErr(L"Error loading bmpOCastle !");
                ErrExit(eD2D);
            }
            bmpRock = Load(L".\\res\\img\\field\\rock.png", Draw);
            if (!bmpRock)
            {
                LogErr(L"Error loading bmpRock !");
                ErrExit(eD2D);
            }
            bmpStart = Load(L".\\res\\img\\field\\start_txt.png", Draw);
            if (!bmpStart)
            {
                LogErr(L"Error loading bmpStart !");
                ErrExit(eD2D);
            }
            bmpTree1 = Load(L".\\res\\img\\field\\Tree1.png", Draw);
            if (!bmpTree1)
            {
                LogErr(L"Error loading bmpTree1 !");
                ErrExit(eD2D);
            }
            bmpTree2 = Load(L".\\res\\img\\field\\Tree2.png", Draw);
            if (!bmpTree2)
            {
                LogErr(L"Error loading bmpTree2 !");
                ErrExit(eD2D);
            }
            bmpTree3 = Load(L".\\res\\img\\field\\Tree3.png", Draw);
            if (!bmpTree3)
            {
                LogErr(L"Error loading bmpTree3 !");
                ErrExit(eD2D);
            }
            bmpArtillery = Load(L".\\res\\img\\field\\Artillery.png", Draw);
            if (!bmpArtillery)
            {
                LogErr(L"Error loading bmpArtillery !");
                ErrExit(eD2D);
            }

            for (int i = 0; i < 60; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\field\\intro\\0" };
                wchar_t add[5]{ L"\0" };

                if (i < 10)wcscat_s(name, L"0");

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpIntro[i] = Load(name, Draw);
                if (!bmpIntro[i])
                {
                    LogErr(L"Error loading bmpIntro !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 24; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\field\\explosion\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpExplosion[i] = Load(name, Draw);
                if (!bmpExplosion[i])
                {
                    LogErr(L"Error loading bmpExplosion !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\field\\fireball\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpFireball[i] = Load(name, Draw);
                if (!bmpFireball[i])
                {
                    LogErr(L"Error loading bmpFireball !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 8; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\field\\hcastle\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHCastle[i] = Load(name, Draw);
                if (!bmpHCastle[i])
                {
                    LogErr(L"Error loading bmpHCastle !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 125; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\field\\mine\\0" };
                wchar_t add[5]{ L"\0" };

                if (i < 10)wcscat_s(name, L"00");
                else if (i < 100)wcscat_s(name, L"0");

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpMine[i] = Load(name, Draw);
                if (!bmpMine[i])
                {
                    LogErr(L"Error loading bmpMine !");
                    ErrExit(eD2D);
                }
            }

            ////////////////////////////////////////

            for (int i = 0; i < 42; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\archer\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHArcherL[i] = Load(name, Draw);
                if (!bmpHArcherL[i])
                {
                    LogErr(L"Error loading bmpHArcherL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 42; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\archer\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHArcherR[i] = Load(name, Draw);
                if (!bmpHArcherR[i])
                {
                    LogErr(L"Error loading bmpHArcherR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\knight\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHKnightL[i] = Load(name, Draw);
                if (!bmpHKnightL[i])
                {
                    LogErr(L"Error loading bmpHKnightL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 25; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\knight\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHKnightR[i] = Load(name, Draw);
                if (!bmpHKnightR[i])
                {
                    LogErr(L"Error loading bmpHKnightR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 4; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\warrior\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHWarriorL[i] = Load(name, Draw);
                if (!bmpHWarriorL[i])
                {
                    LogErr(L"Error loading bmpHWarriorL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\warrior\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHWarriorR[i] = Load(name, Draw);
                if (!bmpHWarriorR[i])
                {
                    LogErr(L"Error loading bmpHWarriorR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 20; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\worker\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHWorkerL[i] = Load(name, Draw);
                if (!bmpHWorkerL[i])
                {
                    LogErr(L"Error loading bmpHWorkerL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 20; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\humans\\worker\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpHWorkerR[i] = Load(name, Draw);
                if (!bmpHWorkerR[i])
                {
                    LogErr(L"Error loading bmpHWorkerR !");
                    ErrExit(eD2D);
                }
            }

            ///////////////////////////////////////////

            for (int i = 0; i < 19; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\archer\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOArcherL[i] = Load(name, Draw);
                if (!bmpOArcherL[i])
                {
                    LogErr(L"Error loading bmpOArcherL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 19; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\archer\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOArcherR[i] = Load(name, Draw);
                if (!bmpOArcherR[i])
                {
                    LogErr(L"Error loading bmpOArcherR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 31; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\tank\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOKnightL[i] = Load(name, Draw);
                if (!bmpOKnightL[i])
                {
                    LogErr(L"Error loading bmpOKnightL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 31; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\tank\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOKnightR[i] = Load(name, Draw);
                if (!bmpOKnightR[i])
                {
                    LogErr(L"Error loading bmpOKnightR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 10; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\warrior\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOWarriorL[i] = Load(name, Draw);
                if (!bmpOWarriorL[i])
                {
                    LogErr(L"Error loading bmpOWarriorL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 10; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\warrior\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOWarriorR[i] = Load(name, Draw);
                if (!bmpOWarriorR[i])
                {
                    LogErr(L"Error loading bmpOWarriorR !");
                    ErrExit(eD2D);
                }
            }

            for (int i = 0; i < 40; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\worker\\l\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOWorkerL[i] = Load(name, Draw);
                if (!bmpOWorkerL[i])
                {
                    LogErr(L"Error loading bmpOWorkerL !");
                    ErrExit(eD2D);
                }
            }
            for (int i = 0; i < 40; ++i)
            {
                wchar_t name[100]{ L".\\res\\img\\orcs\\worker\\r\\" };
                wchar_t add[5]{ L"\0" };

                wsprintf(add, L"%d", i);
                wcscat_s(name, add);
                wcscat_s(name, L".png");

                bmpOWorkerR[i] = Load(name, Draw);
                if (!bmpOWorkerR[i])
                {
                    LogErr(L"Error loading bmpOWorkerR !");
                    ErrExit(eD2D);
                }
            }
        }

        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&iWriteFactory));
        if (hr != S_OK)
        {
            LogErr(L"Error creating D2D1 WriteFactory !");
            ErrExit(eD2D);
        }

        if (iWriteFactory)
        {
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &nrmTextFormat);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"", &midTextFormat);
            hr = iWriteFactory->CreateTextFormat(L"SEGOE PRINT", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
                DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigTextFormat);
            if (hr != S_OK)
            {
                LogErr(L"Error creating D2D1 WriteTextFormats !");
                ErrExit(eD2D);
            }
        }

    }

    PlaySound(L".\\res\\snd\\intro.wav", NULL, SND_ASYNC);

    for (int i = 0; i < 150; i++)
    {
        Draw->BeginDraw();
        Draw->DrawBitmap(bmpIntro[GetIntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
        Draw->DrawBitmap(bmpStart, D2D1::RectF(100.0f, 100.0f, 800.0f, 394.0f));
        Draw->EndDraw();
        Sleep(30);
    }
}

///////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    std::setlocale(LC_ALL, "");

    bIns = hInstance;
    if (!bIns)
    {
        LogErr(L"Error in Windows hInstance");
        ErrExit(eClass);
    }
    else
    {
        int result = 0;
        CheckFile(Ltmp_file, &result);
        if (result == FILE_EXIST)ErrExit(eStarted);
        else
        {
            std::wofstream start(Ltmp_file);
            start << L"Game started at: " << std::chrono::system_clock::now();
            start.close();
        }
    }

    CreateResources();

    while (bMsg.message != WM_QUIT)
    {
        if ((bRet = PeekMessage(&bMsg, bHwnd, NULL, NULL, PM_REMOVE)) != 0)
        {
            if (bRet == -1)ErrExit(eMsg);
            TranslateMessage(&bMsg);
            DispatchMessageW(&bMsg);
        }

        if (pause)
        {
            if (show_help)continue;
            Draw->BeginDraw();
            Draw->DrawBitmap(bmpIntro[GetIntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
            if (txtBrush && bigTextFormat)
                Draw->DrawTextW(L"ПАУЗА", 6, bigTextFormat, D2D1::RectF(scr_width / 2 - 100.0f, scr_height / 2 - 50.0f,
                    scr_width, scr_height), txtBrush);
            Draw->EndDraw();
            continue;
        }

        /////////////////////////////////////////////////////////////////


        // DRAW THINGS ***********************************************

        Draw->BeginDraw();
        if (statBckgBrush && b1BckgBrush && b2BckgBrush && b3BckgBrush && txtBrush
            && hgltBrush && inactBrush && nrmTextFormat)
        {
            Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), statBckgBrush);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b1Rect, 15.0f, 20.0f), b1BckgBrush);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b2Rect, 15.0f, 20.0f), b2BckgBrush);
            Draw->FillRoundedRectangle(D2D1::RoundedRect(b3Rect, 15.0f, 20.0f), b3BckgBrush);

            if (name_set)Draw->DrawTextW(L"ИМЕ НА КРАЛ", 12, nrmTextFormat, b1TxtRect, inactBrush);
            else
            {
                if (!b1Hglt)Draw->DrawTextW(L"ИМЕ НА КРАЛ", 12, nrmTextFormat, b1TxtRect, txtBrush);
                else Draw->DrawTextW(L"ИМЕ НА КРАЛ", 12, nrmTextFormat, b1TxtRect, hgltBrush);
            }
            if (!b2Hglt)Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, b2TxtRect, txtBrush);
            else Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, b2TxtRect, hgltBrush);
            if (!b3Hglt)Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, b3TxtRect, txtBrush);
            else Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, b3TxtRect, hgltBrush);
        }

        Draw->DrawBitmap(bmpField, D2D1::RectF(0, 50.0f, scr_width, scr_height));

        //////////////////////////////////////////////////////////////


        Draw->EndDraw();
    }

    ReleaseResources();
    std::remove(tmp_file);
    return (int) bMsg.wParam;
}