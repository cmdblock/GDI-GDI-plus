// GDI绘图一.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GDI和GDI Plus.h"
#include <iostream>
#include <objidl.h>
#include <gdiplus.h>
#include <math.h>
#include <assert.h>
#include "MemDC.h"

#define BLEND_COUNT 30

#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;


#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HBITMAP hbmpBack = NULL;

//画背景
void PaintImage(HWND hWnd, HDC hdc);

//画文字
void PaintText(HWND hWnd, HDC hdc);

//画线
void PaintLine(HDC hdc);

//画线
void PaintLinePlus(HDC hdc);

//画圆
void PaintCircle(HDC hdc);

//画矩形
void PaintRecPlus(HDC hdc);

//画椭圆
void PaintEllipsePlus(HDC hdc);

//画文字
void PaintTextPlus(HDC hdc);

//画饼图
void PaintPie(HDC hdc);

//普通平铺图像
void TileA(HWND hWnd);

//减区法平铺图像
void TileB(HWND hWnd);

//位图填充画刷平铺图像
void TileC(HWND hWnd);

//坐标起始点改变平铺图像
void TileD(HWND hWnd);

//画百叶窗动画
void PaintBlind(HWND hWnd);

//画位图到DC上
void DrawBitmap(HDC hdc, int x, int y, int width, int height, HBITMAP hBitmap);

//保存位图
BOOL SaveBMP(HBITMAP hbm, const std::wstring& filePath);

//保存位图
void SaveBitmap(HWND hWnd, std::wstring fileName);

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GDI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDI));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GDI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hbmpBack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK));

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...

			//双缓冲技术
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			{//加括号实现析构
				//创建类
				CMemDC dcMem(hdc, rcClient);

				//GDI画图
				PaintImage(hWnd, dcMem);
				PaintText(hWnd, dcMem);
				PaintLine(dcMem);
				PaintCircle(dcMem);

				//GDI+画图
				PaintLinePlus(dcMem);
				PaintRecPlus(dcMem);
				PaintEllipsePlus(dcMem);
				PaintTextPlus(dcMem);
				PaintPie(dcMem);
			} //加括号实现析构

			  //释放hdc
			EndPaint(hWnd, &ps);
		}
		break;
		//case WM_ERASEBKGND:
		//	break;
		case WM_CHAR:
			switch (wParam)
			{
				case (int)'u':
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
					break;
				case (int)'a':
					TileA(hWnd);
					break;
				case (int)'b':
					TileB(hWnd);
					break;
				case (int)'c':
					TileC(hWnd);
					break;
				case (int)'d':
					TileD(hWnd);
					break;
				case (int)'e':
					PaintBlind(hWnd);
					break;
				case (int)'f':
					SaveBitmap(hWnd, L"image.bmp");
					break;
				default:
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}


void PaintImage(HWND hWnd, HDC hdc)
{
	//创建内存DC
	HDC hDcMem = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hDcMem, hbmpBack);

	//获得bmp图像相关信息
	BITMAP bmp;
	GetObject(hbmpBack, sizeof(BITMAP), &bmp);

	//普通填充
	//BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hDcMem, 0, 0, SRCCOPY);

	//拉伸填充
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int nWith = rcClient.right - rcClient.left;
	int nHeight = rcClient.bottom - rcClient.top;
	StretchBlt(hdc, 0, 0, nWith, nHeight, hDcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	//删除掉DC
	SelectObject(hDcMem, hOldBmp);
	DeleteDC(hDcMem);
}

void PaintText(HWND hWnd, HDC hdc)
{
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(LOGFONT));
	logfont.lfWidth = 20;
	logfont.lfHeight = 40;
	logfont.lfCharSet = GB2312_CHARSET;
	_tcscpy(logfont.lfFaceName, _T("宋体"));

	HFONT hFont;
	hFont = CreateFontIndirect(&logfont);

	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	COLORREF clrOldText = SetTextColor(hdc, RGB(0, 0, 128));

	SetBkMode(hdc, TRANSPARENT); //设置背景透明

	std::wstring title = L"VC知识库网站统计流量报表"; //这里用的是wstring
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	DrawText(hdc, title.c_str(), title.length(), &rcClient, DT_CENTER | DT_TOP | DT_SINGLELINE);

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	SetTextColor(hdc, clrOldText);

	//第二种文本绘制方法
	TextOut(hdc, 5, 100, _T("7000"), 4);
}

