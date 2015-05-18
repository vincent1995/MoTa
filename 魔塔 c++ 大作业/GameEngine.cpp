/*************************************
	游戏引擎源文件 GameEngine.cpp
*************************************/

#include "GameEngine.h"	

/* 导入游戏引擎中需要用到的头文件和相关库文件 */
#include "winuser.h"
#include "mmsystem.h"
#include "wingdi.h"

#pragma   comment(lib,"user32.lib")
#pragma   comment(lib,"winmm.lib")
#pragma   comment(lib,"gdi32.lib")


/* 初始化游戏引擎静态指针 */
GameEngine * GameEngine::m_pGameEngine=NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 游戏引擎构造函数 */
GameEngine::GameEngine(HINSTANCE hInstance,LPTSTR szWindowClass,LPTSTR szTitle,WORD wIcon,WORD wSmallIcon,BOOL bFullScreen,int nColorbit,int nWidth,int nHeight)
{
	m_pGameEngine=this;			//将当前实例赋给静态指针m_pGameEngine
	m_hInstance=hInstance;			//设置引擎实例为当前程序实例句柄

	lstrcpy(m_szWindowClass,szWindowClass);	//将窗口类注册名称赋给_szWindowClass
	lstrcpy(m_szTitle,szTitle);					//将标题赋给m_szTitle

	/* 设置屏幕模式和大小 */
	m_bFullScreen=bFullScreen;			
	m_nColorbit=nColorbit;
	m_nWidth=nWidth;
	m_nHeight=nHeight;

	/*设置图标和光标*/
	m_wIcon=wIcon;
	m_wSmallIcon=wSmallIcon;
	
	/* 记录游戏开始时间和下次循环时间 */
	m_lStartTime=timeGetTime();
	m_lNextGameTick=m_lStartTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 游戏引擎析构函数 */
GameEngine::~GameEngine() {   }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 创建游戏窗口 */
BOOL	GameEngine::CreateGameWindow()
{
	WNDCLASSEX wcApp;				//声明窗口类
	HWND hWnd=NULL;					//声明程序主窗口句柄

	int nPosX,nPosY,nWndWidth,nWndHeight;	//声明窗口位置和大小变量
	DWORD dwWinStyle;					//声明窗口风格变量
	wcApp.cbSize=sizeof(wcApp);

	//给窗口属性赋值
	wcApp.lpszClassName=m_szWindowClass;	//设置窗口类名
	wcApp.style=CS_HREDRAW;				//定义窗口风格
	wcApp.lpfnWndProc=WndProc;				//指定消息处理函数
	wcApp.hInstance=m_hInstance;				//指定义窗口应用程序的句柄
	wcApp.cbWndExtra=0;		
	wcApp.cbClsExtra=0;
	//加载程序图标（大）
	wcApp.hIconSm=LoadIcon(m_hInstance, MAKEINTRESOURCE(m_wIcon));		
	//加载程序图标（小）
	wcApp.hIcon=LoadIcon(m_hInstance, MAKEINTRESOURCE(m_wSmallIcon));	
	wcApp.hCursor=LoadCursor(NULL,IDC_ARROW);			//加载鼠标样式
	wcApp.hbrBackground=(HBRUSH)(COLOR_WINDOW);	//设置窗口背景色
	wcApp.lpszMenuName=NULL;							//设置窗口没有菜单

	RegisterClassEx(&wcApp);		//注册窗口类

	/*使用DEVMODE结构设置屏幕显示模式*/
	DEVMODE DevMode;
	ZeroMemory(&DevMode, sizeof(DevMode));	//将结构DevMode的内存清零
	DevMode.dmSize=sizeof(DevMode);		//设置DevMode存储空间以存储屏幕属性数据
	
	//用当前屏幕显示模式设置填充DevMode
	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&DevMode);

	BOOL bDisplayChange=FALSE;			//标识屏幕显示模式是否改变

	//如果屏幕显示模式位数与游戏不同
	if(	DevMode.dmBitsPerPel!=(unsigned long)m_nColorbit)	
	{
		DevMode.dmBitsPerPel=m_nColorbit;	//设置显示模式位数
		bDisplayChange=TRUE;
	}

	//如果全屏状态下的屏幕尺寸与窗口不同；或屏幕尺寸小于窗口,都需要重设显示模式
	if((m_bFullScreen && m_nWidth!=GetSystemMetrics(SM_CXSCREEN))	
		|| m_nWidth>GetSystemMetrics(SM_CXSCREEN))			 
	{
		DevMode.dmPelsWidth=m_nWidth;		//屏幕宽度
		DevMode.dmPelsHeight=m_nHeight;		//屏幕高度	
		bDisplayChange=TRUE;		
	}
	
	//如果显示模式发生改变
	if(bDisplayChange)		
	{
		LONG result=ChangeDisplaySettings(&DevMode,0);	//动态修改屏幕显示模式	
		//判断修改显示模式是否成功
		if(result==DISP_CHANGE_SUCCESSFUL)	
		{	
			// 如果动态修改显示模式成功，则修改显示模式
			ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
		}
		else
		{
			//如果动态修改显示模式失败，恢复原来的屏幕属性
			ChangeDisplaySettings(NULL,0);		//恢复原来的屏幕属性
			return FALSE;						//返回FALSE
		}
	}

	/* 判断窗口显示方式，并设置相应的窗口属性 */
	if(m_bFullScreen)
	{
		/* 全屏状态下 */
		//设置窗口左上角位置
		nPosX=0;
		nPosY=0;
		dwWinStyle=WS_POPUP;			//设置游戏窗口风格为无边框的弹出式窗口
		nWndWidth=m_nWidth;			//设置窗口大小为游戏画面大小
		nWndHeight=m_nHeight;	
	}
	else
	{	
		/*非全屏状态下，窗口显示在屏幕中心*/
		//计算加上边框后的窗口大小
		nWndWidth=m_nWidth+GetSystemMetrics(SM_CXFIXEDFRAME)*2;
		nWndHeight=m_nHeight+GetSystemMetrics(SM_CYFIXEDFRAME)*10;

		//计算在窗口居中时，窗口左上角的位置
		nPosX=(GetSystemMetrics(SM_CXSCREEN)-nWndWidth)/2;		
		nPosY=(GetSystemMetrics(SM_CYSCREEN)-nWndHeight)/2;	
		
		//设置游戏窗口风格为带标题栏和系统菜单的窗口
		dwWinStyle=WS_SYSMENU|WS_CAPTION;		
	}

	//调用函数CreateWindow建立窗口
	hWnd=CreateWindow(m_szWindowClass,		//窗口类注册名
						m_szTitle,			//设置窗口标题
						dwWinStyle,			//设置窗口风格
						nPosX,				//设置窗口左上角X坐标		
						nPosY,				//设置窗口左上角Y坐标	
						nWndWidth,			//设置窗口宽度
						nWndHeight,			//设置窗口高度
						NULL,				//父窗口句柄
						NULL,				//菜单的句柄
						m_hInstance,		//程序实例句柄
						NULL);		

	if(!hWnd)								//如果窗口建立失败则返回FALSE
		return FALSE;
	ShowWindow(hWnd,SW_SHOWNORMAL);	//显示窗口
	UpdateWindow(hWnd);						//更新窗口

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 程序入口函数定义 */
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,LPSTR lpCmdline,int nCmdShow)
{
	MSG msg;					//定义消息结构
	
	/* 初始化游戏 */
	if(!GameInitialize(hInstance))			
		return false;

	/* 消息循环 */
	while(true)
	{
		Sleep(50);
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))	//接收消息
		{
			if(msg.message==WM_QUIT)		//如果是退出消息，则退出循环
				break;

			TranslateMessage(&msg);			//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//处理消息
		}
		else
		{
			if (!GameEngine::GetGameEngine()->GetPause())	//判断游戏是否暂停
			{
				/* 游戏循环 */
				long nTick=timeGetTime();			//获取当前游戏时间
				//获取下次循环时间
				long nNextTick=GameEngine::GetGameEngine()->GetNextGameTick();

				if(nTick>nNextTick)				//如果游戏时间到达下次循环时间
				{
					//计算下次循环的时间
					long nNextGameTick = 
							nTick+GameEngine::GetGameEngine()->GetFrameDelay();
					GameEngine::GetGameEngine()->SetNextGameTick(nNextGameTick);
					//执行游戏逻辑函数

					GameAction(GameEngine::GetGameEngine()->GetWindow());
				}
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 消息处理函数定义 */
LRESULT CALLBACK WndProc(HWND hWnd,		//窗口句柄
						 UINT message,		 	//消息标识
						 WPARAM wParam,		//消息参数
						 LPARAM lParam)		//消息参数
{	
	/*用开关语句判断消息并进行相应处理*/
	switch(message)		
	{
	case WM_CREATE:					//窗口建立消息
		GameEngine::GetGameEngine()->SetWindow(hWnd);//设置窗口句柄
		GameStart(hWnd);				//游戏开始
		break;

	case WM_PAINT:					//窗口绘制消息



		HDC hDC;					//定义绘图用的设备环境
		HDC hMemDC;					//内存缓冲设备环境
		HBITMAP hbmMem,hbmOld;		//内存缓冲设备环境中的位图
		PAINTSTRUCT ps;				//绘图结构ps
		hDC=BeginPaint(hWnd,&ps);	//开始绘图，并获取绘图环境hDC

		hMemDC=CreateCompatibleDC(hDC);	//创建内存兼容设备环境
		//创建内存兼容位图
		hbmMem=CreateCompatibleBitmap(hDC,GameEngine::GetGameEngine()->GetWidth(),GameEngine::GetGameEngine()->GetHeight());
		hbmOld=(HBITMAP)SelectObject(hMemDC,hbmMem);
		GamePaint(hMemDC);				//调用GamePaint()函数绘图到内存设备环境
		//将内存设备环境中的内容绘制到物理设备环境
		BitBlt(hDC,0,0,GameEngine::GetGameEngine()->GetWidth(),GameEngine::GetGameEngine()->GetHeight(),hMemDC,0,0,SRCCOPY);
		//释放资源
		SelectObject(hMemDC,hbmOld);
		DeleteObject(hbmMem);
		DeleteDC(hMemDC);
		EndPaint(hWnd,&ps);			//结束绘图

		break;

	case WM_LBUTTONDOWN:			//鼠标左键按下消息
			//调用鼠标单击消息处理函数
			MouseClick(hWnd,LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	case WM_KEYDOWN:
			KeyEvent(hWnd,wParam);
			break;
	case WM_CHAR:
			CharEvent(hWnd,wParam);
			break;
	case WM_SETFOCUS:				//游戏窗口得到焦点消息
		GameActive(hWnd);			//游戏继续
		GameEngine::GetGameEngine()->SetPause(FALSE);
		break;

	case WM_KILLFOCUS:			//游戏窗口失去焦点消息
		GamePause(hWnd);			//游戏暂停
		GameEngine::GetGameEngine()->SetPause(TRUE);
		break;

	case WM_CLOSE:					//窗口关闭消息
		if(GameWindowClose(hWnd))	//窗口关闭前的处理
			DestroyWindow(hWnd);	//发出销毁窗口消息
		break;

	case WM_DESTROY:				//程序销毁消息
		GameEnd();					//游戏结束，释放资源
		PostQuitMessage(0);			//通知WINDOWS，程序已发出终止请求
		break;

	default:
		//没有其他消息时，Windows将调用此缺省函数进行处理
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return false;
}
