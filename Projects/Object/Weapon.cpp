﻿#include "DxLib.h"
#include "DebugDraw.h"
#include "LoadCsv.h"
#include "EffectManager.h"
#include "Stage.h"
#include "Player.h"
#include "Weapon.h"

// 定数
namespace
{
	const char* kTextHandleFileName = "data/ui/text/hirou.png";	// 画像ハンドルのパス名
	const std::string kWeaponFileName = "data/model/weapon/";	// モデルのファイルパス名
	constexpr int kPlayerHandFrameNum = 51;	// 武器をアタッチするフレーム番号
	constexpr float kGroundHeight = 42.0f;  // 地面の高さ
	const float kDispTextAdjY = 25.0f;		// 拾うのテキスト調整位置
	const float kDispEffectRange = 60.0f;	// エフェクトを表示する範囲
}

Weapon::Weapon(std::shared_ptr<Player> pPlayer) :
	m_loadLocationData(-1),
	m_isResetPos(false),
	m_isHitAttack(false)
{
	m_pPlayer = pPlayer;
	m_handle = LoadGraph(kTextHandleFileName);

	LoadCsv::GetInstance().LoadWeaponData(m_weaponData); // 武器データの読み込み
	LoadLocationData(); // 配置データの読み込み
}

Weapon::~Weapon()
{
	DeleteGraph(m_handle);
	for (auto& pair : m_objHandle)
	{
		MV1DeleteModel(pair.second);
	}
	m_objHandle.clear();
}

void Weapon::Init()
{
	// サイズや位置の調整
	for (auto& loc : m_locationData)
	{
		loc.durability = m_weaponData[loc.id].durability;
		loc.updateCol.colRadius = m_weaponData[loc.id].colRadius;
		MV1SetPosition(m_objHandle[loc.id], loc.pos);
		MV1SetScale(m_objHandle[loc.id], loc.scale);
	}
}

void Weapon::Update(Stage& stage)
{
	MATRIX frameMatrix = {}; // 武器の回転行列

	// 武器位置更新
	for (auto& loc : m_locationData)
	{
		// チュートリアル中
		bool isTuto = !m_pPlayer->GetTutoInfo().isEndTutorial && m_pPlayer->GetTutoInfo().currentNum <= Player::TutorialNum::kTuto_2;
		// プレイヤーが近くにいるかどうか
		bool isNearPlayer = VSize(VSub(m_pPlayer->GetPos(), loc.pos)) <= kDispEffectRange;

		if (isTuto || !isNearPlayer)
		{
			// エフェクト、武器を表示しない
			EffectManager::GetInstance().StopWeaponEffect(loc.id);
			MV1SetFrameVisible(m_objHandle[loc.id], 0, false);
		}

		// 武器位置をリセットする
		ResetWeapon(frameMatrix, loc);

		// バトル中でない場合
		if (!m_pPlayer->GetIsBattle())
		{
			InvisibleWeapon(loc); // 武器を非表示にする
		}
		// バトル中の場合
		else
		{
			// 耐久力が0になった場合
			if (loc.durability <= 0)
			{
				InvisibleWeapon(loc); // 武器を非表示にする

				// エフェクトを停止
				EffectManager::GetInstance().StopWeaponEffect(loc.id);
			}
			else
			{
				// モデルを表示する
				MV1SetFrameVisible(m_objHandle[loc.id], 0, true);
			}

			// プレイヤーが武器を掴んだ場合、プレイヤーの手の位置に武器を移動させる
			if (loc.isGrab)
			{
				SetModelFramePos(loc, frameMatrix);

				// エフェクトを停止
				EffectManager::GetInstance().StopWeaponEffect(loc.id);
			}
			else
			{
				// エフェクトを再生
				if (!EffectManager::GetInstance().GetIsPlayingWeaponEffect(loc.id) && isNearPlayer)
				{
					EffectManager::GetInstance().AddWeaponEffect(loc.id, loc.pos);
				}

				loc.rot = VGet(0.0f, 0.0f, 0.0f); // 回転を初期化
				frameMatrix = MGetIdent();		  // 単位行列を設定
				MV1SetMatrix(m_objHandle[loc.id], frameMatrix);
			}
		}

		loc.pos = VAdd(loc.pos, VGet(0.0f, m_gravity, 0.0f)); // 重力を足す
		loc.pos = VAdd(VGet(loc.pos.x, 0.0f, loc.pos.z), stage.CheckObjectCol(*this, VGet(0.0f, 0.0f, 0.0f))); // ステージと当たり判定を行う
		loc.pos.y = std::max(kGroundHeight, loc.pos.y);
		
		UpdateCol(loc); // 当たり判定位置更新
		MV1SetPosition(m_objHandle[loc.id], loc.pos);
		MV1SetRotationXYZ(m_objHandle[loc.id], loc.rot);
		MV1SetScale(m_objHandle[loc.id], loc.scale);
	}
}

void Weapon::Draw()
{
	for (auto& loc : m_locationData)
	{
		MV1DrawModel(m_objHandle[loc.id]); // モデル表示
	}

	// バトル中でない場合は表示しない
	if(!m_pPlayer->GetIsBattle()) return;

#ifdef _DEBUG
	DebugDraw debug;
	int dispY = 440; // 武器情報デバッグ表示位置
	for (const auto& loc : m_locationData)
	{
		// 武器情報描画
		//debug.DrawWeaponInfo(loc.id.c_str(), loc.tag.c_str(), loc.pos, loc.rot, loc.scale, loc.durability, dispY);
		dispY += 20;

		if (loc.durability <= 0) continue;
		
		// 当たり判定描画
		debug.DrawWeaponCol(loc.updateCol.colStartPos, loc.updateCol.colEndPos, loc.updateCol.colRadius);
	}
#endif
}