void PaintLine(HDC hdc)
{
	HPEN hPen = CreatePen(PS_DASH, 2, RGB(120, 240, 50));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, 78, 315, NULL);
	LineTo(hdc, 150, 170);
	LineTo(hdc, 182, 239);
	LineTo(hdc, 240, 114);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void PaintCircle(HDC hdc)
{
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(224, 231, 60);

	HBRUSH hBrush = CreateBrushIndirect(&logBrush);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, 300, 100, 400, 200);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}


void PaintRecPlus(HDC hdc)
{
	Graphics graphics(hdc);

	// Create a Pen object.
	Pen blackPen(Color(255, 0, 0, 0), 3);

	// Create a RectF object.
	//RectF rect(0.0f, 0.0f, 300.0f, 300.0f);

	// Draw rect.
	graphics.DrawRectangle(&blackPen, 0, 0, 400, 400);
}

void PaintEllipsePlus(HDC hdc)
{
	HBRUSH hOldBrush;
	HPEN hOldPen;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;

	//画第一个圆
	logBrush.lbColor = RGB(179, 227, 255);
	HBRUSH hBrushA = CreateBrushIndirect(&logBrush);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrushA);
	hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Ellipse(hdc, 300, 200, 300 + 250, 200 + 250);
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hBrushA); //系统pen不需要删除

	//画第二个圆
	logBrush.lbColor = RGB(128, 208, 255);
	HBRUSH hBrushB = CreateBrushIndirect(&logBrush);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrushB);
	hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Ellipse(hdc, 300, 200, 300 + 160, 200 + 160);
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hBrushB); //系统pen不需要删除

	//画第三个圆，用GDI+
	Graphics graphics(hdc);
	SolidBrush BrushC(Color(120, 255, 0, 0));
	graphics.FillEllipse(&BrushC, 300, 200, 60, 60);
}

void PaintPie(HDC hdc)
{
	//备份DC
	int nSavedDC = SaveDC(hdc);

	HBRUSH hbrA = CreateSolidBrush(RGB(78, 207, 255));
	HBRUSH hbrB = CreateSolidBrush(RGB(255, 174, 0));
	HBRUSH hbrC = CreateSolidBrush(RGB(199, 78, 255));

	//画饼A
	SelectObject(hdc, hbrA);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	Pie(hdc, 470, 390, 470 + 220, 390 + 220, 582, 391, 629, 600);
	SelectObject(hdc, hbrB);
	DeleteObject(hbrA);

	//画饼B
	Pie(hdc, 470, 390, 470 + 220, 390 + 220, 692, 500, 582, 391);
	SelectObject(hdc, hbrC);
	DeleteObject(hbrB);

	//画饼C
	Pie(hdc, 470, 390, 470 + 220, 390 + 220, 662, 578, 692, 500);

	//还原DC
	RestoreDC(hdc, nSavedDC);
	DeleteObject(hbrC);

	//画饼D
	Graphics graphics(hdc);
	SolidBrush solidBrushD(Color(255, 156, 255, 0));
	graphics.FillPie(&solidBrushD, 470, 390, 219, 219, 40, 25);

	//画饼上面的文字
	TextOut(hdc, 500, 510, L"A", 1);
	TextOut(hdc, 625, 450, L"B", 1);
	TextOut(hdc, 650, 520, L"C", 1);
	TextOut(hdc, 623, 559, L"D", 1);
}

void PaintLinePlus(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);
}

void PaintTextPlus(HDC hdc)
{
	Graphics graphics(hdc);
	Gdiplus::Font myFont(L"xiaonantongxue", 24);
	SolidBrush mySolidBrush(Color(255, 255, 255, 255));
	graphics.DrawString(L"GDI+文字", -1, &myFont, PointF(400, 200), &mySolidBrush);
}

void TileA(HWND hWnd)
{
	//获取窗口DC
	HDC hdc = GetDC(hWnd);

	//保存DC
	int isdc = SaveDC(hdc);

	//加载位图
	HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	//创建内存DC
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, hBitmap);

	//获取位图大小
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	int iNumX = 300 / bitmap.bmWidth + 1;
	int iNumY = 300 / bitmap.bmHeight + 1;

	for (int i = 0; i < iNumY; i++)
	{
		for (int j = 0; j < iNumX; j++)
		{
			//位图平铺
			BitBlt(hdc, j * bitmap.bmWidth, i * bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight,
				hMemDC, 0, 0, SRCCOPY);
		}
	}

	HPEN hPenRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPenRed);

	SelectObject(hdc, GetStockObject(NULL_BRUSH));

	//创建矩形区域
	Rectangle(hdc, 0, 0, 300, 300);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPenRed);

	//删除位图
	DeleteObject(hBitmap);

	//删除内存DC
	DeleteDC(hMemDC);

	//恢复DC
	RestoreDC(hdc, isdc);

	//释放DC
	ReleaseDC(hWnd, hdc);
}

