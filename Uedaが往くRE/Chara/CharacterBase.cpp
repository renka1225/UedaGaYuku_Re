#include "DebugDraw.h"
#include "LoadCsv.h"
#include "UiBar.h"
#include "CharacterBase.h"

// 定数
namespace
{
	constexpr int kColDataNum = 19; // 当たり判定情報数
	constexpr float kAdj = 2.5f;	// 敵に当たった際の位置調整量

	constexpr float kSlowAnimSpeed = 0.3f;	// スローモーション時のアニメーション速度

	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
	constexpr float kAnimBlendSpeed = 0.2f;	 // アニメーションブレンドの変化速度
	constexpr int kPosLogNum = 6;			 // 覚えておく過去の位置情報の数
	constexpr float kAlphaAdj = 0.3f;		 // 残像の透明度を調整
}

CharacterBase::CharacterBase():
	m_colData(),
	m_animData(),
	m_status(),
	m_angle(0.0f),
	m_hp(0.0f),
	m_gauge(0.0f),
	m_isAttack(false),
	m_isInvincible(false),
	m_isGuard(false),
	m_isOnDamage(false),
	m_isPossibleGrabEnemy(false),
	m_isPossibleGrabWeapon(false),
	m_isNowGrabWeapon(false),
	m_isPossibleMove(true),
	m_isAnimSlow(false),
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
	m_colData.resize(kColDataNum);
	m_posLog.resize(kPosLogNum);
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

void CharacterBase::OnDamage(float damage)
{
	m_isOnDamage = true;
	m_hp -= damage;
	m_hp = std::max(0.0f, m_hp);
}

void CharacterBase::CheckCharaCol(ObjectBase& obj, const CharacterBase::ColData& colData, int charaType)
{
	bool isHit = HitCheck_Capsule_Capsule(m_colData[charaType].bodyUpdateStartPos, m_colData[charaType].bodyUpdateEndPos, m_colData[charaType].bodyRadius, 
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);

	// 当たっている場合
	if (isHit)
	{
		// キャラクターの位置を補正する
		VECTOR colVec = VNorm(VSub(m_pos, obj.GetPos()));
		m_pos = VAdd(m_pos, VScale(colVec, kAdj));
	}
}

bool CharacterBase::CheckHitPunchCol(const CharacterBase::ColData& colData, int charaType)
{
	// 左肩から左肘
	bool isHit1 = HitCheck_Capsule_Capsule(m_colData[charaType].leftShoulderPos, m_colData[charaType].leftForeArmPos, m_colData[charaType].armRadius, 
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 左肘から左手首
	bool isHit2 = HitCheck_Capsule_Capsule(m_colData[charaType].leftForeArmPos, m_colData[charaType].leftHandPos, m_colData[charaType].armRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 右肩から右肘
	bool isHit3 = HitCheck_Capsule_Capsule(m_colData[charaType].rightShoulderPos, m_colData[charaType].rightForeArmPos, m_colData[charaType].armRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 右肘から右手首
	bool isHit4 = HitCheck_Capsule_Capsule(m_colData[charaType].rightShoulderPos, m_colData[charaType].rightForeArmPos, m_colData[charaType].armRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);

	if (isHit1 || isHit2 || isHit3 || isHit4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CharacterBase::CheckHitKickCol(const CharacterBase::ColData& colData, int charaType)
{
	// 左ももから左膝
	bool isHit1 = HitCheck_Capsule_Capsule(m_colData[charaType].leftUpLegPos, m_colData[charaType].leftLegPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 左膝から左足首
	bool isHit2 = HitCheck_Capsule_Capsule(m_colData[charaType].leftLegPos, m_colData[charaType].leftFootPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 左足首から左つま先
	bool isHit3 = HitCheck_Capsule_Capsule(m_colData[charaType].leftFootPos, m_colData[charaType].leftEndPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 右ももから右膝
	bool isHit4 = HitCheck_Capsule_Capsule(m_colData[charaType].rightUpLegPos, m_colData[charaType].rightLegPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 右膝から右足首
	bool isHit5 = HitCheck_Capsule_Capsule(m_colData[charaType].rightLegPos, m_colData[charaType].rightFootPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
	// 右足首から右つま先
	bool isHit6 = HitCheck_Capsule_Capsule(m_colData[charaType].rightFootPos, m_colData[charaType].rightEndPos, m_colData[charaType].legRadius,
		colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);

	if (isHit1 || isHit2 || isHit3 || isHit4 || isHit5 || isHit6)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterBase::ChangeAnim(std::string animName)
{
	m_currenAnimName = animName;

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
	if (m_isAnimSlow)
	{
		m_animPlaySpeed = kSlowAnimSpeed;
	}
	else
	{
		m_animPlaySpeed = m_animData[m_currenAnimName].playSpeed;
	}

	m_animLoopStartTime = m_animData[m_currenAnimName].loopFrame;
	m_animLoopEndTime = GetAnimTotalTime(m_currenAnimName);

	// ループ終了フレームが0でない場合、ループフラグを立てる
	if (m_animLoopEndTime > 0.0f)
	{
		m_isLoopAnim = true;
	}

	// 新たにアニメーションをアタッチする
	m_currentPlayAnim = MV1AttachAnim(m_modelHandle, GetAnimIndex(m_currenAnimName));
	m_currentAnimTime = 0.0f;
	//アニメーションの総再生時間を設定
	m_totalAnimTime = GetAnimTotalTime(m_currenAnimName);

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

void CharacterBase::SlowAnim()
{
	m_isAnimSlow = true;
}

void CharacterBase::ResetAnim()
{
	m_animPlaySpeed = m_animData[m_currenAnimName].playSpeed;
	m_isAnimSlow = false;
}

void CharacterBase::UpdateCol(int charType)
{
	// キャラクターの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(m_angle);

	// キャラクター全体の当たり判定位置を更新
	m_colData[charType].bodyUpdateStartPos = VAdd(m_pos, (VTransform(m_colData[charType].bodyStartPos, rotationMatrix)));
	m_colData[charType].bodyUpdateEndPos = VAdd(m_colData[charType].bodyUpdateStartPos, (VTransform(m_colData[charType].bodyEndPos, rotationMatrix)));
}

void CharacterBase::UpdateAngle()
{
	m_angle = atan2f(m_moveDir.x, m_moveDir.z);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

void CharacterBase::UpdatePosLog()
{
	// 位置ログをずらす
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 1; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
	}
	// 1フレーム前の位置情報を入れる
	m_posLog[0] = m_pos;
}

void CharacterBase::DrawAfterImage()
{
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 0; i--)
	{
		// ログが古くなるほど透明になるようにする
		float alpha = (1.0f - (static_cast<float>(i) / m_posLog.size())) * kAlphaAdj;
		MV1SetOpacityRate(m_modelHandle, alpha);
		MV1SetPosition(m_modelHandle, m_posLog[i]);
		MV1DrawModel(m_modelHandle);
	}
	MV1SetOpacityRate(m_modelHandle, 1.0f); // 透明度を戻す
}

float CharacterBase::GetAnimTotalTime(std::string animName)
{
	int animIndex = GetAnimIndex(animName);
	float totalTime = MV1GetAnimTotalTime(m_modelHandle, animIndex);

	// スローモーション再生中の場合
	if (m_isAnimSlow)
	{
		totalTime /= kSlowAnimSpeed;
	}

	return totalTime;
}

int CharacterBase::GetAnimIndex(std::string animName)
{
	return MV1GetAnimIndex(m_modelHandle, ("Armature|" + animName).c_str());
}