﻿#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "Stage.h"
#include <cmath>

// 定数
namespace
{
    // ハンドルのパス
    const char* kCityHandle = "data/model/city/city.mv1";
    const char* kSkydoomHandle = "data/model/city/skydoom.mv1";

    // ステージ
    const VECTOR kStageScale = VGet(10.0f, 10.0f, 10.0f);
    const VECTOR kSkydoomScale = VGet(1.5f, 1.5f, 1.5f);
    const VECTOR kStagePos = VGet(0.0f, 0.0f, 0.0f);
    const VECTOR kSkydoomPos = VGet(7000.0f, 60.0f, 5000.0f);
    constexpr float kSkydoomRotationSpeed = 0.001f;


    // 当たり判定
    constexpr float kDefaultSize = 50.0f;	 // 周囲のポリゴン検出に使用する球の初期サイズ
    constexpr float kHitWidth = 8.0f;	     // 当たり判定カプセルの半径
    constexpr float kHitHeight = 40.0f;	     // 当たり判定カプセルの高さ
    constexpr float kHitBottom = 20.0f;	     // 当たり判定カプセルの位置
    constexpr float kHitBottom2 = -20.0f;	 // 当たり判定カプセルの位置
    constexpr int kHitTryNum = 16;		     // 壁押し出し処理の最大試行回数
    constexpr float kHitSlideLength = 0.5f;	 // 一度の壁押し出し処理でスライドさせる距離
}

Stage::Stage(std::shared_ptr<Player> player) :
    m_skydoomRotate(0.0f),
    m_wallNum(0),
    m_floorNum(0),
    m_floor(),
    m_wall()
{
    m_pPlayer = player;

	m_stageHandle = MV1LoadModel(kCityHandle);
    m_skydoomHandle = MV1LoadModel(kSkydoomHandle);

    MV1SetScale(m_stageHandle, kStageScale);
    MV1SetPosition(m_stageHandle, kStagePos);
    MV1SetScale(m_skydoomHandle, kSkydoomScale);
    MV1SetPosition(m_skydoomHandle, kSkydoomPos);

    // モデル全体のコリジョン情報のセットアップ
    MV1SetupCollInfo(m_stageHandle, -1);
}

Stage::~Stage()
{
    MV1DeleteModel(m_stageHandle);
    MV1DeleteModel(m_skydoomHandle);
}

void Stage::Update()
{
    // スカイドームを回転させる
    m_skydoomRotate += kSkydoomRotationSpeed;
    
    if (m_skydoomRotate > DX_PI_F * 2)
    {
        m_skydoomRotate -= DX_PI_F * 2;
    }

    MV1SetRotationXYZ(m_skydoomHandle, VGet(0.0f, m_skydoomRotate, 0.0f));
}

void Stage::Draw()
{
    MV1DrawModel(m_skydoomHandle);
    MV1DrawModel(m_stageHandle);
}

VECTOR Stage::CheckObjectCol(ObjectBase& obj, const VECTOR& moveVec)
{
    VECTOR oldPos = obj.GetPos();		    // 移動前の座標	
    VECTOR nextPos = VAdd(oldPos, moveVec);	// 移動後の座標

    // オブジェクトの周囲にあるステージポリゴンを取得する
    auto hitDim = MV1CollCheck_Sphere(m_stageHandle, -1, oldPos, kDefaultSize + VSize(moveVec));

    // 検出されたポリゴンが壁ポリゴンか床ポリゴンか判別し、保存する
    // MEMO:壁ポリゴン(XZ平面に垂直なポリゴン)、床ポリゴン(XZ平面に垂直でないポリゴン)
    AnalyzeWallAndFloor(hitDim, oldPos);

    // 壁ポリゴンとの当たり判定をチェックする
    nextPos = CheckHitObjWithWall(obj, nextPos);
    // 床ポリゴンとの当たり判定をチェックする
    nextPos = CheckHitObjWithFloor(obj, nextPos);

    // 検出したプレイヤーの周囲のポリゴン情報の後始末をする
    MV1CollResultPolyDimTerminate(hitDim);

    return nextPos;
}

void Stage::SetDropMoney(VECTOR enemyPos, int dropMoney)
{
    m_pPlayer->AddMoney(dropMoney); // プレイヤーの所持金を増やす
}

