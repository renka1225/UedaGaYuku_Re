#include "DebugDraw.h"
#include "LoadCsv.h"
#include "CharacterBase.h"

// 定数
namespace
{
	constexpr float kAdj = 3.0f;			 // 敵に当たった際の位置調整量
	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
	constexpr float kAnimBlendSpeed = 0.2f;	 // アニメーションブレンドの変化速度
}

CharacterBase::CharacterBase():
	m_colData(),
	m_updateCol(),
	m_animData(),
	m_status(),
	m_angle(0.0f),
	m_hp(0.0f),
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

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

void CharacterBase::Init()
{
	LoadCsv::GetInstance().LoadAnimData(m_animData);
	m_hp = m_status.maxHp;

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(m_modelHandle, -1);
}

void CharacterBase::Update()
{
	ObjectBase::Update();
}

void CharacterBase::Draw()
{
	MV1DrawModel(m_modelHandle);
}

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

void CharacterBase::UpdateCol()
{
	// プレイヤーの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(m_angle);

	// プレイヤー全体の当たり判定位置を更新
	m_updateCol.bodyStartPos = VAdd(m_pos, (VTransform(m_colData.bodyStartPos, rotationMatrix)));
	m_updateCol.bodyEndPos = VAdd(m_updateCol.bodyStartPos, (VTransform(m_colData.bodyEndPos, rotationMatrix)));

	// 腕の当たり判定位置を更新
	m_updateCol.armStartPos = VAdd(m_pos, (VTransform(m_colData.armStartPos, rotationMatrix)));
	m_updateCol.armEndPos = VAdd(m_updateCol.armStartPos, (VTransform(m_colData.armEndPos, rotationMatrix)));

	// 脚の当たり判定位置を更新
	m_updateCol.legStartPos = VAdd(m_pos, (VTransform(m_colData.legStartPos, rotationMatrix)));
	m_updateCol.legEndPos = VAdd(m_updateCol.legStartPos, (VTransform(m_colData.legEndPos, rotationMatrix)));
}

void CharacterBase::CheckCharaCol(ObjectBase& obj, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius)
{
	bool isHit = HitCheck_Capsule_Capsule(m_colData.bodyStartPos, m_colData.bodyEndPos, m_colData.bodyRadius, eCapPosTop, eCapPosBottom, eCapRadius);

	if (isHit)
	{
		// キャラクターの位置を補正する
		VECTOR colVec = VNorm(VSub(m_pos, obj.GetPos()));
		m_pos = VAdd(m_pos, VScale(colVec, kAdj));
	}
}
