#include "DxLib.h"
#include "Input.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1");	// モデルのファイル名
	const VECTOR kInitPos = VGet(3000.0f, 12.0f, 3000.0f);			// 初期位置
	constexpr float kScale = 0.05f;	// 拡大率
	constexpr float kMove = 1.0f;	// 移動量
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
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	Move(input);
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="moveVec">移動ベクトル参照</param>
/// <param name="stage">ステージ情報参照</param>
void Player::Move(const Input& input)
{
	if (input.IsPressing("right"))
	{
		m_pos.x += kMove;
	}
	if (input.IsPressing("left"))
	{
		m_pos.x -= kMove;
	}
	if (input.IsPressing("up"))
	{
		m_pos.z += kMove;
	}
	if (input.IsPressing("down"))
	{
		m_pos.z -= kMove;
	}

	MV1SetPosition(m_modelHandle, m_pos);
}