void Weapon::DrawWeaponUi()
{
	// バトル中でない場合は表示しない
	if (!m_pPlayer->GetIsBattle()) return;

	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->GetIsGrabWeapon()) return;

		if (loc.durability <= 0) continue;

		// プレイヤーが武器に近づいたら拾うUIを表示する
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			// UIの位置を計算する
			VECTOR modelTopPos = VAdd(loc.pos, VGet(0.0f, kDispTextAdjY, 0.0f));
			VECTOR screenPos = ConvWorldPosToScreenPos(modelTopPos);

			DrawGraphF(screenPos.x, screenPos.y, m_handle, true);
		}
	}
}

void Weapon::DecrementDurability()
{
	if (m_isHitAttack) return;
	
	for (auto& loc : m_locationData)
	{
		if (loc.isGrab)
		{
			loc.durability--;  // 耐久力を1減らす
			m_isHitAttack = true;
		}
	}
}

void Weapon::UpdateIsGrab(bool isGrab)
{
	for (auto& loc : m_locationData)
	{
		if (isGrab)
		{
			if (m_pPlayer->IsNearWeapon(loc.pos))
			{
				loc.isGrab = true;
			}
		}
		else
		{
			loc.isGrab = false;
		}
	}
}

std::string Weapon::GetNearWeaponTag() const
{
	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			return loc.tag;
		}
	}

	return "";
}

void Weapon::LoadLocationData()
{
	m_loadLocationData = FileRead_open((kWeaponFileName + "locationData.loc").c_str());

	int dataCnt = 0; // データ数
	FileRead_read(&dataCnt, sizeof(dataCnt), m_loadLocationData);
	m_locationData.resize(dataCnt);

	for (auto& loc : m_locationData)
	{
		// オブジェクトIDをロード
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), m_loadLocationData);
		loc.id.resize(nameCnt);

		// MEMO:loc.id.data()の部分はC++20だとエラーにならない
		FileRead_read(loc.id.data(), static_cast<int>(sizeof(char)* loc.id.size()), m_loadLocationData);

		// タグをロード
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), m_loadLocationData);
		loc.tag.resize(tagCnt);
		FileRead_read(loc.tag.data(), static_cast<int>(sizeof(char) * loc.tag.size()), m_loadLocationData);

		// 座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), m_loadLocationData);
		loc.initPos = loc.pos;
		// 回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), m_loadLocationData);
		loc.initRot = loc.rot;
		// スケール情報
		FileRead_read(&loc.scale, sizeof(loc.scale), m_loadLocationData);

		// 武器名
		loc.name = m_weaponData[loc.id].name;
	}
	FileRead_close(m_loadLocationData);

	// モデルのパスを設定
	for (auto& loc : m_locationData)
	{
		if (m_objHandle.find(loc.id) == m_objHandle.end())
		{
			std::string modelPath = kWeaponFileName + loc.name + ".mv1";
			m_objHandle[loc.id] = MV1LoadModel(modelPath.c_str());
		}
	}
}

void Weapon::UpdateCol(auto& loc)
{
	// 向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(loc.rot.z);

	// 当たり判定位置を更新
	loc.updateCol.colStartPos = VAdd(loc.pos, (VTransform(m_weaponData[loc.id].colStartPos, rotationMatrix)));
	loc.updateCol.colEndPos = VAdd(loc.updateCol.colStartPos, (VTransform(m_weaponData[loc.id].colEndPos, rotationMatrix)));
}

bool Weapon::CheckWeaponCol(const CharacterBase::ColData& colData, Player& player)
{
	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			return HitCheck_Capsule_Capsule(loc.updateCol.colStartPos, loc.updateCol.colEndPos, loc.updateCol.colRadius, colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
		}	
	}

	return false;
}

void Weapon::SetModelFramePos(auto& loc, MATRIX frameMatrix)
{
	MATRIX rotMatrix; // 回転行列

	// プレイヤーの手の位置を取得
	frameMatrix = MV1GetFrameLocalWorldMatrix(m_pPlayer->GetHandle(), kPlayerHandFrameNum);

	// 武器位置を更新
	loc.pos = VTransform(m_weaponData[loc.id].grabPos, frameMatrix);

	// 武器の回転行列を作成
	CreateRotationXYZMatrix(&rotMatrix, m_weaponData[loc.id].grabRot.x, m_weaponData[loc.id].grabRot.y, m_weaponData[loc.id].grabRot.z);
	MV1SetMatrix(m_objHandle[loc.id], MMult(rotMatrix, frameMatrix));
}

void Weapon::ResetWeapon(MATRIX frameMatrix, auto& loc)
{
	if (!m_isResetPos) return;
	
	loc.pos = VAdd(loc.initPos, m_pPlayer->GetPos());
	loc.pos.y = loc.initPos.y;
	loc.rot = loc.initRot;
	loc.durability = m_weaponData[loc.id].durability;
	loc.isGrab = false;
	frameMatrix = MGetIdent();	// 単位行列を設定
	MV1SetMatrix(m_objHandle[loc.id], frameMatrix);

	m_isResetPos = false;
}

void Weapon::InvisibleWeapon(auto& loc)
{
	// モデルを非表示し、当たり判定を消す
	MV1SetFrameVisible(m_objHandle[loc.id], 0, false);
	m_pPlayer->SetIsGrabWeapon(false); // プレイヤーの武器掴み状態を解除する
	loc.isGrab = false;
	loc.durability = std::max(loc.durability, 0);

	// エフェクトを停止
	EffectManager::GetInstance().StopWeaponEffect(loc.id);
}
