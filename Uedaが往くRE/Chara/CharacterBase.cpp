#include "DebugDraw.h"
#include "CharacterBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
CharacterBase::CharacterBase():
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1)
{
}

/// <summary>
/// デストラクタ
/// </summary>
CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
void CharacterBase::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void CharacterBase::Update()
{
}

/// <summary>
/// 描画
/// </summary>
void CharacterBase::Draw()
{
	MV1DrawModel(m_modelHandle);
}

/// <summary>
/// アニメーションを変更する
/// </summary>
/// <param name="animName">アニメーション名</param>
void CharacterBase::ChangeAnim(std::string animName)
{
}
