// キャラクターに付いていくカメラ
//
// ＜操作＞
// 方向キー：キャラクターモデル移動
// Z,Cキー：カメラの水平角度を変更
// S,Xキー：カメラの垂直角度を変更

#include <memory>

#include "header.hpp"

#include "main.hpp"

#include "avatar.hpp"

constexpr char numOfAvts = 32;

// 移動速度
//#define MOVESPEED			10.0f

// ラインを描く範囲
constexpr float LINE_AREA_SIZE = 10000.0f;

// ラインの数
constexpr int LINE_NUM = 50;

std::string str[3];


IPDATA ip;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	float  cameraHAngle ;
	float  cameraVAngle ;
	
	int fileHandle;

	short charaId;
	char hostId = false;

	std::string name = "Non";
	int modelId = 2;
	static HANDLE th;

	//std::unique_ptr<NetWork> network;

	Character* Avatar[numOfAvts];
	MyCharacter* AvatarMe;


	signed char myAvaterId = -1;

	// ウインドウモードで起動
	ChangeWindowMode( TRUE ) ;
	SetAlwaysRunFlag( TRUE );

	
	
	readFile( ip, name, modelId);
	//host = false;
	SystemBits |= (1<<2);
	th=CreateThread(0,0,(LPTHREAD_START_ROUTINE)netSetUp,(LPVOID)&ip,0,NULL);

	while (SystemBits & (1<<2))
	{
		/* code */
	}
	

	SetMainWindowText((TCHAR*)"ゆっくりしていってね");

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1 ;

	for (char i = 0; i < 32; i++)
	{
		Avatar[i] = nullptr;
	}
	
	// 描画先を裏画面にする
	SetDrawScreen( DX_SCREEN_BACK ) ;


	AvatarMe = new MyCharacter();
    
	AvatarMe->CharactorSet(charaId);
	//Host a;
	//Communication = &a;
	std::cout << "CharactorSetOK\n";
	//network->setBuffer(AvatarMe->GetSetUp());
	std::cout << "BufferセットアップOK";

	// カメラの向きを初期化
	cameraHAngle = 0.0f ;
	cameraVAngle = 40.0f ;

	
	for (int i = 0; i < 32; i++)
	{
		if (Avatar[i] != nullptr)
		{
		
			if (AvatarMe->num != i)
			{
	//			Avatar[i]->setMove(Communication->getBuffer(i));
			}
		}
	}
	
	// カメラのクリッピング距離を設定
	SetCameraNearFar( 100.0f, 50000.0f );

	// 背景の色を設定する
	SetBackgroundColor( 128, 200, 100 ) ;

	// メインループ
	while( (ProcessMessage() == 0) && !(SystemBits & (1<<1)))
	{

		// 画面のクリア
		ClearDrawScreen() ;
		
		AvatarMe->GetMoveKey();

		//network->setBuffer(AvatarMe->GetAvatarStatus());

		AvatarMe->MoveCamera();
		
		AvatarMe->CameraMove();

		for (char i = 0; i <32; i++)
		{
			if(Avatar[i] != nullptr)
				Avatar[i]->DrawModel();
		}
		

	//	Communication->setBuffer( AvatarMe->GetDate());

		// 位置関係が分かるように地面にラインを描画する
		{
			int i ;
			VECTOR Pos1 ;
			VECTOR Pos2 ;

			SetUseZBufferFlag( TRUE ) ;

			Pos1 = VGet( -LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f ) ;
			Pos2 = VGet( -LINE_AREA_SIZE / 2.0f, 0.0f,  LINE_AREA_SIZE / 2.0f ) ;
			for( i = 0 ; i <= LINE_NUM ; i ++ )
			{
				DrawLine3D( Pos1, Pos2, GetColor( 255,255,255 ) ) ;
				Pos1.x += LINE_AREA_SIZE / LINE_NUM ;
				Pos2.x += LINE_AREA_SIZE / LINE_NUM ;
			}

			Pos1 = VGet( -LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f ) ;
			Pos2 = VGet(  LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f ) ;
			for( i = 0 ; i < LINE_NUM ; i ++ )
			{
				DrawLine3D( Pos1, Pos2, GetColor( 255,255,255 ) ) ;
				Pos1.z += LINE_AREA_SIZE / LINE_NUM ;
				Pos2.z += LINE_AREA_SIZE / LINE_NUM ;
			}

			SetUseZBufferFlag( FALSE ) ;
		}

		// 裏画面の内容を表画面に反映

		AvatarMe->DrawName();

		ScreenFlip() ;
	}
	//netSituation &= ~(1 << 0);
	//netSituation &= ~(1 << 1);
	// ＤＸライブラリの後始末

	DxLib_End() ;
	DWORD result;

	SystemBits &= ~(1 << 0);

	while (!(SystemBits & (1 << 1)))
	{
		/* code */
	}
	
	/*
	do
	{
		GetExitCodeThread( th, &result);
	} while (STILL_ACTIVE==result);
	*/

	// ソフトの終了

	std::cout << "システムを終了\n";

	return 0 ;
}