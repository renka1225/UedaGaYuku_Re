#include "DebugDraw.h"
#include "LoadCsv.h"
#include "ModelFrameName.h"
#include "Player.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

// TODO:敵のタイプをIDから取得できるようにする

// 定数
namespace
{
	constexpr float kScale = 0.15f;		 // モデルの拡大率
	constexpr float kSpawnRange = 50.0f; // スポーンする範囲
}

EnemyBase::EnemyBase(Player& player, std::string charaId, int number, int modelHandle):
	m_enemyNumber(number),
	m_isDead(false)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, charaId);
	LoadCsv::GetInstance().LoadColData(m_colData[m_enemyNumber], charaId);

	m_modelHandle = modelHandle;
	m_colData[m_enemyNumber].bodyUpdateStartPos = m_colData[m_enemyNumber].bodyStartPos;
	m_colData[m_enemyNumber].bodyUpdateEndPos = m_colData[m_enemyNumber].bodyEndPos;

	// 敵の初期位置を設定
	// プレイヤーの範囲内に配置する
	float randPosX = player.GetPos().x + GetRand(kSpawnRange * 2) - kSpawnRange;
	float randPosZ = player.GetPos().z + GetRand(kSpawnRange * 2) - kSpawnRange;
	m_pos = VGet(randPosX, player.GetPos().y, randPosZ);
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

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

	// 当たり判定をチェックする
	player.CheckCharaCol(*this, m_colData[m_enemyNumber], CharaType::kPlayer);

	m_pState->Update(stage, player); // stateの更新
	UpdateAnim();					 // アニメーションを更新
	UpdateCol(m_enemyNumber);		 // 当たり判定位置更新
	GetFramePos();					 // モデルフレーム位置を取得
}

void EnemyBase::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, m_pState->GetStateName()); // 敵の情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_colData[m_enemyNumber]);// 全身(紫色)
	//debug.DrawArmCol(m_colData[m_enemyNumber]);	// 腕(水色)
	//debug.DrawLegCol(m_colData[m_enemyNumber]);	// 脚(黄色)
#endif
}

void EnemyBase::GetFramePos()
{
	m_colData[m_enemyNumber].leftShoulderPos = GetModelFramePos(EnemyFrameName::kLeftShoulder.c_str());		// 左肩
	m_colData[m_enemyNumber].leftForeArmPos = GetModelFramePos(EnemyFrameName::kLeftForeArm.c_str());		// 左肘
	m_colData[m_enemyNumber].leftHandPos = GetModelFramePos(EnemyFrameName::kLeftHandPos.c_str());			// 左手首
	m_colData[m_enemyNumber].rightShoulderPos = GetModelFramePos(EnemyFrameName::kRightShoulder.c_str());	// 右肩
	m_colData[m_enemyNumber].rightForeArmPos = GetModelFramePos(EnemyFrameName::kRightForeArm.c_str());		// 右肘
	m_colData[m_enemyNumber].rightHandPos = GetModelFramePos(EnemyFrameName::kRightHand.c_str());			// 右手首

	m_colData[m_enemyNumber].leftUpLegPos = GetModelFramePos(EnemyFrameName::kLeftUpLeg.c_str());			// 左もも
	m_colData[m_enemyNumber].leftLegPos = GetModelFramePos(EnemyFrameName::kLeftLeg.c_str());				// 左膝
	m_colData[m_enemyNumber].leftFootPos = GetModelFramePos(EnemyFrameName::kLeftFoot.c_str());				// 左足首
	m_colData[m_enemyNumber].leftEndPos = GetModelFramePos(EnemyFrameName::kLeftEnd.c_str());				// 左足終点
	m_colData[m_enemyNumber].rightUpLegPos = GetModelFramePos(EnemyFrameName::kRightUpLeg.c_str());			// 右もも
	m_colData[m_enemyNumber].rightLegPos = GetModelFramePos(EnemyFrameName::kRightLeg.c_str());				// 右膝
	m_colData[m_enemyNumber].rightFootPos = GetModelFramePos(EnemyFrameName::kRightFoot.c_str());			// 右足首
	m_colData[m_enemyNumber].rightEndPos = GetModelFramePos(EnemyFrameName::kRightEnd.c_str());				// 右足終点
}
