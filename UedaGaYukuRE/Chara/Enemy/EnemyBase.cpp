#include "Vec2.h"
#include "Game.h"
#include "DebugDraw.h"
#include "LoadCsv.h"
#include "ModelFrameName.h"
#include "UiBar.h"
#include "Item.h"
#include "Player.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

// 定数
namespace
{
	constexpr float kScale = 0.15f;					// モデルの拡大率
	constexpr float kSpawnRange = 500.0f;			// スポーンする範囲
	const VECTOR kBossSpwnPos = VGet(9100.0f, 45.0f, 4000.0f); // ボスのスポーン位置

	constexpr float kDispNameRange = 1000.0f;		// 敵名を表示する範囲
	const Vec2 kAdjDispNamePos = { 32.0f, 30.0f };	// 敵名の表示位置調整
}

EnemyBase::EnemyBase(std::shared_ptr<UiBar> pUi, std::shared_ptr<Item> pItem, Player& player):
	m_enemyName(""),
	m_enemyIndex(0),
	m_eToPVec(VGet(0.0f, 0.0f, 0.0f)),
	m_isDead(false)
{
	m_pUiBar = pUi;
	m_pItem = pItem;

	// 敵の初期位置を設定
	if (m_enemyIndex == CharacterBase::CharaType::kEnemy_boss)
	{
		m_pos = kBossSpwnPos;
	}
	else
	{
		// プレイヤーの範囲内に配置する
		float randPosX = player.GetPos().x + GetRand(static_cast<int>(kSpawnRange) * 2) - kSpawnRange;
		float randPosZ = player.GetPos().z + GetRand(static_cast<int>(kSpawnRange) * 2) - kSpawnRange;
		m_pos = VGet(randPosX, player.GetPos().y, randPosZ);
	}

	//m_pEnemyAI = std::make_shared<EnemyAI>(*this, m_pEnemyAI);
}

EnemyBase::~EnemyBase()
{
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	m_pState = nullptr;
	m_pState = std::make_shared<EnemyStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;

	auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pState);
	state->Init();
}

void EnemyBase::Update(Stage& stage, Player& player)
{
	if (nullptr) return;

	CharacterBase::Update();

	// 前のフレームと違うstateの場合
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	m_eToPVec = VSub(player.GetPos(), m_pos);

	// 当たり判定をチェックする
	player.CheckCharaCol(*this, m_colData[m_enemyIndex], CharaType::kPlayer);

	m_pState->Update(stage, player); // stateの更新
	UpdateAngle();					 // 向きを更新
	UpdateAnim();					 // アニメーションを更新
	UpdateCol(m_enemyIndex);		 // 当たり判定位置更新
	UpdatePosLog();					 // 位置ログを更新
	GetFramePos();					 // モデルフレーム位置を取得

	m_pUiBar->Update(); // HPバーの更新
}

void EnemyBase::Draw(Player& player)
{
	CharacterBase::Draw();

	// 回避中は残像を表示する
	if (m_pState->GetKind() == EnemyStateBase::EnemyStateKind::kAvoid)
	{
		DrawAfterImage();
	}

	// プレイヤーに近づいたら敵名を表示する
	VECTOR modelTopPos = VAdd(m_pos, VGet(0.0f, kAdjDispNamePos.y, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(modelTopPos);
	bool isViewClip = CheckCameraViewClip(modelTopPos); // カメラの視界内に入っているか(true:視界に入っていない)

	bool isDispName = VSize(m_eToPVec) < kDispNameRange && !isViewClip;
	if (isDispName)
	{
		m_pUiBar->DrawEnemyHpBar(*this);
		DrawFormatStringF(screenPos.x - kAdjDispNamePos.x, screenPos.y - kAdjDispNamePos.y, Color::kColorW, "%s", m_enemyName.c_str());
	}

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, m_enemyIndex, m_pState->GetStateName()); // 敵の情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_colData[m_enemyIndex]);// 全身(紫色)
	//debug.DrawArmCol(m_colData[m_enemyIndex]);	// 腕(水色)
	//debug.DrawLegCol(m_colData[m_enemyIndex]);	// 脚(黄色)
#endif
}

void EnemyBase::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);
	m_pUiBar->SetEnemyDamage(damage);
}

void EnemyBase::SetEnemyInfo(std::string name, std::string charaId, int index, int modelHandle)
{
	m_enemyName = name;
	m_enemyIndex = index;
	m_modelHandle = modelHandle;

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, charaId);
	LoadCsv::GetInstance().LoadColData(m_colData[m_enemyIndex], charaId);

	m_colData[m_enemyIndex].bodyUpdateStartPos = m_colData[m_enemyIndex].bodyStartPos;
	m_colData[m_enemyIndex].bodyUpdateEndPos = m_colData[m_enemyIndex].bodyEndPos;
}

void EnemyBase::GetFramePos()
{
	std::string enemyRig;
	if (m_enemyIndex == CharaType::kEnemy_01)
	{
		enemyRig = "mixamorig12:";
	}
	else
	{
		enemyRig = "mixamorig:";
	}

	m_colData[m_enemyIndex].leftShoulderPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftShoulder).c_str());		// 左肩
	m_colData[m_enemyIndex].leftForeArmPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftForeArm).c_str());		// 左肘
	m_colData[m_enemyIndex].leftHandPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftHandPos).c_str());			// 左手首
	m_colData[m_enemyIndex].rightShoulderPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightShoulder).c_str());	// 右肩
	m_colData[m_enemyIndex].rightForeArmPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightForeArm).c_str());		// 右肘
	m_colData[m_enemyIndex].rightHandPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightHand).c_str());			// 右手首

	m_colData[m_enemyIndex].leftUpLegPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftUpLeg).c_str());			// 左もも
	m_colData[m_enemyIndex].leftLegPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftLeg).c_str());				// 左膝
	m_colData[m_enemyIndex].leftFootPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftFoot).c_str());				// 左足首
	m_colData[m_enemyIndex].leftEndPos = GetModelFramePos((enemyRig + EnemyFrameName::kLeftEnd).c_str());				// 左足終点
	m_colData[m_enemyIndex].rightUpLegPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightUpLeg).c_str());			// 右もも
	m_colData[m_enemyIndex].rightLegPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightLeg).c_str());				// 右膝
	m_colData[m_enemyIndex].rightFootPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightFoot).c_str());			// 右足首
	m_colData[m_enemyIndex].rightEndPos = GetModelFramePos((enemyRig + EnemyFrameName::kRightEnd).c_str());				// 右足終点
}
