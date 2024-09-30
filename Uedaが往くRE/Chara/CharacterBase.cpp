#include "DebugDraw.h"
#include "CsvLoad.h"
#include "CharacterBase.h"

// 定数
namespace
{
	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
}

/// <summary>
/// コンストラクタ
/// </summary>
CharacterBase::CharacterBase():
	m_animData(),
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_prevPlayAnim(-1),
	m_animPlaySpeed(0.0f),
	m_currentAnimTime(0.0f),
	m_prevAnimTime(0.0f),
	m_totalAnimTime(0.0f),
	m_animBlendRate(0.0f)
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
	//LoadAnimData(CsvLoad::GetInstance.LoadAnimData());
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
	// 前のアニメーションをデタッチする
	if (m_prevAnimTime != -1)
	{
		MV1DetachAnim(m_modelHandle, static_cast<int>(m_prevAnimTime));
	}

	// 新たにアニメーションをアタッチする
	m_prevAnimTime = MV1AttachAnim(m_modelHandle, m_animData.number);
	//アニメーションの総再生時間を設定
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData.number);

	//アニメの再生速度を設定
	m_animPlaySpeed = m_animData.playSpeed;
}


/// <summary>
/// アニメーションを更新
/// </summary>
void CharacterBase::UpdateAnim()
{
	// ブレンド率が1以下の場合
	//if (m_animBlendRate < kAnimBlendMax)
	//{
	//	m_animBlendRate += m_animPlaySpeed;
	//	m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	//}

	// アニメーションの再生フレームを進める
	m_currentAnimTime += m_animPlaySpeed;

	if (m_currentAnimTime > m_totalAnimTime)
	{
		m_currentAnimTime = 0.0f;
		m_animPlaySpeed = 0.0f;
	}

	// 再生時間を更新
	MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimTime);
}
