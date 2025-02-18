#include "SceneSelect.h"
#include "PlayerSelect.h"

namespace
{
	const char* const kPlayerModelPath = "data/model/chara/player_select.mv1"; // プレイヤーのモデルパス
	constexpr float kModelScale = 1.0f;			// モデルの拡大率
	constexpr float kBackAngle = DX_PI_F;		// 後ろ向きの角度
	constexpr float kFrontAngle = DX_PI_F / 7;	// 正面の角度
	constexpr float kBesideAngle = DX_PI_F / 3;	// 横向きの角度
	const std::map<int, VECTOR> kPos
	{
		{SceneSelect::SelectScene::kContinue, VGet(85.0f, -70.0f, -10.0f)}, // 続きから
		{SceneSelect::SelectScene::kFirst, VGet(85.0f, -70.0f, -10.0f)},	// 最初から
		{SceneSelect::SelectScene::kOption, VGet(95.0f, -35.0f, 0.0f)},		// 設定
		{SceneSelect::SelectScene::kGameEnd, VGet(120.0f, 0.0f, 0.0f)},		// ゲーム終了
	};
}

PlayerSelect::PlayerSelect()
{
	m_modelHandle = MV1LoadModel(kPlayerModelPath);
	MV1SetPosition(m_modelHandle, kPos.at(0));
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, kBackAngle, 0.0f)); // 角度を更新
}

PlayerSelect::~PlayerSelect()
{
	MV1DeleteModel(m_modelHandle);
}

void PlayerSelect::Init()
{
	CharacterBase::Init();
}

void PlayerSelect::Update()
{
	UpdateAnim();// アニメーションを更新
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f)); // 角度を更新
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
	case SceneSelect::SelectScene::kContinue:
		m_pos = kPos.at(SceneSelect::SelectScene::kContinue);
		m_angle = kBackAngle;
			break;
	case SceneSelect::SelectScene::kFirst:
		m_pos = kPos.at(SceneSelect::SelectScene::kFirst);
		m_angle = kFrontAngle;
		break;
	case SceneSelect::SelectScene::kOption:
		m_pos = kPos.at(SceneSelect::SelectScene::kOption);
		m_angle = kBesideAngle;
		break;
	case SceneSelect::SelectScene::kCopyright:
		break;
	case SceneSelect::SelectScene::kGameEnd:
		m_pos = kPos.at(SceneSelect::SelectScene::kGameEnd);
		m_angle = kFrontAngle;
		break;
	}

	MV1SetPosition(m_modelHandle, m_pos);
}