void TileB(HWND hWnd)
{
	//获取窗口DC
	HDC hdc = GetDC(hWnd);

	//保存DC
	int isdc = SaveDC(hdc);

	//加载位图
	HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	//创建内存DC
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, hBitmap);

	//创建矩形区域
	HRGN hRgn = CreateRectRgn(0, 0, 300, 300);

	//选择剪切区域
	SelectClipRgn(hdc, hRgn);

	//获取位图大小
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	int iNumX = 300 / bitmap.bmWidth + 1;
	int iNumY = 300 / bitmap.bmHeight + 1;

	for (int i = 0; i < iNumY; i++)
	{
		for (int j = 0; j < iNumX; j++)
		{
			//位图平铺
			BitBlt(hdc, j * bitmap.bmWidth, i * bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight,
				hMemDC, 0, 0, SRCCOPY);
		}
	}

	HPEN hPenRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPenRed);

	SelectObject(hdc, GetStockObject(NULL_BRUSH));

	//画一个矩形
	Rectangle(hdc, 0, 0, 300, 300);

	SelectObject(hdc, hPenOld);
	DeleteObject(hPenRed);

	//删除位图
	DeleteObject(hBitmap);

	//删除内存DC
	DeleteDC(hMemDC);

	//恢复DC
	RestoreDC(hdc, isdc);

	//删除剪切区域
	DeleteObject(hRgn);

	//释放DC
	ReleaseDC(hWnd, hdc);
}

void TileC(HWND hWnd)
{
	//获取窗口DC
	HDC hdc = GetDC(hWnd);

	//保存DC
	int isdc = SaveDC(hdc);

	//加载位图
	HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	//创建纹理画刷
	HBRUSH hBrush = CreatePatternBrush(hBitmap);

	//选择画刷
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//画笔
	HPEN hPenRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPenRed);

	//画一个矩形
	Rectangle(hdc, 0, 0, 300, 300);

	//删除画笔
	SelectObject(hdc, hPenOld);
	DeleteObject(hPenRed);

	//删除画刷
	SelectObject(hdc, hOldBrush);
	DeleteObject(hOldBrush);

	//删除位图
	DeleteObject(hBitmap);

	//恢复DC
	RestoreDC(hdc, isdc);

	//释放DC
	ReleaseDC(hWnd, hdc);
}

void TileD(HWND hWnd)
{
	//获取窗口DC
	HDC hdc = GetDC(hWnd);

	//保存DC
	int isdc = SaveDC(hdc);

	//偏移初始坐标
	SetViewportOrgEx(hdc, 300, 300, NULL);

	//加载位图
	HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	//创建纹理画刷
	HBRUSH hBrush = CreatePatternBrush(hBitmap);

	//选择画刷
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//画笔
	HPEN hPenRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPenOld = (HPEN)SelectObject(hdc, hPenRed);

	//画一个矩形
	Rectangle(hdc, 0, 0, 300, 300);

	//删除画笔
	SelectObject(hdc, hPenOld);
	DeleteObject(hPenRed);

	//删除画刷
	SelectObject(hdc, hOldBrush);
	DeleteObject(hOldBrush);

	//删除位图
	DeleteObject(hBitmap);

	//恢复DC
	RestoreDC(hdc, isdc);

	//释放DC
	ReleaseDC(hWnd, hdc);
}

