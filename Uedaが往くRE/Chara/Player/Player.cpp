#include "DxLib.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1"); // モデルのファイル名
	const VECTOR kInitPos = VGet(0.0f, 0.0f, 0.0f);				  // 初期位置
	constexpr float kScale = 0.1f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	CharacterBase::~CharacterBase();
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();
}