void Stage::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition)
{
    // 壁ポリゴンと床ポリゴンの数を初期化する
    m_wallNum = 0;
    m_floorNum = 0;

    // 検出されたポリゴンの数だけ繰り返す
    for (int i = 0; i < hitDim.HitNum; i++)
    {
        // XZ平面に垂直かどうか
        // MEMO:垂直かどうかはポリゴンの法線のY成分が0に限りなく近いかどうかで判断する
        bool isVertical = hitDim.Dim[i].Normal.y < 0.000001f && hitDim.Dim[i].Normal.y > -0.000001f;
        
        // XZ平面に垂直な場合(壁ポリゴンと判断された場合)
        if (isVertical)
        {
            // 壁ポリゴンと判断された場合でも、プレイヤーのY座標+1.0fより高いポリゴンのみ当たり判定を行う
            if (hitDim.Dim[i].Position[0].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[1].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[2].y > checkPosition.y + 1.0f)
            {
                // ポリゴンの数が列挙できる限界数に達していない場合、ポリゴンを配列に追加する
                if (m_wallNum < MaxHitColl)
                {
                    // ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
                    m_wall[m_wallNum] = &hitDim.Dim[i];
                    m_wallNum++;
                }
            }
        }
        // XZ平面に垂直でない場合
        else
        {
            // ポリゴンの数が列挙できる限界数に達していない場合、ポリゴンを配列に追加
            if (m_floorNum < MaxHitColl)
            {
                // ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
                m_floor[m_floorNum] = &hitDim.Dim[i];
                m_floorNum++;
            }
        }
    }
}

VECTOR Stage::CheckHitObjWithWall(ObjectBase& obj, const VECTOR& checkPosition)
{
    // 補正後の位置
    VECTOR fixedPos = checkPosition;

    if (m_wallNum == 0) return fixedPos;

    // 壁からの押し出し処理を行う
    for (int i = 0; i < kHitTryNum; i++)
    {
        // 当たる可能性のある壁ポリゴンを全て見る
        bool isHitWall = false;
        for (int i = 0; i < m_wallNum; i++)
        {
            // i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
            auto poly = m_wall[i];

            // オブジェクトの終点
            VECTOR fixedEndPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));
            // オブジェクトが壁に当たっているか
            bool isHit = HitCheck_Capsule_Triangle(fixedPos, fixedEndPos, kHitWidth, poly->Position[0], poly->Position[1], poly->Position[2]);

            // オブジェクトと当たっている場合
            if (isHit)
            {
                // オブジェクトを壁の法線方向に移動させる
                fixedPos = VAdd(fixedPos, VScale(poly->Normal, kHitSlideLength));

                // 移動した壁ポリゴンと接触しているかどうか判定する
                for (int j = 0; j < m_wallNum; j++)
                {
                    poly = m_wall[j];
                    if (isHit)
                    {
                        isHitWall = true;
                        break;
                    }
                }

                // 全てのポリゴンと当たっていなかったらループ終了
                if (!isHitWall) break;
            }
        }

        // 全ての壁ポリゴンと接触しなくなったらループから抜ける
        if (!isHitWall) break;
    }

    return fixedPos;
}

VECTOR Stage::CheckHitObjWithFloor(ObjectBase& obj, const VECTOR& checkPosition)
{
    VECTOR fixedPos = checkPosition;

    if (m_floorNum == 0) return fixedPos;

    bool isHitFloor = false;
    float maxY = 0.0f;

    for (int i = 0; i < m_floorNum; i++)
    {
        auto poly = m_floor[i]; // i番目の床ポリゴンのアドレス

        HITRESULT_LINE lineResult{};  // 線分とポリゴンとの当たり判定の結果を代入する構造体
        VECTOR topPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));       // オブジェクトの始点先の位置
        VECTOR bottomPos = VAdd(fixedPos, VGet(0.0f, kHitBottom, 0.0f));    // オブジェクトの終点少し低い位置
        VECTOR bottomPos2 = VAdd(fixedPos, VGet(0.0f, kHitBottom2, 0.0f));  // オブジェクトの終点さらに低い位置

        // 始点からそこそこ低い位置の間で当たっているかを判定する
        lineResult = HitCheck_Line_Triangle(topPos, bottomPos2, poly->Position[0], poly->Position[1], poly->Position[2]);

        // 既に当たったポリゴンがあり、検出した床ポリゴンより低い場合何もしない
        if (lineResult.HitFlag)
        {
            if (!(isHitFloor && maxY > lineResult.Position.y))
            {
                // 接触したY座標を保存する
                isHitFloor = true;
                maxY = lineResult.Position.y;
            }
        }
    }

    // 床ポリゴンに当たった場合
    if (isHitFloor)
    {
        fixedPos.y = maxY;
    }

    return fixedPos;
}