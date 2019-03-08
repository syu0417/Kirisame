///-----------------------------------------------
//キャラクタークラスの処琁E
//-----------------------------------------------
#include "Character.h"
#include "config.h"
#include "Asset.h"
#include "main.h"
#include "TexLoad.h"

#define HOSEITI 15.0f//キャラクター描画位置調整用
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;

Character::Character(void) {
	//初期位置をセチE��
	Coord.X = PLAYER_STARTPOS_X;
	Coord.Y = PLAYER_STARTPOS_Y;

	//初期位置の配�EをセチE��
	GroundInfo.X = (PLAYER_STARTPOS_X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (PLAYER_STARTPOS_Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////初期位置の頂点セチE��
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;
	//チE��スチャセチE��
	Texture = TexOp->PlayerTex;
	KeyWait = 0;
	Inputflg = false;

	UpdateInput();
	JoypadDI_X = GetGamePadLeftStickX();
	JoypadDI_Y = GetGamePadLeftStickY();

}

Character::~Character(void) {
}


void Character::SetCoord(COORD inCoord) {
	memcpy(&Coord, &inCoord, sizeof(COORD));

	//初期位置の配�EをセチE��
	GroundInfo.X = (inCoord.X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (inCoord.Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////初期位置の頂点セチE��
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;

	//サメの向きを�E期状態に
	Tu = 0;
	Tv = 0;

}

void Character::Draw(void) {
	//キャラを描画
	Draw2dPolygon(Coord.X - CHARA_SIZE / 2, Coord.Y - CHARA_SIZE + HOSEITI, CHARA_SIZE, CHARA_SIZE, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, Tu, Tv, 0.5f, 0.5f);
}

void Character::Move(void)
{
	//仮想キー
	bool UP_TRIG = false;
	bool DOWN_TRIG = false;
	bool LEFT_TRIG = false;
	bool RIGHT_TRIG = false;

	memcpy(&wark_coord, &Coord, sizeof(COORD));//キャラの座標�E退避用
	memcpy(&wark_groundinfo, &GroundInfo, sizeof(COORD));//キャラの配�E座標�E退避用

														 //対応するキーが押されたら(キーボ�EチE
	if (GetKeyboardPress(DIK_UP) && Inputflg == false) {
		UP_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.5f;
	}
	else if (GetKeyboardPress(DIK_DOWN) && Inputflg == false) {
		DOWN_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.5f;
	}
	if (GetKeyboardPress(DIK_LEFT) && Inputflg == false) {
		LEFT_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.0f;
	}
	else if (GetKeyboardPress(DIK_RIGHT) && Inputflg == false) {
		RIGHT_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.0f;
	}
	if (Inputflg == true)
	{
		KeyWait++;
	}
	if (KeyWait >= KEYWAIT)
	{
		KeyWait = 0;
		Inputflg = false;
	}

	//対応する�Eタンが押されたら(ゲームパッチE
	if (g_pDIDevGamePad) {
		if (GetGamePadLeftStickY() <= JoypadDI_Y - GAMEPAD_DEADZONE&&Inputflg == false) {
			UP_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.5f;
		}
		else if (GetGamePadLeftStickY() >= JoypadDI_Y + GAMEPAD_DEADZONE&&Inputflg == false) {
			DOWN_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.5f;
		}
		if (GetGamePadLeftStickX() <= JoypadDI_X - GAMEPAD_DEADZONE&&Inputflg == false) {
			LEFT_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.0f;
		}
		else if (GetGamePadLeftStickX() >= JoypadDI_X + GAMEPAD_DEADZONE&&Inputflg == false) {
			RIGHT_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.0f;
		}
	}

	if (UP_TRIG)//1マス上に移勁E
	{
		if (Coord.X % MASUWIDTH == 0)//頂点にぁE��時�Eみ移動可能に
		{
			Coord.Y -= MASUHEIGHT / 2;
			GroundInfo.Y--;//配�EのY座標をマイナス
			Pos_Vertex.Y -= (short)0.5;
		}

	}
	else if (DOWN_TRIG)//�E��Eス下に移勁E
	{
		if (Coord.X % MASUWIDTH == 0)//頂点にぁE��時�Eみ移動可能に
		{
			Coord.Y += MASUHEIGHT / 2;
			GroundInfo.Y++;//配�EのY座標をプラス
			Pos_Vertex.Y += (short)0.5;
		}

	}
	if (LEFT_TRIG)//1マス左に移勁E
	{
		if (Coord.Y % MASUHEIGHT == 0)//頂点にぁE��時�Eみ移動可能に
		{
			Coord.X -= MASUWIDTH / 2;
			GroundInfo.X--;//配�EのX座標をマイナス
			Pos_Vertex.X -= (short)0.5;
		}

	}
	else if (RIGHT_TRIG)//1マス右に移勁E
	{
		if (Coord.Y % MASUHEIGHT == 0)//頂点にぁE��時�Eみ移動可能に
		{
			Coord.X += MASUWIDTH / 2;
			GroundInfo.X++;//配�EのX座標をプラス
			Pos_Vertex.X += (short)0.5;
		}

	}

	///////////////////////////////////////
	// マスの外に出なぁE��ぁE��する処琁E
	///////////////////////////////////////
	if (Coord.X <= STAGEPOS_YOKO)//左にポロリしためE
	{
		Coord.X = STAGEPOS_YOKO;
		GroundInfo.X = 0;
		Pos_Vertex.X = 0;
	}
	if (Coord.Y <= STAGEPOS_TATE)//上にポロリしためE
	{
		Coord.Y = STAGEPOS_TATE;
		GroundInfo.Y = 0;
		Pos_Vertex.Y = 0;
	}
	if (Coord.X >= STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM))//右にポロリしためE
	{
		Coord.X = STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM);
		GroundInfo.X = STAGESIZE_IGOX - 1;
		Pos_Vertex.X = VERTEXX_NUM;
	}
	if (Coord.Y >= STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM))//下にポロリしためE
	{
		Coord.Y = STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM);
		GroundInfo.Y = STAGESIZE_IGOY - 1;
		Pos_Vertex.Y = VERTEXY_NUM;
	}


}

void Character::CheckMove(bool tate, bool yoko)
{

	if (Stagetype != Vertex) {
		switch (Stagetype) {
		case Side_Tate:
			if (tate == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;

		case Side_Yoko:
			if (yoko == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;
		}
	}
}

bool Character::CheckDive(bool tate, bool yoko, bool vertex) {
	switch (Stagetype) {
	case Side_Tate:
		if (tate == 0) {
			return false;
		}
		break;

	case Side_Yoko:
		if (yoko == 0) {
			return false;
		}
		break;

	case Vertex:
		if (vertex == 0) {
			return false;
		}
		break;
	}
	return true;
}
void Character::Update(bool Dive_State)
{
	//更新

	Move();

	if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 0)//頂点にぁE��場吁E
	{
		Stagetype = Vertex;
	}
	else if (GroundInfo.X % 2 == 1 && GroundInfo.Y % 2 == 0) {//横辺
		Stagetype = Side_Yoko;
	}
	else if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 1) {//縦辺
		Stagetype = Side_Tate;
	}

}
StageTypeT Character::OutStageType(void) {
	//StageTypeを返す
	return Stagetype;
}
void Character::Hit(void) {
	//攻撁E��らった時の処琁E
	int a = 0;///仮
}
COORD Character::OutCoord(void) {
	//スチE�Eジ冁E�E位置を取り�EぁE
	return GroundInfo;
}
COORD Character::OutPos(void) {
	//座標を取り出ぁE
	return Coord;
}