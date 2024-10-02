#include "DebugDraw.h"
#include "LoadCsv.h"
#include "CharacterBase.h"

// 定数
namespace
{
	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
	constexpr float kAnimBlendSpeed = 0.2f;	 // アニメーションブレンドの変化速度
}

/// <summary>
/// コンストラクタ
/// </summary>
CharacterBase::CharacterBase():
	m_animData(),
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_prevPlayAnim(-1),
	m_animBlendRate(0.0f),
	m_animPlaySpeed(0.0f),
	m_currentAnimTime(0.0f),
	m_prevAnimTime(0.0f),
	m_totalAnimTime(0.0f),
	m_animLoopStartTime(0.0f),
	m_animLoopEndTime(0.0f),
	m_isLoopAnim(false)
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
	LoadCsv::GetInstance().LoadAnimData(m_animData);
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
		MV1DetachAnim(m_modelHandle, static_cast<int>(m_prevPlayAnim));
		m_prevPlayAnim = -1;
	}

	// 再生中のアニメーションを1つ前に移動する
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimTime = m_currentAnimTime;

	// アニメーションを設定
	m_animPlaySpeed = m_animData[animName].playSpeed;
	m_animLoopStartTime = m_animData[animName].loopFrame;
	m_animLoopEndTime = m_animData[animName].endFrame;

	// ループ終了フレームが0でない場合、ループフラグを立てる
	if (m_animLoopEndTime > 0)
	{
		m_isLoopAnim = true;
	}

	// 新たにアニメーションをアタッチする
	m_currentPlayAnim = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	m_currentAnimTime = 0.0f;
	//アニメーションの総再生時間を設定
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].number);

	// ブレンド率はprevが有効でない場合、1.0にする
	if (m_prevPlayAnim == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else
	{
		m_animBlendRate = 0.0f;
	}
}

/// <summary>
/// アニメーションを更新
/// </summary>
void CharacterBase::UpdateAnim()
{
	// ブレンド率が1以下の場合
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimBlendSpeed;
		m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	}

	// アニメーションの再生時間を進める
	m_currentAnimTime += m_animPlaySpeed;

	// アニメーションが繰り返し行われる場合
	if (m_isLoopAnim)
	{
		if (m_currentAnimTime > m_animLoopEndTime)
		{
			m_currentAnimTime = m_animLoopStartTime;
		}
	}

	// アニメーションの総再生時間を超えた場合
	if (m_currentAnimTime > m_totalAnimTime)
	{
		m_currentAnimTime = 0.0f;
		m_animPlaySpeed = 0.0f;
	}

	// 再生時間を更新
	MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimTime);
	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevPlayAnim, kAnimBlendMax - m_animBlendRate);
}
