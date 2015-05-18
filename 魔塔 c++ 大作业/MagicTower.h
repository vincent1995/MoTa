#include"GameEngine.h"
#include"map player.h"
#include"resource.h"
#include<time.h>
#include<stdlib.h>
#pragma comment(lib,"Msimg32.lib")
/////////////////////////////////////////////////////////////////////////////////////////
//��������
/////////////////////////////////////////////////////////////////////////////////////////////

//��ʼ�����йغ���
void Paint_SceStart(HDC hdc);//��ʼ��������
void KeyEvent_SceStart(HWND hwnd,WPARAM wParam);//��ʼ���������¼�
void Initial_SceStart();//��ʼ����ʼ����

//��Ϸ�������йغ���
void Paint_SceMain(HDC hdc);//��Ϸ����������
void PaintMap(HDC hdc);
void PaintDisplay(HDC hdc);
void KeyEvent_SceMain(HWND hwnd,WPARAM wParam);//�����������¼�
void CharEvent_SceMain(HWND hwnd,WPARAM wParam);
void Initial_SceMain(HDC hdc);//��ʼ��������
void GameAction_SceMain(HWND hwnd);
bool AttackMonster(int Monster_Num);
bool AttackMonster5();
void LoadResource(HDC hdc);//����ͼƬ
void DeleteResource();//ɾ��ͼƬ
//�˵���������
void Paint_SceMenu(HDC hdc);
void KeyEvent_SceMenu(HWND hwnd,WPARAM wParam);
void CharEvent_SceMenu(HWND hwnd,WPARAM wParam);
void GameAction_SceMenu(HWND hwnd);
void UseBagThing(int thing_num);
void DrawBagThing(HDC hdc);
void DrawMonsterLoss(HDC hdc,int x,int y,int i,int ID);
void DrawMonster(HDC hdc,int x,int y,int i);
void DrawMonsterInformation(HDC hdc);
void DrawSave(HDC hdc,int i,int j);
void save(int i);
void read(int i);
int judge(int i);
void DrawYellowKey(HDC hdc,int x,int y);
void DrawBlueKey(HDC hdc,int x,int y);
void DrawRedKey(HDC hdc,int x,int y);
void DrawSword(HDC hdc,int x,int y);
void DrawShield(HDC hdc,int x,int y);
void DrawMedicine(HDC hdc,int x,int y);

//////////////////////////////////////////////////////////////////////////////////////////////
//��������ָ��
GameEngine * g_pGE;

//���峣��
const int x_mapsize=10;
const int y_mapsize=10;
const int floor_num=1;

const int x_blocksize=50;
const int y_blocksize=50;

const int x_displaysize=400;
const int y_displaysize=500;
//                               1	2	3	4	5	6	7	8	9	10  ������
int const Monster_HP[10]=		{20,40,	80,	50,	200,100,150,200,500,1000};	
int const Monster_Defence[10]=	{0,	1,	1,	2,	3,	5,	8,	13,	21,	34};	
int const Monster_Attack[10]=	{6,	8,	15,	30,	50,	60,	80,	100,200,300};
int const Monster_Money[10]=	{1,	3,	5,	10,	10,	20,	30,	50,	100,200};
int const x_Arrow=500;
int const y_Arrow[3]={300,400,500};
///////////////////////////////////
//����ȫ�ֱ���

//�������й�
long NowTime;
long NextTime;
bool monsterloss=false;
char* g_strTitle="ħ��";
player* hero;
map*  m[TOTAL_MAP];
int  cur_map;
int Scene_Num;
int Old_Scene_Num;
bool IniSceDone;
int Frame_Num;
long NextFrame;
int Poison;

int x_DesPos,y_DesPos;
int x_hero,y_hero;
int hero_direction;
int hero_step;
//��ʼ�����й�
int Arrow_Num;
//�˵������й�
int SaveTip;
int Save_Num;
int  Menu_Num;

////////////////////////////////////
//��Դ���
HDC mdc_object;//object.bmp
HBITMAP hbm_object;
HBITMAP old_hbm_ojbect;
BITMAP bm_object;

HDC KeyYDC;//��Կ��
HBITMAP KeyYBit;
HBITMAP OldKeyYBit;
BITMAP KeyYBm;

HDC KeyBDC;//��Կ��
HBITMAP KeyBBit;
HBITMAP OldKeyBBit;
BITMAP KeyBBm;

HDC KeyRDC;//��Կ��
HBITMAP KeyRBit;
HBITMAP OldKeyRBit;
BITMAP KeyRBm;

HDC ShieldDC;//����
HBITMAP ShieldBit;
HBITMAP OldShieldBit;
BITMAP ShieldBm;

HDC SwordDC;//��
HBITMAP SwordBit;
HBITMAP OldSwordBit;
BITMAP SwordBm;

HDC MedicineDC;//ҩˮ
HBITMAP MedicineBit;
HBITMAP OldMedicineBit;
BITMAP MedicineBm;


HDC MonsterDC1;
HBITMAP MonsterBit1,OldMonsterBit1;
BITMAP MonsterBm1;

HDC MonsterDC2;
HBITMAP MonsterBit2,OldMonsterBit2;
BITMAP MonsterBm2;

HDC MonsterDC34;
HBITMAP MonsterBit34,OldMonsterBit34;
BITMAP MonsterBm34;

HDC MonsterDC5;
HBITMAP MonsterBit5,OldMonsterBit5;
BITMAP MonsterBm5;

HDC MonsterDC6;
HBITMAP MonsterBit6,OldMonsterBit6;
BITMAP MonsterBm6;

HDC MonsterDC7;
HBITMAP MonsterBit7,OldMonsterBit7;
BITMAP MonsterBm7;

HDC MonsterDC8;
HBITMAP MonsterBit8,OldMonsterBit8;
BITMAP MonsterBm8;

HDC MonsterDC9;
HBITMAP MonsterBit9,OldMonsterBit9;
BITMAP MonsterBm9;

HDC MonsterDCBoss;
HBITMAP MonsterBitBoss,OldMonsterBitBoss;
BITMAP MonsterBmBoss;

HDC mdc_hero;
HBITMAP hbm_hero;
HBITMAP old_hbm_hero;
BITMAP bm_hero;

HDC mdc_godness;
HBITMAP hbm_godness;
HBITMAP old_hbm_godness;
BITMAP  bm_godness;

HDC mdc_shop;
HBITMAP hbm_shop;
HBITMAP old_hbm_shop;
BITMAP  bm_shop;
////////////////////////////////

