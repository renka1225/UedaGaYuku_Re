#include "DebugDraw.h"
#include "LoadCsv.h"
#include "Player.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

// 定数
namespace
{
	constexpr float kScale = 0.15f;	 // モデルの拡大率
	const VECTOR kInitPos = VGet(7600.0, 40.0f, 5300.0f);	// 初期位置
}

EnemyBase::EnemyBase(std::string charaId)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, charaId);
	LoadCsv::GetInstance().LoadColData(m_colData, charaId);

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
	player.CheckCharaCol(*this, m_updateCol.bodyStartPos, m_updateCol.bodyEndPos, m_colData.bodyRadius);

	m_pState->Update(stage); // stateの更新
	UpdateAnim();			 // アニメーションを更新
	UpdateCol();			 // 当たり判定の位置更新

	if (m_hp <= 0)
	{
		Death();
	}
}

void EnemyBase::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, m_pState->GetStateName()); // 敵の情報を描画
#endif
}

void EnemyBase::Death()
{
	// TODO:敵が倒れるようにする
	// 倒れた数秒後に消える
}