void PaintBlind(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	RECT rect;
	GetClientRect(hWnd, &rect);

	//把屏幕中的DC写入到内存DC
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmapSrc = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hMemDC, hBitmapSrc);
	BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);

	//把hBitmapSrc中的数据拷贝到pBitmapDataSrc内存中
	BYTE *pBitmapDataSrc = new BYTE[(rect.right - rect.left) * (rect.bottom - rect.top) * 4]; //因为是ARBG(带透明通道)，所以从乘以4
	GetBitmapBits(hBitmapSrc, (rect.right - rect.left) * (rect.bottom - rect.top) * 4, pBitmapDataSrc);

	//把窗口填充成白色
	SelectObject(hdc, GetStockObject(WHITE_PEN));
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	//百叶窗的位图句柄
	HBITMAP hBitmaps[BLEND_COUNT];
	for (int i = 0; i < BLEND_COUNT; i++)
	{
		BITMAPINFO* pbi;
		BYTE* pMem;
		BYTE* pBits;
		DWORD addrs;
		//LPBYTE lpBitmapBits;

		pMem = (BYTE*)malloc(sizeof(BITMAPINFO));
		pbi = (BITMAPINFO*)pMem;
		pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbi->bmiHeader.biHeight = (rect.bottom - rect.top) / BLEND_COUNT;
		pbi->bmiHeader.biWidth = rect.right - rect.left;
		pbi->bmiHeader.biPlanes = 1;
		pbi->bmiHeader.biBitCount = 32;
		pbi->bmiHeader.biCompression = BI_RGB;

		hBitmaps[i] = CreateDIBSection(hdc, pbi, DIB_RGB_COLORS, (void **)&addrs, NULL, 0);
		if (hBitmaps[i] == NULL)
			MessageBox(NULL, L"创建失败", L"标题", MB_OK);

		free(pMem);

		//获取百叶窗位图的数据地址
		pBits = (BYTE*)(addrs);

		//设置百叶窗位图的数据，把当前位图的指定切割区域的像素颜色数据
		int iStorgeSize = (int)((rect.bottom - rect.top) / BLEND_COUNT) * 4 * (rect.right - rect.left);
		int iResult = i * iStorgeSize;

		memcpy(
			pBits,
			pBitmapDataSrc + iResult,
			iStorgeSize);
	}

	//释放当前窗口位图的像素颜色数据
	delete[] pBitmapDataSrc;

	//删除当前窗口的位图
	DeleteObject(hBitmapSrc);

	//删除内存
	DeleteDC(hMemDC);

	//暂停200毫秒
	Sleep(200);

	//绘制百叶窗偶数个数部分
	for (int i = 0; i < BLEND_COUNT; i += 2)
	{
		Sleep(80);
		DrawBitmap(
			hdc,
			0,
			i * (int)((rect.bottom - rect.top) / BLEND_COUNT),
			rect.right - rect.left,
			(int)((rect.bottom - rect.top) / BLEND_COUNT),
			hBitmaps[i]);
	}

	//暂停500毫秒
	Sleep(500);

	//绘制百叶窗奇数个数部分
	for (int i = 1; i < BLEND_COUNT; i += 2)
	{
		Sleep(80);
		DrawBitmap(
			hdc,
			0,
			i * (int)((rect.bottom - rect.top) / BLEND_COUNT),
			rect.right - rect.left,
			(int)((rect.bottom - rect.top) / BLEND_COUNT),
			hBitmaps[i]);
	}

	//释放当前窗口DC
	ReleaseDC(hWnd, hdc);

	//删除位图句柄数组里的各个位图
	for (int i = 0; i < BLEND_COUNT; i++)
	{
		DeleteObject(hBitmaps[i]);
	}

	//刷新窗口
	InvalidateRect(hWnd, &rect, FALSE);
	UpdateWindow(hWnd);
}

void DrawBitmap(HDC hdc, int x, int y, int width, int height, HBITMAP hBitmap)
{
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hdc, x, y, width, height, hMemDC, 0, 0, SRCCOPY);
	DeleteDC(hMemDC);
}

BOOL SaveBMP(HBITMAP hbm, const std::wstring& filePath)
{
	LPBYTE pBitmap = NULL;

	BITMAP bm = { 0 };
	assert(GetObject(hbm, sizeof(BITMAP), &bm));
	assert(bm.bmWidth && bm.bmHeight);

	if (!bm.bmBits)
	{
		LONG size = bm.bmWidthBytes * bm.bmHeight;
		pBitmap = new BYTE[size];
		assert(GetBitmapBits(hbm, size, pBitmap) == size);
	}
	else
	{
		pBitmap = (LPBYTE)bm.bmBits;
	}

	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = 0x4d42;
	bfh.bfSize = bm.bmWidthBytes * bm.bmHeight + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biHeight = -bm.bmHeight;
	bmi.bmiHeader.biPlanes = bm.bmPlanes;
	bmi.bmiHeader.biBitCount = bm.bmBitsPixel;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = bm.bmWidthBytes * bm.bmHeight;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;

	FILE *fp = NULL;
	_tfopen_s(&fp, filePath.c_str(), _T("wb"));
	assert(fp);
	size_t writeCount = fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
	assert(writeCount == 1);
	writeCount = fwrite(&bmi, sizeof(BITMAPINFOHEADER), 1, fp);
	assert(writeCount == 1);
	writeCount = fwrite(pBitmap, bmi.bmiHeader.biSizeImage, 1, fp);
	assert(writeCount == 1);
	fclose(fp);

	return true;
}


void SaveBitmap(HWND hWnd, std::wstring fileName)
{
	//DeleteFile(_T("image.bmp"));
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
	SaveBMP(hBitmap, fileName);
	DeleteDC(hdc);
}