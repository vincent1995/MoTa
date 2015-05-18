/*************************************
	游戏引擎头文件 GameEngine.h
*************************************/

#ifndef GAMEENGINE					//条件编译，目的是防止文件被重复编译

#define GAMEENGINE  1				//定义条件编译的宏

#include "windows.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 定义游戏引擎类GameEngine */
class GameEngine
{
/* 声明不公开的成员变量 */
protected:
	static GameEngine * m_pGameEngine;	//静态成员，用于记录指向引擎自身的指针                                                                 
	HINSTANCE	m_hInstance;				//当前程序实例句柄
	HWND		m_hWnd;				//当前程序实例窗口句柄
	TCHAR		m_szWindowClass[50];		//窗口类名称
	TCHAR		m_szTitle[50];				//窗口标题
	WORD		m_wIcon;				//程序图标（大）
	WORD		m_wSmallIcon;			//程序图标（小）
	int			m_nWidth,m_nHeight;		//游戏窗口宽度和高度

	BOOL		m_bFullScreen;			//是否全屏显示
	int			m_nColorbit;				//色彩模式（32位、24位或16位）

	int			m_nFrameDelay;			//游戏帧延时，即两次循环的时间间隔
	long			m_lNextGameTick;		//下一次游戏循环执行时间
	long			m_lStartTime;				//记录游戏开始时间
	BOOL		m_bPause;				//游戏是否处理于暂停状态

/* 声明成员方法 */
public:
	//声明构造函数
	GameEngine(HINSTANCE hInstance,				//设置程序句柄
				LPTSTR szWindowClass,			//设置窗口名称
				LPTSTR szTitle,					//设置窗口标题
				WORD wIcon,					//设置图标（大）
				WORD wSmallIcon,				//设置图标（小）
				BOOL bFullScreen=FALSE,			//设置全屏，默认为FALSE
				int nColorbit=32,					//设置色彩模式，默认为32位色
				int nWidth=800,					//设置窗口宽度，默认为800像素
				int nHeight=600);					//设置窗口高度，默认为600像素

	//声明虚析构函数，便于在游戏中进行扩展
	virtual ~GameEngine();

	//声明创建游戏窗口的函数
	BOOL	CreateGameWindow();
	
	/* 声明成员访问方法 */

	//获取游戏引擎指针
	static	GameEngine * GetGameEngine() 
	{
		return m_pGameEngine;
	};
	
	//获取游戏实例句柄
	HINSTANCE	GetInstance() 
	{
		return m_hInstance;
	};

	//获取、设置游戏窗口句柄
	HWND	GetWindow()	
	{
		return m_hWnd;
	};

	void	SetWindow(HWND hWnd)
	{
		m_hWnd=hWnd;
	};

	//获取、设置游戏暂停状态
	BOOL	GetPause()
	{
		return m_bPause;
	};
	void	SetPause(BOOL bPause)
	{
		m_bPause=bPause;
	};

	//获取、设置游戏帧延时时间
	int		GetFrameDelay()
	{
		return m_nFrameDelay;
	};

	void	SetFrameDelay(int nFrameDelay)
	{
		m_nFrameDelay=nFrameDelay;
	};

	//获取游戏窗口的高度和宽度
	int		GetWidth()
	{
		return m_nWidth;
	};

	int		GetHeight()
	{
		return m_nHeight;
	};

	//获取游戏开始时间
	long GetStartTime()
	{
		return m_lStartTime;
	}

	//设置、获取下一次游戏循环时间
	void SetNextGameTick(int nNextGameTick)
	{
		m_lNextGameTick=nNextGameTick;
	}

	int GetNextGameTick()
	{
		return m_lNextGameTick;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Windows函数声明 */

//Windows程序入口函数声明
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,LPSTR lpCmdline,int nCmdShow);

//Windows程序消息处理函数声明
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,	WPARAM wParam,LPARAM lParam);

/* 游戏引擎函数接口声明，下面这些函数是各个游戏所特有的，必须在游戏开发时设计 */

//游戏初始化
BOOL GameInitialize(HINSTANCE hInstance);

//游戏开始
void GameStart(HWND hWnd);

//游戏结束
void GameEnd();

//游戏逻辑行为
void GameAction(HWND hWnd);

//处理鼠标单击消息
void MouseClick(HWND hWnd,int x,int y,WPARAM wParam);

//处理键盘消息
void KeyEvent(HWND hwnd,WPARAM wParam);
void CharEvent(HWND hwnd,WPARAM wParam);

//游戏画面绘制
void GamePaint(HDC hDC);

//游戏激活
void GameActive(HWND hWnd);

//游戏暂停
void GamePause(HWND hWnd);

//关闭游戏窗口
BOOL GameWindowClose(HWND hWnd);

#endif
