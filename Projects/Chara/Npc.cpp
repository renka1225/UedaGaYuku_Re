#include "LoadCsv.h"
#include "Player.h"
#include "Npc.h"

namespace
{
	const std::string kCharaId = "npc";				   // キャラクターのID名
	const VECTOR kPos = VGet(8769.0f, 40.0f, 3143.0f); // 表示位置
	constexpr float kScale = 0.15f;					   // 拡大率
}

Npc::Npc(int modelHandle)
{
	m_modelHandle = modelHandle;
	m_pos = kPos;

	LoadCsv::GetInstance().LoadColData(m_colData[CharaType::kNpc], kCharaId);

	m_colData[CharaType::kNpc].bodyUpdateStartPos = m_colData[CharaType::kNpc].bodyStartPos;
	m_colData[CharaType::kNpc].bodyUpdateEndPos = m_colData[CharaType::kNpc].bodyEndPos;
}

Npc::~Npc()
{
}

void Npc::Init()
{
	CharacterBase::Init();
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	MV1SetPosition(m_modelHandle, m_pos);
	ChangeAnim(AnimName::kIdleStand);
}

void Npc::Update(Stage& pStage, const Player& pPlayer)
{
	CharacterBase::Update();

	// ステージと当たり判定を行って座標を保存する
	m_pos = pStage.CheckObjectCol(*this, VGet(0.0f, 0.0f, 0.0f));

	UpdateAnim();				// アニメーションを更新
	UpdateCol(CharaType::kNpc);	// 当たり判定の位置更新

	if (!pPlayer.GetIsBattle())
	{
		m_moveDir = VSub(pPlayer.GetPos(), m_pos); // プレイヤーの方を向く
	}

	UpdateAngle();
	
	MV1SetPosition(m_modelHandle, m_pos);
}

void Npc::Draw()
{
	CharacterBase::Draw();
}
