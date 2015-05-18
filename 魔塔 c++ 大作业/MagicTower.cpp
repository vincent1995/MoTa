#include"MagicTower.h"
#include<stdlib.h>
#include<Windows.h>
//主干函数
BOOL GameInitialize(HINSTANCE hInstance)
{
	g_pGE=new GameEngine(hInstance,TEXT("魔塔"),TEXT("魔塔"),
		IDI_BIGICON,IDI_SMALLICON,FALSE,32,900,500);
	if(g_pGE==NULL)
		return false;
	g_pGE->SetFrameDelay(33);
	g_pGE->SetPause(false);
	
	if(!g_pGE->CreateGameWindow())	//创建游戏窗口
	{
		MessageBox(NULL,TEXT("修改显示模式失败!\r\n程序退出!"),TEXT("错误"),MB_OK| MB_ICONERROR);
		return FALSE;		//退出程序
	}
	return true;
}
void GameStart(HWND hwnd)
{
	Scene_Num=0;
	Initial_SceStart();
	LoadResource(GetDC(hwnd));
}
void KeyEvent(HWND hwnd,WPARAM wParam)
{
	switch(Scene_Num)
	{
	case 0://开始场景
		KeyEvent_SceStart(hwnd,wParam);
		break;
	case 1://主场景
		KeyEvent_SceMain(hwnd,wParam);
		break;
	case 2://菜单场景
		KeyEvent_SceMenu(hwnd,wParam);
		break;
	}
}
void CharEvent(HWND hwnd,WPARAM wParam)
{
	switch(Scene_Num)
	{
	case 0://开始场景
		break;
	case 1://主场景
		CharEvent_SceMain(hwnd,wParam);
		break;
	case 2://菜单场景
		CharEvent_SceMenu(hwnd,wParam);
		break;
	}
}
void GameAction(HWND hwnd)
{
	switch(Scene_Num)
	{
	case 0://开始场景
		break;
	case 1://主场景
		GameAction_SceMain(hwnd);
		break;
	}
	InvalidateRect(hwnd,NULL,FALSE);
	UpdateWindow(hwnd);
}
void GamePaint(HDC hdc)
{
	switch(Scene_Num)
	{
	case 0://开始场景
		Paint_SceStart(hdc);
		break;
	case 1://主场景
		Paint_SceMain(hdc);
		break;
	case 2://菜单场景
		Paint_SceMenu(hdc);
		break;
	}
}
void GameEnd()
{
	DeleteResource();
	delete g_pGE;
};
void GameActive(HWND hWnd)
{
	g_pGE->SetPause(FALSE);
};
void GamePause(HWND hWnd)
{
	g_pGE->SetPause(TRUE);
};
BOOL GameWindowClose(HWND hWnd)
{
	if(MessageBox(g_pGE->GetWindow(),TEXT("你确定要退出吗？"),TEXT("退出游戏"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK)==IDYES)
		return TRUE;
	else
		return FALSE;
};
void MouseClick(HWND hWnd,int x,int y,WPARAM wParam){};
//开始场景函数
void KeyEvent_SceStart(HWND hwnd,WPARAM wParam)
{
	switch(wParam)
	{
	case VK_UP:
		if(Arrow_Num>0)
			Arrow_Num--;
		break;
	case VK_DOWN:
		if(Arrow_Num<3)
			Arrow_Num++;
		break;
	case VK_RETURN:
	case VK_SPACE:
		switch(Arrow_Num)
		{
		case 0://开始游戏
			Scene_Num=1;
			Initial_SceMain(GetDC(hwnd));
			break;
		case 1://载入游戏
			Scene_Num=2;
			Menu_Num=1;
			SaveTip=1;
			break;
		case 2://关于游戏
			MessageBox(g_pGE->GetWindow(),TEXT("操作方法：上下左右键控制人物移动。"),TEXT("关于游戏"),MB_YESNO);
			MessageBox(g_pGE->GetWindow(),TEXT("制作人员名单：vincent、小蛮腰、学霸强、JJ。游戏最终解释权归620游戏开发小组所有。"),TEXT("关于游戏"),MB_YESNO);
			break;
		case 3://退出游戏
			SendMessage(hwnd,WM_CLOSE,0,0);
			break;
		}
		break;
	}
};
void Paint_SceStart(HDC hdc)
{
	HDC mdc=CreateCompatibleDC(hdc);//载入背景图片
	HBITMAP hbegin=(HBITMAP)LoadImage(NULL,TEXT("Resource\\cbegin.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	HBITMAP hbeginold=(HBITMAP)SelectObject(mdc,hbegin);

	HDC hdc_tip=CreateCompatibleDC(hdc);//载入箭头图片
	HBITMAP hbm_tip=(HBITMAP)LoadImage(NULL,TEXT("Resource\\ctip.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	HBITMAP old_hbm_tip=(HBITMAP)SelectObject(hdc_tip,hbm_tip);

	BITMAP Bk,tip;
	GetObject(hbegin,sizeof(BITMAP),&Bk);
	GetObject(hbm_tip,sizeof(BITMAP),&tip);

	StretchBlt(hdc,0,0,GameEngine::GetGameEngine()->GetWidth(),
		GameEngine::GetGameEngine()->GetHeight(),mdc,0,0,Bk.bmWidth,Bk.bmHeight,SRCCOPY);
	TransparentBlt(hdc,300,250+Arrow_Num*58,40,40,hdc_tip,0,0,tip.bmWidth,tip.bmHeight,RGB(255,255,255));
	SelectObject(mdc,hbeginold);
	SelectObject(hdc_tip,old_hbm_tip);

	DeleteObject(hbegin);
	DeleteObject(hbm_tip);

	DeleteDC(mdc);
	DeleteDC(hdc_tip);
	
	HFONT hFont,hOldFont;
	hFont=CreateFont(25,17,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	hOldFont=(HFONT)SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(255,0,0));
	SetBkMode(hdc,TRANSPARENT);
	int size;
	TCHAR szText[50];
	size=wsprintf(szText,TEXT("620出品"));
	TextOut(hdc,680,400,szText,size);
	SelectObject(hdc,hOldFont);
	DeleteObject(hFont);	

	hFont=CreateFont(15,7,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	hOldFont=(HFONT)SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(255,0,0));
	SetBkMode(hdc,TRANSPARENT);

	size=wsprintf(szText,TEXT("CopyRight©  All Rights Reserved"));
	TextOut(hdc,630,430,szText,size);
	SelectObject(hdc,hOldFont);
	DeleteObject(hFont);	
};
void Initial_SceStart()
{
	Arrow_Num=0;
	Old_Scene_Num=0;
}
//主场景函数
void Initial_SceMain(HDC hdc)
{
	cur_map=0;
	for(int i=0;i<TOTAL_MAP;i++)
		m[i]=new map(x_mapsize,y_mapsize,i);
	hero=new player();
	x_hero=x_DesPos=7;
	y_hero=y_DesPos=5;
	hero_direction=0;
	hero_step=0;
	IniSceDone=true;
	Poison=0;
	Old_Scene_Num=1;

	Frame_Num=0;//动画帧计时器
	NextFrame=timeGetTime()+300;
}
void LoadResource(HDC hdc)
{
	mdc_object=CreateCompatibleDC(hdc);//加载object.bmp
	hbm_object=(HBITMAP)LoadImage(NULL,TEXT("Resource\\object.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	old_hbm_ojbect=(HBITMAP)SelectObject(mdc_object,hbm_object);
	GetObject(hbm_object,sizeof(BITMAP),&bm_object);

	KeyYDC=CreateCompatibleDC(hdc);//载入铜色钥匙.bmp
	KeyYBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\铜色钥匙.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldKeyYBit=(HBITMAP)SelectObject(KeyYDC,KeyYBit);
	GetObject(KeyYBit,sizeof(BITMAP),&KeyYBm);

	KeyBDC=CreateCompatibleDC(hdc);
	KeyBBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\蓝色钥匙.bmp"),IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE);
	OldKeyBBit=(HBITMAP)SelectObject(KeyBDC,KeyBBit);
	GetObject(KeyBBit,sizeof(BITMAP),&KeyBBm);

	KeyRDC=CreateCompatibleDC(hdc);
	KeyRBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\红色钥匙.bmp"),IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE);
	OldKeyBBit=(HBITMAP)SelectObject(KeyRDC,KeyRBit);
	GetObject(KeyRBit,sizeof(BITMAP),&KeyRBm);

	SwordDC=CreateCompatibleDC(hdc);
	SwordBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\剑.bmp"),IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE);
	OldSwordBit=(HBITMAP)SelectObject(SwordDC,SwordBit);
	GetObject(SwordBit,sizeof(BITMAP),&SwordBm);

	ShieldDC=CreateCompatibleDC(hdc);
	ShieldBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\盾牌.bmp"),IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE);
	OldKeyBBit=(HBITMAP)SelectObject(ShieldDC,ShieldBit);
	GetObject(ShieldBit,sizeof(BITMAP),&ShieldBm);

	MedicineDC=CreateCompatibleDC(hdc);
	MedicineBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\药水.bmp"),IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE);
	OldMedicineBit=(HBITMAP)SelectObject(MedicineDC,MedicineBit);
	GetObject(MedicineBit,sizeof(BITMAP),&MedicineBm);

	MonsterDC1=CreateCompatibleDC(hdc);
	MonsterBit1=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物1.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit1=(HBITMAP)SelectObject(MonsterDC1,MonsterBit1);	
	GetObject(MonsterBit1,sizeof(BITMAP),&MonsterBm1);

	MonsterDC2=CreateCompatibleDC(hdc);
	MonsterBit2=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物2.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit2=(HBITMAP)SelectObject(MonsterDC2,MonsterBit2);	
	GetObject(MonsterBit2,sizeof(BITMAP),&MonsterBm2);

	MonsterDC34=CreateCompatibleDC(hdc);
	MonsterBit34=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物3、4.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit34=(HBITMAP)SelectObject(MonsterDC34,MonsterBit34);	
	GetObject(MonsterBit34,sizeof(BITMAP),&MonsterBm34);

	MonsterDC5=CreateCompatibleDC(hdc);
	MonsterBit5=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物5.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit5=(HBITMAP)SelectObject(MonsterDC5,MonsterBit5);	
	GetObject(MonsterBit5,sizeof(BITMAP),&MonsterBm5);

	MonsterDC6=CreateCompatibleDC(hdc);
	MonsterBit6=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物6.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit6=(HBITMAP)SelectObject(MonsterDC6,MonsterBit6);	
	GetObject(MonsterBit6,sizeof(BITMAP),&MonsterBm6);

	MonsterDC7=CreateCompatibleDC(hdc);
	MonsterBit7=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物7.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit7=(HBITMAP)SelectObject(MonsterDC7,MonsterBit7);	
	GetObject(MonsterBit7,sizeof(BITMAP),&MonsterBm7);

	MonsterDC8=CreateCompatibleDC(hdc);
	MonsterBit8=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物8.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit8=(HBITMAP)SelectObject(MonsterDC8,MonsterBit8);	
	GetObject(MonsterBit8,sizeof(BITMAP),&MonsterBm8);

	MonsterDC9=CreateCompatibleDC(hdc);
	MonsterBit9=(HBITMAP)LoadImage(NULL,TEXT("Resource\\怪物9.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBit9=(HBITMAP)SelectObject(MonsterDC9,MonsterBit9);	
	GetObject(MonsterBit9,sizeof(BITMAP),&MonsterBm9);

	MonsterDCBoss=CreateCompatibleDC(hdc);
	MonsterBitBoss=(HBITMAP)LoadImage(NULL,TEXT("Resource\\boss.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	OldMonsterBitBoss=(HBITMAP)SelectObject(MonsterDCBoss,MonsterBitBoss);	
	GetObject(MonsterBitBoss,sizeof(BITMAP),&MonsterBmBoss);

	mdc_hero=CreateCompatibleDC(hdc);//加载主角图片
	hbm_hero=(HBITMAP)LoadImage(NULL,TEXT("Resource\\hero.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	old_hbm_hero=(HBITMAP)SelectObject(mdc_hero,hbm_hero);
	GetObject(hbm_hero,sizeof(BITMAP),&bm_hero);

	mdc_godness=CreateCompatibleDC(hdc);
	hbm_godness=(HBITMAP)LoadImage(NULL,TEXT("Resource\\仙女室友.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	old_hbm_godness=(HBITMAP)SelectObject(mdc_godness,hbm_godness);
	GetObject(hbm_godness,sizeof(BITMAP),&bm_godness);

	mdc_shop=CreateCompatibleDC(hdc);
	hbm_shop=(HBITMAP)LoadImage(NULL,TEXT("Resource\\shop.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	old_hbm_shop=(HBITMAP)SelectObject(mdc_shop,hbm_shop);
	GetObject(hbm_shop,sizeof(BITMAP),&bm_shop);
}
void DeleteResource()
{
	SelectObject(mdc_object,old_hbm_ojbect);//删除object.bmp
	DeleteObject(hbm_object);
	DeleteDC(mdc_object);

	SelectObject(KeyYDC,OldKeyYBit);//删除黄色钥匙
	DeleteObject(KeyYBit);
	DeleteDC(KeyYDC);

	SelectObject(KeyBDC,OldKeyBBit);//删除蓝色钥匙
	DeleteObject(KeyBBit);
	DeleteDC(KeyBDC);

	SelectObject(KeyRDC,OldKeyRBit);//删除红色钥匙
	DeleteObject(KeyRBit);
	DeleteDC(KeyRDC);

	SelectObject(ShieldDC,OldShieldBit);//删除盾
	DeleteObject(ShieldBit);
	DeleteDC(ShieldDC);

	SelectObject(SwordDC,OldSwordBit);//删除剑
	DeleteObject(SwordBit);
	DeleteDC(SwordDC);

	SelectObject(MedicineDC,OldMedicineBit);//删除药水
	DeleteObject(MedicineBit);
	DeleteDC(MedicineDC);

	SelectObject(MonsterDC1,OldMonsterBit1);
	DeleteObject(MonsterBit1);
	DeleteDC(MonsterDC1);

	SelectObject(MonsterDC2,OldMonsterBit2);
	DeleteObject(MonsterBit2);
	DeleteDC(MonsterDC2);

	SelectObject(MonsterDC34,OldMonsterBit34);
	DeleteObject(MonsterBit34);
	DeleteDC(MonsterDC34);

	SelectObject(MonsterDC5,OldMonsterBit5);
	DeleteObject(MonsterBit5);
	DeleteDC(MonsterDC5);

	SelectObject(MonsterDC6,OldMonsterBit6);
	DeleteObject(MonsterBit6);
	DeleteDC(MonsterDC6);

	SelectObject(MonsterDC7,OldMonsterBit7);
	DeleteObject(MonsterBit7);
	DeleteDC(MonsterDC7);

	SelectObject(MonsterDC8,OldMonsterBit8);
	DeleteObject(MonsterBit8);
	DeleteDC(MonsterDC8);

	SelectObject(MonsterDC9,OldMonsterBit9);
	DeleteObject(MonsterBit9);
	DeleteDC(MonsterDC9);

	SelectObject(MonsterDCBoss,OldMonsterBitBoss);
	DeleteObject(MonsterBitBoss);
	DeleteDC(MonsterDCBoss);

	SelectObject(mdc_hero,old_hbm_hero);
	DeleteObject(hbm_hero);
	DeleteDC(mdc_hero);

	SelectObject(mdc_shop,old_hbm_shop);
	DeleteObject(hbm_shop);
	DeleteDC(mdc_shop);
}

void Paint_SceMain(HDC hdc)
{
	PaintDisplay(hdc);
	SetViewportOrgEx(hdc,x_displaysize,0,NULL);
	PaintMap(hdc);
	SetViewportOrgEx(hdc,0,0,NULL);
}
void PaintMap(HDC hdc)
{
	for(int i=0;i<x_mapsize;i++)//绘制墙体，地面
	{
		for(int j=0;j<y_mapsize;j++)
		{
			switch(m[cur_map]->GetMapPoint(i,j))
			{
			case 0://墙壁
			case 98://可打碎的墙
				StretchBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					1*bm_object.bmWidth/30,
					3*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,SRCCOPY);
				break;
			case 24://岩浆
				StretchBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					2*bm_object.bmWidth/30,
					16*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,SRCCOPY);
				break;
			default: //空地  （在物品以及怪物之下先绘制地面）
				StretchBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					0*bm_object.bmWidth/30,
					0*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,SRCCOPY);
				break;
			}
		}
	}
	///////////////////////////////////////////////////////////
	//绘制物品，怪物
	for(int i=0;i<x_mapsize;i++)
	{
		for(int j=0;j<y_mapsize;j++)
		{
			switch(m[cur_map]->GetMapPoint(i,j))
			{
			case 2://怪物
			case 3:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				if(i!=x_DesPos||j!=y_DesPos)
					DrawMonster(hdc,i*x_blocksize,j*y_blocksize,m[cur_map]->GetMapPoint(i,j));
				 else if(i==x_DesPos&&j==y_DesPos&&monsterloss)
					DrawMonsterLoss(hdc,j*y_blocksize,i*x_blocksize,(NextTime-NowTime)*255/1000,
					m[cur_map]->GetMapPoint(i,j));
				break;
			case 4://向上楼梯
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					18*bm_object.bmWidth/30,
					20*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,RGB(0,0,0));
				break;
			case 5://向下楼梯
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					18*bm_object.bmWidth/30,
					19*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,RGB(0,0,0));
				break;
			case 6://黄色门
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					3*bm_object.bmWidth/30,
					20*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,RGB(0,0,0));
				break;
			case 7://蓝色门
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					2*bm_object.bmWidth/30,
					20*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,RGB(0,0,0));
				break;
			case 8://红色门
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_object,
					1*bm_object.bmWidth/30,
					20*bm_object.bmHeight/30,
					bm_object.bmWidth/30,bm_object.bmHeight/30,RGB(0,0,0));
				break;
			case 9://黄色钥匙
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,KeyYDC,
						0,0,KeyYBm.bmWidth,KeyYBm.bmHeight,RGB(238,243,250));
				break;
			case 10://蓝色钥匙
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,KeyBDC,
						0,0,KeyBBm.bmWidth,KeyBBm.bmHeight,RGB(238,243,250));
				break;
			case 11://红色钥匙
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,KeyRDC,
						0,0,KeyRBm.bmWidth,KeyRBm.bmHeight,RGB(238,243,250));
				break;
			case 12://血瓶
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,MedicineDC,
						0,0,MedicineBm.bmWidth,MedicineBm.bmHeight,RGB(238,243,250));
				break;
			case 13://剑
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,SwordDC,
						0,0,SwordBm.bmWidth,SwordBm.bmHeight,RGB(238,243,250));
				break;
			case 14://盾牌
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,ShieldDC,
						0,0,ShieldBm.bmWidth,ShieldBm.bmHeight,RGB(238,243,250));
				break;
			case 15://商店				
				TransparentBlt(hdc,(j-1)*y_blocksize,i*x_blocksize,3*x_blocksize,y_blocksize,mdc_shop,
					0,
					0,
					3*bm_shop.bmWidth/4,bm_shop.bmHeight/4,RGB(255,255,255));
				break;
			case 97://仙女
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_godness,
					Frame_Num*bm_godness.bmWidth/4,
					3*bm_godness.bmHeight/4,
					bm_godness.bmWidth/4,bm_godness.bmHeight/4,RGB(255,255,255));
				break;
			case 100://室友  胜利
				TransparentBlt(hdc,j*y_blocksize,i*x_blocksize,x_blocksize,y_blocksize,mdc_godness,
					Frame_Num*bm_godness.bmWidth/4,
					2*bm_godness.bmHeight/4,
					bm_godness.bmWidth/4,bm_godness.bmHeight/4,RGB(255,255,255));
				break;
			}
		}
	}

	//绘制物品怪物结束
	//////////////////////////////

	//////////////////////////////
	//绘制主角
	TransparentBlt(hdc,y_hero*y_blocksize,x_hero*x_blocksize,x_blocksize,y_blocksize,mdc_hero,
		hero_step*bm_hero.bmWidth/4,
		hero_direction*bm_hero.bmHeight/4,
		bm_hero.bmWidth/4,bm_hero.bmHeight/4,RGB(255,255,255));


	//绘制主角结束
	//////////////////////////////////////////////////////////
	return ;


}
void PaintDisplay(HDC hdc)
{	

	HDC mdc;
	mdc=CreateCompatibleDC(hdc);
	HBITMAP hBit,hOldBit;
	hBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\frame.bmp"),IMAGE_BITMAP,400,400,LR_LOADFROMFILE);
	hOldBit=(HBITMAP)SelectObject(mdc,hBit);
	BitBlt(hdc,0,0,400,400,mdc,0,0,SRCCOPY);
	SelectObject(mdc,hOldBit);
	DeleteObject(hBit);
	DeleteDC(mdc);


	HFONT hFont,hOldFont;
	hFont=CreateFont(25,17,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	hOldFont=(HFONT)SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(255,0,0));
	SetBkMode(hdc,TRANSPARENT);
	int size;
	TCHAR szText[20];
	size=wsprintf(szText,TEXT("勇士属性"));
	TextOut(hdc,180,40,szText,size);
	SelectObject(hdc,hOldFont);
	DeleteObject(hFont);
	HBRUSH hBrush,OldBrush;
	hBrush=CreateSolidBrush(RGB(0,0,0));
	OldBrush=(HBRUSH)SelectObject(hdc,hBrush);

	hFont=CreateFont(18,15,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	hOldFont=(HFONT)SelectObject(hdc,hFont);

	size = wsprintf(szText,TEXT("%d"),1);
	TextOut(hdc, 215, 87, szText, size);
	size=wsprintf(szText,TEXT("%d"),hero->GetHp());
	TextOut(hdc,215,109,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetAttack());
	TextOut(hdc,215,129,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetDefence());
	TextOut(hdc,215,149,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetMoney());
	TextOut(hdc,215,169,szText,size);
	size=wsprintf(szText,TEXT("%d"),0);
	TextOut(hdc,215,189,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetKeyNum(0));
	TextOut(hdc,145,230,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetKeyNum(1));
	TextOut(hdc,145,270,szText,size);
	size=wsprintf(szText,TEXT("%d"),hero->GetKeyNum(2));
	TextOut(hdc,145,310,szText,size);
	if(cur_map+1<10)
	{
		size=wsprintf(szText,TEXT("%d"),cur_map+1);
	TextOut(hdc,180,348,szText,size);
	}
	else
	{
	size=wsprintf(szText,TEXT("%d"),cur_map+1);
	TextOut(hdc,175,348,szText,size);
	}
	size=wsprintf(szText,TEXT("按S保存"));
	TextOut(hdc,0,410,szText,size);
	size=wsprintf(szText,TEXT("按L载入进度"));
	TextOut(hdc,200,410,szText,size);
	size=wsprintf(szText,TEXT("按G进入怪物手册"));
	TextOut(hdc,0,440,szText,size);
	SelectObject(hdc,hOldFont);
	DeleteObject(hFont);
	SelectObject(hdc,OldBrush);
	DeleteObject(hBrush);
}

void KeyEvent_SceMain(HWND hwnd,WPARAM wParam)
{
	if(x_DesPos!=x_hero||y_DesPos!=y_hero)return;
	switch(wParam)
	{
	case VK_UP:
		if(x_DesPos-1>=0)
			x_DesPos--;
		hero_direction=3;
		hero_step=(hero_step+1)%4;
		break;
	case VK_DOWN:
		if(x_DesPos+1<x_mapsize)
			x_DesPos++;
		hero_direction=0;
		hero_step=(hero_step+1)%4;
		break;
	case VK_LEFT:
		if(y_DesPos-1>=0)
			y_DesPos--;
		hero_direction=1;
		hero_step=(hero_step+1)%4;
		break;
	case VK_RIGHT:
		if(y_DesPos+1<y_mapsize)
			y_DesPos++;
		hero_direction=2;
		hero_step=(hero_step+1)%4;
		break;
	}
}
void CharEvent_SceMain(HWND hwnd,WPARAM wParam)
{
	switch(wParam)//进入菜单场景
	{
	case 's'://保存
	case 'S':
		Scene_Num=2;
		Menu_Num=0;
		SaveTip=1;
		break;
	case 'l':
	case 'L'://读取
		Scene_Num=2;
		Menu_Num=1;
		SaveTip=1;
		break;
	case 'g'://怪物手册
	case 'G':
		Scene_Num=2;
		Menu_Num=3;
		break;

	}
}
void GameAction_SceMain(HWND hwnd)
{
	if(NextFrame<timeGetTime())//设置帧
	{
		Frame_Num=(Frame_Num+1)%4;
		NextFrame=timeGetTime()+300;
	}

	if(x_DesPos==x_hero&&y_DesPos==y_hero)//判定移动方向目标
		return ;
	int obj=m[cur_map]->GetMapPoint(x_DesPos,y_DesPos);
	switch(obj)
	{
	case 0://墙壁
	case 96://不可进入的空地
	case 24:
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 1://空地
		x_hero=x_DesPos;
		y_hero=y_DesPos;
		if(Poison)
		{
			hero->SetHp(-20);
			Poison--;
		}
		break;
	case 98://可以打碎的墙壁
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 97://仙女
		MessageBox(g_pGE->GetWindow(),TEXT("仙女：勇士，你来危险的魔塔做什么？"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("勇士：我是来救人的。"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("仙女：你是来救你心爱的女孩的么？"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("勇士：不是，我是来救我的室友的！"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("仙女：呃。。。同学，你是华工的吧？"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("勇士：你怎么知道？！"),TEXT("对话"),MB_YESNO);
		MessageBox(g_pGE->GetWindow(),TEXT("仙女：呃，没什么。我衷心祝愿你能成功。左右两边有武器，带上它们去救你的室友吧！"),TEXT("对话"),MB_YESNO);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 22://怪物9   中毒
		if(AttackMonster(8))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
			MessageBox(g_pGE->GetWindow(),TEXT("你中毒了，小心！五步之内你都会受到毒性伤害"),TEXT("游戏提示"),MB_YESNO);
			Poison=5;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 23://怪物10
		if(AttackMonster(9))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 4://上升楼梯
		x_hero=x_DesPos;
		y_hero=y_DesPos;
		cur_map++;
		break;
	case 5://下降楼梯
		x_hero=x_DesPos;
		y_hero=y_DesPos;
		cur_map--;
		break;
	case 6://黄色门
		if(hero->SetKeyNum(0,-1))
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 7://蓝色门
		if(hero->SetKeyNum(1,-1))
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 8://红色门
		if(hero->SetKeyNum(2,-1))
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 9://黄钥匙
		hero->SetKeyNum(0,1);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 10://蓝钥匙
		hero->SetKeyNum(1,1);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 11://红钥匙
		hero->SetKeyNum(2,1);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 12://血瓶
		hero->SetHp(200);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 13://剑
		hero->SetAttack(20);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 14://盾
		hero->SetDefence(5);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 101://作弊
		MessageBox(g_pGE->GetWindow(),TEXT("勇士！620与你同在，你将受到祝福！"),
			TEXT("作弊装置"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
		hero->SetAttack(620);
		hero->SetDefence(620);
		hero->SetHp(6200);
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 100://胜利
		MessageBox(g_pGE->GetWindow(),TEXT("你到达了魔塔的最高层，找到了你的室友。故事的最后，你和你的室友回到华工重新过上了幸福的生活！"),TEXT("胜利！！"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
		MessageBox(g_pGE->GetWindow(),TEXT("你通关了！！谢谢游戏！！620出品！！版权所有  侵权必究"),TEXT("胜利！！"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
		Scene_Num=0;
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 99://幻影
		if(MessageBox(g_pGE->GetWindow(),TEXT("你正走在路上，突然面前出现了一个幻影，幻影自称是作者，他自称他有法术改变你的能力，你愿意让幻影在你身上施展法术么？"),
			TEXT("空地"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK)==IDYES)
		{
			srand( (unsigned)time( NULL ) );
			if(rand()%2)
			{
				MessageBox(g_pGE->GetWindow(),TEXT("你感受到了一种力量传到你的体内，你的攻击力翻倍了！幻影悄悄飘走了"),
				TEXT("幻影"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
				hero->SetAttack(hero->GetAttack());
			}
			else
			{
				MessageBox(g_pGE->GetWindow(),TEXT("你的身体突然间失去了力量，你晕了过去，醒来过后，你的生命值减少了"),
				TEXT("幻影"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
				hero->SetHp(-hero->GetHp()/2);
			}
		m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
		}
		else
		{
				MessageBox(g_pGE->GetWindow(),TEXT("幻影留在原地，静静地看着你"),
				TEXT("幻影"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
		}
		x_hero=x_DesPos;
		y_hero=y_DesPos;
		break;
	case 15://商店
		if(MessageBox(g_pGE->GetWindow(),TEXT("客官你好呀!看来你是着了吧，要不要来一碗620特制的火锅/n能增加你500点生命，只要200金币哦！！"),
			TEXT("商店"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK)==IDYES)
		{
			if(hero->SetMoney(-200))
			{
				MessageBox(g_pGE->GetWindow(),TEXT("客官慢走，欢迎下次再来啊！"),
					TEXT("商店"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
				hero->SetHp(500);
			}
			else
			{
				MessageBox(g_pGE->GetWindow(),TEXT("钱都没有还想吃火锅！"),
					TEXT("商店"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
			}
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("这么美味的火锅你却吃不到真的好可惜啊"),
				TEXT("商店"),MB_YESNO|MB_DEFBUTTON2|MB_ICONASTERISK);
		}
		x_DesPos=x_hero;
		y_DesPos=y_hero;
		break;
	case 2://怪物1
		if(AttackMonster(0))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 3://怪物2
		if(AttackMonster(1))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 16://怪物3
		if(AttackMonster(2))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 17://怪物4
		if(AttackMonster(3))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 18://怪物5
		if(AttackMonster(4))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 19://怪物6
		if(AttackMonster(5))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 20://怪物7
		if(AttackMonster(6))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	case 21://怪物8
		if(AttackMonster(7))
		{
			NowTime=timeGetTime();
			NextTime=NowTime;
			while(NextTime-NowTime<1000)
			{
				monsterloss=true;
				InvalidateRect(hwnd,NULL,FALSE);
				UpdateWindow(hwnd);
				NextTime=timeGetTime();
			}
			monsterloss=false;
			m[cur_map]->SetMapPoint(x_DesPos,y_DesPos,1);
			x_DesPos=x_hero;
			y_DesPos=y_hero;
		}
		else
		{
			MessageBox(g_pGE->GetWindow(),TEXT("你挂了！！哈哈哈"),TEXT("游戏结束"),MB_YESNO);
			GameStart(hwnd);
		}
		break;
	}
}
bool AttackMonster(int Mon_Num)
{
	int mon_hp=Monster_HP[Mon_Num];
	int hero_damage=hero->GetAttack()-Monster_Defence[Mon_Num];
	if(hero_damage<=0)hero_damage=1;

	int mon_damage=Monster_Attack[Mon_Num]-hero->GetDefence();
	if(mon_damage<=0) mon_damage=1;

	while(true)
	{
		mon_hp-=hero_damage;
		if(mon_hp<=0)
		{
			hero->SetMoney(Monster_Money[Mon_Num]);
			return true;
		}
		if(!hero->SetHp(-mon_damage))
			return false;
	}
}
bool AttackMonster5()
{
	int mon_hp=Monster_HP[4];
	int hero_damage=hero->GetAttack()-Monster_Defence[4];
	if(hero_damage<=0)hero_damage=1;

	int mon_damage=Monster_Attack[4];
	if(mon_damage<=0) mon_damage=1;

	while(true)
	{
		mon_hp-=hero_damage;
		if(mon_hp<=0)
		{
			hero->SetMoney(Monster_Money[4]);
			return true;
		}
		if(!hero->SetHp(-mon_damage))
			return false;
	}
}
//菜单场景函数
void Paint_SceMenu(HDC hdc)
{
	switch(Menu_Num)
	{
	case 0://保存
		for(int i=1;i<=4;i++)
		{
			int j=judge(i);
		DrawSave(hdc,i,j);
		}
		break;
	case 1://读取
		for(int i=1;i<=4;i++)
		{
		int j=judge(i);
		DrawSave(hdc,i,j);
		}
		break;
	case 3://怪物手册
		DrawMonsterInformation(hdc);
		break;
	}

}
void KeyEvent_SceMenu(HWND hwnd,WPARAM wParam)
{
	switch(Menu_Num)
	{
	case 0://保存
		switch(wParam)
		{
		case VK_UP:
			if(SaveTip!=1)
				SaveTip--;
			break;
		case VK_DOWN:
			if(SaveTip!=4)
				SaveTip++;
			break;
		case VK_SPACE:
			save(SaveTip);
			Scene_Num=1;
			break;
		case VK_ESCAPE:
			Scene_Num=1;
			break;
		}
		break;
	case 1://读取
		switch(wParam)
		{
		case VK_ESCAPE:
			Scene_Num=Old_Scene_Num;
			break;
		case VK_UP:
			if(SaveTip!=1)
				SaveTip--;
			break;
		case VK_DOWN:
			if(SaveTip!=4)
				SaveTip++;
			break;
		case VK_SPACE:
			if(judge(SaveTip))
			{
				Initial_SceMain(GetDC(hwnd));
				read(SaveTip);
				Scene_Num=1;
			}
			else
			{
				MessageBox(NULL,TEXT("此处存档为空"),TEXT("错误"),MB_OK| MB_ICONERROR);
			}
			break;

		}
		break;
	case 3://怪物手册
		switch(wParam)
		{
		case VK_ESCAPE:
			Scene_Num=1;
			break;
		}
		break;
	}
}
void CharEvent_SceMenu(HWND hwnd,WPARAM wParam)
{
	switch(wParam)//菜单场景中再次按下对应按键回到主场景
	{
	case 's'://保存
	case 'S':
		if(Menu_Num==0)
		Scene_Num=Old_Scene_Num;
		break;
	case 'l':
	case 'L'://读取
		if(Menu_Num==1)
		Scene_Num=Old_Scene_Num;
		break;
	case 'g'://怪物手册
	case 'G':
		if(Menu_Num==3)
		Scene_Num=Old_Scene_Num;
		break;
	}
}
void GameAction_SceMenu(HWND hwnd){}
//怪物消失的函数
void DrawMonsterLoss(HDC hdc,int x,int y,int i,int ID)
{
	
	HDC BlankDC=CreateCompatibleDC(hdc);
	HBITMAP BlankBit=CreateCompatibleBitmap(hdc,x_blocksize,y_blocksize);
	HBITMAP OldBlankBit=(HBITMAP)SelectObject(BlankDC,BlankBit);

	
	StretchBlt(BlankDC,0,0,x_blocksize,y_blocksize,mdc_object,0,0,bm_object.bmWidth/30,
		bm_object.bmHeight/30,SRCCOPY);
	if(ID==2)
	{
		
		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC1,0,0,
			MonsterBm1.bmWidth/4,MonsterBm1.bmHeight/4,RGB(255,255,255));
	}
	else if(ID==3)
	{
		
		
		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC2,0,3*MonsterBm2.bmHeight/4,
			MonsterBm2.bmWidth/4,MonsterBm2.bmHeight/4,RGB(255,255,255));
	}
	else if(ID==16||ID==17)
	{
		

		if(ID==16)
			TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC34,0,0,
			MonsterBm34.bmWidth/4,MonsterBm34.bmHeight/4,RGB(255,255,255));
		if(ID==17)
			TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC34,0,MonsterBm34.bmHeight/4,
			MonsterBm34.bmWidth/4,MonsterBm34.bmHeight/4,RGB(255,255,255));
	}
	else 	if(ID==18)
	{
		
		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC5,0,0,
			MonsterBm5.bmWidth/4,MonsterBm5.bmHeight/4,RGB(255,255,255));
	}
	else 	if(ID==19)
	{
		

		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC6,0,0,
			MonsterBm6.bmWidth/4,MonsterBm6.bmHeight/4,RGB(255,255,255));
	}
	else 	if(ID==20)
	{
		
		
		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC7,0,2*MonsterBm7.bmHeight/4,
			MonsterBm7.bmWidth/4,MonsterBm7.bmHeight/4,RGB(255,255,255));
	}
	else 	if(ID==21)
	{
		

		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC8,0,0,
			MonsterBm8.bmWidth/4,MonsterBm8.bmHeight/4,RGB(255,255,255));
	}
	else 	if(ID==22)
	{
		
		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDC9,0,2*MonsterBm9.bmHeight/4,
			MonsterBm9.bmWidth/4,MonsterBm9.bmHeight/4,RGB(255,255,255));
	}
	else
	{

		TransparentBlt(BlankDC,0,0,x_blocksize,y_blocksize,MonsterDCBoss,0,0,
			MonsterBmBoss.bmWidth/4,MonsterBmBoss.bmHeight/4,RGB(255,255,255));
	}
	BLENDFUNCTION bf;
	bf.BlendOp=AC_SRC_OVER;
	bf.BlendFlags=0;
	bf.SourceConstantAlpha=255-i;
	bf.AlphaFormat=0x00;

	AlphaBlend(hdc,x,y,x_blocksize,y_blocksize,BlankDC,0,0,x_blocksize,y_blocksize,bf);

	SelectObject(BlankDC,OldBlankBit);

	DeleteObject(BlankBit);
	DeleteObject(OldBlankBit);
	DeleteDC(BlankDC);
	long nTick=timeGetTime();			//获取当前游戏时间
				//获取下次循环时间
				long nNextTick=GameEngine::GetGameEngine()->GetNextGameTick();

				if(nTick>nNextTick)				//如果游戏时间到达下次循环时间
				{
					//计算下次循环的时间
					long nNextGameTick = 
							nTick+GameEngine::GetGameEngine()->GetFrameDelay();
					if(NextFrame<timeGetTime())//设置帧
					{

						Frame_Num=(Frame_Num+1)%2;
						NextFrame=timeGetTime()+300;
					}
				}

}
void DrawMonster(HDC hdc,int x,int y,int Mon_Num)
{
	switch(Mon_Num)
	{
	case 2:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC1,
		Frame_Num*MonsterBm1.bmWidth/4,0,MonsterBm1.bmWidth/4,MonsterBm1.bmHeight/4,RGB(255,255,255));
		break;
	case 3:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC2,
		Frame_Num*MonsterBm2.bmWidth/4,MonsterBm2.bmHeight/4*3,MonsterBm2.bmWidth/4,MonsterBm2.bmHeight/4,RGB(255,255,255));
		break;
	case 16:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC34,
		Frame_Num*MonsterBm34.bmWidth/4,0,MonsterBm34.bmWidth/4,MonsterBm34.bmHeight/4,RGB(255,255,255));	
		break;
	case 17:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC34,
		Frame_Num*MonsterBm34.bmWidth/4,MonsterBm34.bmHeight/4,MonsterBm34.bmWidth/4,MonsterBm34.bmHeight/4,RGB(255,255,255));	
		break;
	case 18:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC5,
		Frame_Num*MonsterBm5.bmWidth/4,0,MonsterBm5.bmWidth/4,MonsterBm5.bmHeight/4,RGB(255,255,255));	
		break;
	case 19:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC6,
		Frame_Num*MonsterBm6.bmWidth/4,0,MonsterBm6.bmWidth/4,MonsterBm6.bmHeight/4,RGB(255,255,255));	
		break;
	case 20:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC7,
		Frame_Num*MonsterBm7.bmWidth/4,MonsterBm7.bmHeight/4*2,MonsterBm7.bmWidth/4,MonsterBm7.bmHeight/4,RGB(255,255,255));	
		break;
	case 21:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC8,
		Frame_Num*MonsterBm8.bmWidth/4,0,MonsterBm8.bmWidth/4,MonsterBm8.bmHeight/4,RGB(255,255,255));	
		break;
	case 22:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDC9,
		Frame_Num*MonsterBm9.bmWidth/4,MonsterBm9.bmHeight/4*2,MonsterBm9.bmWidth/4,MonsterBm9.bmHeight/4,RGB(255,255,255));	
		break;
	case 23:
		TransparentBlt(hdc,y,x,x_blocksize,y_blocksize,MonsterDCBoss,
		Frame_Num*MonsterBmBoss.bmWidth/4,0,MonsterBmBoss.bmWidth/4,MonsterBmBoss.bmHeight/4,RGB(255,255,255));	
		break;
	}
}

//怪物手册

void DrawMonsterInformation(HDC hdc)
{
	HDC MonsterInformationDC=CreateCompatibleDC(hdc);
	HBITMAP MonsterInformationBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//背景.bmp"),IMAGE_BITMAP,GameEngine::GetGameEngine()->GetWidth(),
		GameEngine::GetGameEngine()->GetHeight(),LR_LOADFROMFILE);
	HBITMAP OldMonsterInformationBit=(HBITMAP)SelectObject(MonsterInformationDC,MonsterInformationBit);

	HFONT hFont,hOldFont;
	hFont=CreateFont(50,30,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	hOldFont=(HFONT)SelectObject(MonsterInformationDC,hFont);
	SetTextColor(MonsterInformationDC,RGB(255,0,0));
	SetBkMode(MonsterInformationDC,TRANSPARENT);
HFONT hFont1=CreateFont(20,15,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	int size;
	TCHAR szText[50];
	size=wsprintf(szText,TEXT("怪物手册"));
	TextOut(MonsterInformationDC,GameEngine::GetGameEngine()->GetWidth()/2-100,
		0,szText,size);
	HFONT OldhFont1=(HFONT)SelectObject(MonsterInformationDC,hFont1);
	bool monster[10]={false};
    HDC MonsterDC=CreateCompatibleDC(hdc);
	HBITMAP MonsterBit=NULL,OldMonsterBit=NULL;
	BITMAP MonsterBm;
	int Monster_Num=0;
	for(int i=0;i<x_mapsize;i++)
	{
			for(int j=0;j<y_mapsize;j++)
			{
				if(m[cur_map]->GetMapPoint(i,j)==2&&!monster[0])
					monster[0]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==3&&!monster[1])
					monster[1]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==16&&!monster[2])
					monster[2]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==17&&!monster[3])
					monster[3]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==18&&!monster[4])
					monster[4]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==19&&!monster[5])
					monster[5]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==20&&!monster[6])
					monster[6]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==21&&!monster[7])
					monster[7]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==22&&!monster[8])
					monster[8]=true;
				else if(m[cur_map]->GetMapPoint(i,j)==23&&!monster[9])
					monster[9]=true;
			}
	}
	if(monster[0])
				{					
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物1.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));										
					
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("史莱姆   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[0],Monster_Attack[0],Monster_Defence[0]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);	
					Monster_Num++;
				}
	if(monster[1])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物2.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,3*MonsterBm.bmHeight/4,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("快乐鸡   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[1],Monster_Attack[1],Monster_Defence[1]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
	if(monster[2])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物3、4.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("小蝙蝠   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[2],Monster_Attack[2],Monster_Defence[2]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);	
					Monster_Num++;
				}
				 if(monster[3])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物3、4.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,MonsterBm.bmHeight/4,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("蝙蝠侠   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[3],Monster_Attack[3],Monster_Defence[3]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
					 if(monster[4])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物5.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("黑暗法师   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[4],Monster_Attack[4],Monster_Defence[4]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
				 if(monster[5])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物6.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("半兽人   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[5],Monster_Attack[5],Monster_Defence[5]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
				 if(monster[6])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物7.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,2*MonsterBm.bmHeight/4,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("熔岩巨兽   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[6],Monster_Attack[6],Monster_Defence[6]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
				 if(monster[7])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物8.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("幽冥骑士   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[7],Monster_Attack[7],Monster_Defence[7]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
					if(monster[8])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//怪物9.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,2*MonsterBm.bmHeight/4,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("瘟疫战士   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[8],Monster_Attack[8],Monster_Defence[8]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);	
					Monster_Num++;
				}
				 if(monster[9])
				{
					MonsterBit=(HBITMAP)LoadImage(NULL,TEXT("Resource//boss.bmp"),
						IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					OldMonsterBit=(HBITMAP)SelectObject(MonsterDC,MonsterBit);
					GetObject(MonsterBit,sizeof(BITMAP),&MonsterBm);
					TransparentBlt(MonsterInformationDC,120,70+50*Monster_Num,x_blocksize,y_blocksize,
						MonsterDC,0,0,MonsterBm.bmWidth/4,MonsterBm.bmHeight/4,RGB(255,255,255));
							
					SetTextColor(MonsterInformationDC,RGB(0,0,255));
					size=wsprintf(szText,TEXT("暗夜天尊   HP:%d   攻击力:%d   防御力:%d"),
						Monster_HP[9],Monster_Attack[9],Monster_Defence[9]);
					TextOut(MonsterInformationDC,190,85+50*Monster_Num,szText,size);
					Monster_Num++;
				}
	BitBlt(hdc,0,0,GameEngine::GetGameEngine()->GetWidth(),
		GameEngine::GetGameEngine()->GetHeight(),MonsterInformationDC,0,0,SRCCOPY);
	

	SelectObject(MonsterInformationDC,OldhFont1);
	SelectObject(MonsterInformationDC,OldMonsterInformationBit);
	SelectObject(MonsterInformationDC,hOldFont);
	SelectObject(MonsterDC,OldMonsterBit);

	DeleteObject(hFont1);
	DeleteObject(MonsterBit);
	DeleteDC(MonsterDC);
	DeleteObject(MonsterInformationBit);
	DeleteObject(hFont);
	DeleteDC(MonsterInformationDC);
	

}
//存档读档函数
void DrawSave(HDC hdc,int i,int j)
{
	HFONT hFont=CreateFont(30,10,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,0);
	HFONT OldhFont=(HFONT)SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(0,255,0));
	TCHAR szText[10];
	int size=0;
	HDC TipDC=CreateCompatibleDC(hdc);
	HBITMAP TipBit=(HBITMAP)LoadImage(NULL,TEXT("Resource\\ctip.bmp"),IMAGE_BITMAP,60,30,LR_LOADFROMFILE);
	HBITMAP OldTipBit=(HBITMAP)SelectObject(TipDC,TipBit);
	if(SaveTip==1)
		TransparentBlt(hdc,GameEngine::GetGameEngine()->GetWidth()/2-120,
			GameEngine::GetGameEngine()->GetHeight()/4-100,
			60,30,TipDC,0,0,60,30,RGB(255,255,255));
	else if(SaveTip==2)
		TransparentBlt(hdc,GameEngine::GetGameEngine()->GetWidth()/2-120,
			2*GameEngine::GetGameEngine()->GetHeight()/4-100,
			60,30,TipDC,0,0,60,30,RGB(255,255,255));
	else if(SaveTip==3)
		TransparentBlt(hdc,GameEngine::GetGameEngine()->GetWidth()/2-120,
			3*GameEngine::GetGameEngine()->GetHeight()/4-100,
			60,30,TipDC,0,0,60,30,RGB(255,255,255));
	else if(SaveTip==4)
		TransparentBlt(hdc,GameEngine::GetGameEngine()->GetWidth()/2-120,
			GameEngine::GetGameEngine()->GetHeight()-100,
			60,30,TipDC,0,0,60,30,RGB(255,255,255));
	if(i==1)
	{
		if(j==0)
		{
		size=wsprintf(szText,TEXT("存档一（空）"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
		else
		{
		size=wsprintf(szText,TEXT("存档一"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
	}
	else if(i==2)
	{
		if(j==0)
		{
		size=wsprintf(szText,TEXT("存档二（空）"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			2*GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
		else
		{
		size=wsprintf(szText,TEXT("存档二"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			2*GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
	}
	else if(i==3)
	{
		if(j==0)
		{
		size=wsprintf(szText,TEXT("存档三（空）"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			3*GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
		else
		{
		size=wsprintf(szText,TEXT("存档三"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			3*GameEngine::GetGameEngine()->GetHeight()/4-100,szText,size);
		}
	}
	else if(i==4)
	{
		if(j==0)
		{
		size=wsprintf(szText,TEXT("存档四（空）"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			GameEngine::GetGameEngine()->GetHeight()-100,szText,size);
		}
		else
		{
		size=wsprintf(szText,TEXT("存档四"));
		TextOut(hdc,GameEngine::GetGameEngine()->GetWidth()/2-60,
			GameEngine::GetGameEngine()->GetHeight()-100,szText,size);
		}
	}
	
	SelectObject(TipDC,OldTipBit);
	DeleteObject(TipBit);
	DeleteDC(TipDC);
	
	SelectObject(hdc,OldhFont);
	DeleteObject(hFont);
}
int judge(int i)                                                      //判断第i个存档是否为空（空返回0，非空返回1）
{
	fstream f;
	int j=0;
    switch(i)
	{
	case 1:
		{
			f.open("file1.dat",ios::in|ios::binary);
		}break;
		case 2:
		{
			f.open("file2.dat",ios::in|ios::binary);
		}break;
		case 3:
		{
			f.open("file3.dat",ios::in|ios::binary);
		}break;
		case 4:
		{
			f.open("file4.dat",ios::in|ios::binary);
		}break;
	}
	f.seekg(0,ios::beg);
	f.read((char*)&j,sizeof(int));
	return j;
	f.close();
}
void save(int i)                                                         //存储于第i个存档
{
	fstream f;
	switch(i)
	{
	case 1:f.open("file1.dat",ios::out|ios::binary);break;
	case 2:f.open("file2.dat",ios::out|ios::binary);break;
	case 3:f.open("file3.dat",ios::out|ios::binary);break;
	case 4:f.open("file4.dat",ios::out|ios::binary);
	}
	f.seekp(0,ios::beg);
	int j=1;
	f.write((char*)&j,sizeof(int));
	f.write((char*)hero,sizeof(player));
		for(int k=0;k<TOTAL_MAP;k++)
		{
		for(int i=0;i<x_mapsize;i++)
			for(int j=0;j<y_mapsize;j++)
			{
				int p=m[k]->GetMapPoint(i,j);
				f.write((char*)&p,sizeof(int));
			}
		}
	f.write((char*)&cur_map,sizeof(int));
	f.write((char*)&x_hero,sizeof(int));
	f.write((char*)&y_hero,sizeof(int));
	f.close();
}
void read(int i)                                                        //读取第i个存档
{
	fstream f;
	switch(i)
	{
	case 1:f.open("file1.dat",ios::in|ios::binary);break;
	case 2:f.open("file2.dat",ios::in|ios::binary);break;
	case 3:f.open("file3.dat",ios::in|ios::binary);break;
	case 4:f.open("file4.dat",ios::in|ios::binary);
	}
	f.seekg(0,ios::beg);
	f.seekg(sizeof(int));

	f.read((char*)hero,sizeof(player));
	for(int k=0;k<TOTAL_MAP;k++)
	{
		for(int i=0;i<x_mapsize;i++)
			for(int j=0;j<y_mapsize;j++)
			{
					int p;
				f.read((char*)&p,sizeof(int));
				m[k]->SetMapPoint(i,j,p);
			}
	}
	f.read((char*)&cur_map,sizeof(int));
	f.read((char*)&x_DesPos,sizeof(int));
	f.read((char*)&y_DesPos,sizeof(int));
	f.close();
}

