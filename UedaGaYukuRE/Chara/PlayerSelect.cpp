#include "PlayerSelect.h"

namespace
{
	const char* const kPlayerModelPath = "data/model/chara/player_select.mv1"; // プレイヤーのモデルパス
	constexpr float kModelScale = 1.0f;	  // モデルの拡大率
	constexpr float kAngle = DX_PI_F / 7; // プレイヤーの角度
	const std::map<int, VECTOR> kPos
	{
		{0, VGet(90.0f, -70.0f, -10.0f)}, // 続きから
		{1, VGet(90.0f, -70.0f, -10.0f)}, // 最初から
		{2, VGet(90.0f, -35.0f, -10.0f)}, // 設定
		{3, VGet(120.0f, 0.0f, 0.0f)},	  // ゲーム終了
	};
}

PlayerSelect::PlayerSelect()
{
	m_modelHandle = MV1LoadModel(kPlayerModelPath);
	MV1SetPosition(m_modelHandle, kPos.at(0));
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, kAngle, 0.0f)); // 角度を更新
}

PlayerSelect::~PlayerSelect()
{
	MV1DeleteModel(m_modelHandle);
}

void PlayerSelect::Init()
{
}

void PlayerSelect::Update()
{
	UpdateAnim();// アニメーションを更新
}

void PlayerSelect::Draw()
{
	CharacterBase::Draw();
}

void PlayerSelect::ChangePos(int select)
{
	// 表示位置を変更
	switch (select)
	{
	case 0:
		m_pos = kPos.at(0);
			break;
	case 1:
		m_pos = kPos.at(1);
		break;
	case 2:
		m_pos = kPos.at(2);
		break;
	case 3:
		break;
	case 4:
		m_pos = kPos.at(3);
		break;
	}

	MV1SetPosition(m_modelHandle, m_pos);
}