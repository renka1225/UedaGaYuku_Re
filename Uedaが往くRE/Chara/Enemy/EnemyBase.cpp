#include "DebugDraw.h"
#include "LoadCsv.h"
#include "Player.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

// TODO:敵のタイプをIDから取得できるようにする

// 定数
namespace
{
	constexpr float kScale = 0.15f;	 // モデルの拡大率
	const VECTOR kInitPos = VGet(7600.0, 40.0f, 5300.0f);	// 初期位置
}

EnemyBase::EnemyBase(std::string charaId):
	m_isDead(false)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, charaId);
	LoadCsv::GetInstance().LoadColData(m_colData[CharaType::kEnemy_01], charaId);

	m_modelHandle = MV1LoadModel(("data/model/chara/" + charaId + ".mv1").c_str());
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	m_pos = kInitPos;
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	m_pState = std::make_shared<EnemyStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;

	auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pState);
	state->Init();
}

void EnemyBase::Update(Stage& stage, Player& player)
{
	CharacterBase::Update();

	// 前のフレームと違うstateの場合
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	// 当たり判定をチェックする
	player.CheckCharaCol(*this, m_colData[CharaType::kEnemy_01]);

	m_pState->Update(stage, player); // stateの更新
	UpdateAnim();			 // アニメーションを更新
	UpdateCol();			 // 当たり判定の位置更新
	GetFramePos();			 // モデルフレーム位置を取得
}

void EnemyBase::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, m_pState->GetStateName()); // 敵の情報を描画
	// 当たり判定描画
	//debug.DrawAimCol(m_colData[CharaType::kEnemy_01]); // 腕(水色)
	//debug.DrawLegCol(m_colData[CharaType::kPlayer]);	 // 脚(黄色)
#endif
}

void EnemyBase::GetFramePos()
{
	//// フレームの座標を取得する
	//m_colData[CharaType::kEnemy_01].leftShoulderPos = GetModelFramePos(PlayerFrameName::kLeftShoulder.c_str());	// 左肩
	//m_colData[CharaType::kEnemy_01].leftForeArmPos = GetModelFramePos(PlayerFrameName::kLeftForeArm.c_str());		// 左肘
	//m_colData[CharaType::kEnemy_01].leftHandPos = GetModelFramePos(PlayerFrameName::kLeftHandPos.c_str());		// 左手首
	//m_colData[CharaType::kEnemy_01].rightShoulderPos = GetModelFramePos(PlayerFrameName::kRightShoulder.c_str()); // 右肩
	//m_colData[CharaType::kEnemy_01].rightForeArmPos = GetModelFramePos(PlayerFrameName::kRightForeArm.c_str());	// 右肘
	//m_colData[CharaType::kEnemy_01].rightHandPos = GetModelFramePos(PlayerFrameName::kRightHand.c_str());			// 右手首

	//m_colData[CharaType::kPlayer].leftUpLegPos = GetModelFramePos(PlayerFrameName::kLeftUpLeg.c_str());			// 左もも
	//m_colData[CharaType::kPlayer].leftLegPos = GetModelFramePos(PlayerFrameName::kLeftLeg.c_str());				// 左膝
	//m_colData[CharaType::kPlayer].leftFootPos = GetModelFramePos(PlayerFrameName::kLeftFoot.c_str());			// 左足首
	//m_colData[CharaType::kPlayer].leftEndPos = GetModelFramePos(PlayerFrameName::kLeftEnd.c_str());				// 左足終点
	//m_colData[CharaType::kPlayer].rightUpLegPos = GetModelFramePos(PlayerFrameName::kRightUpLeg.c_str());		// 右もも
	//m_colData[CharaType::kPlayer].rightLegPos = GetModelFramePos(PlayerFrameName::kRightLeg.c_str());			// 右膝
	//m_colData[CharaType::kPlayer].rightFootPos = GetModelFramePos(PlayerFrameName::kRightFoot.c_str());			// 右足首
	//m_colData[CharaType::kPlayer].rightEndPos = GetModelFramePos(PlayerFrameName::kRightEnd.c_str());			// 右足終点
}
