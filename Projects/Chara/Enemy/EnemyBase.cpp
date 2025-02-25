#include "Vec2.h"
#include "Game.h"
#include "DebugDraw.h"
#include "LoadCsv.h"
#include "ModelFrameName.h"
#include "Font.h"
#include "UiBar.h"
#include "Item.h"
#include "Player.h"
#include "EnemyAI.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

// 定数
namespace
{
	constexpr float kScale = 0.15f;		// モデルの拡大率

	constexpr float kFirstSpawnMinDist = 150.0f; // 1体目の敵のスポーン位置の最小距離
	constexpr float kFirstSpawnRange = 600.0f;	 // 1体目のスポーンする範囲
	constexpr float kSpawnRange = 50.0f;		 // 2体目以降のスポーンする範囲

	const VECTOR kTutoSpwnPos = VGet(8700.0f, 45.0f, 3100.0f); // チュートリアル敵のスポーン位置
	const VECTOR kBossSpwnPos = VGet(8994.0f, 45.0f, 2854.0f); // ボスのスポーン位置

	constexpr float kDispNameRange = 1000.0f;				// 敵名を表示する範囲
	const Vec2 kAdjDispNamePos = { 32.0f, 30.0f };			// 敵名の表示位置調整
	const Vec2 kSpecialEnemyNamePos = { 392.0f, 928.0f };	// 特殊敵の名前表示位置

	constexpr float kRecoveryHp = 5.0f;	// 回復量
}

EnemyBase::EnemyBase(std::shared_ptr<UiBar> pUi, std::shared_ptr<Item> pItem, Player& pPlayer):
	m_enemyName(""),
	m_enemyIndex(0),
	m_eToPVec(VGet(0.0f, 0.0f, 0.0f))
{
	m_pUiBar = pUi;
	m_pItem = pItem;
}

EnemyBase::~EnemyBase()
{
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	m_pState = std::make_shared<EnemyStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;

	auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pState);
	state->Init();

	m_pEnemyAI = std::make_shared<EnemyAI>(shared_from_this());
	m_pEnemyAI->Init(m_enemyIndex);

}

void EnemyBase::Update(Stage& pStage, Player& pPlayer)
{
	CharacterBase::Update();

	// チュートリアル中は死亡しない
	if (!pPlayer.GetTutoInfo().isEndTutorial && pPlayer.GetTutoInfo().currentNum <= Player::TutorialNum::kTuto_4)
	{
		m_hp = std::max(1.0f, m_hp);
	}

	// 会話中は動けないようにする
	if (pPlayer.GetTutoInfo().isNowKnowledge || pPlayer.GetTutoInfo().isTalk)
	{
		m_isPossibleMove = false;
	}

	// プレイヤーが必殺技を出している最中は動けない
	if (pPlayer.GetCurrentAnim() == AnimName::kSpecialAtk1 || pPlayer.GetCurrentAnim() == AnimName::kSpecialAtk1)
	{
		m_isPossibleMove = false;
	}

	// AIの更新
	m_pEnemyAI->Update();
	m_pEnemyAI->DecideNextAction(pPlayer);

	// stateを更新する
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}
	m_pState->Update(pStage, pPlayer);

	m_eToPVec = VSub(pPlayer.GetPos(), m_pos);

	// 当たり判定をチェックする
	pPlayer.CheckCharaCol(*this, m_colData[m_enemyIndex], CharaType::kPlayer);

	UpdateAngle();				// 向きを更新
	UpdateAnim();				// アニメーションを更新
	UpdateCol(m_enemyIndex);	// 当たり判定位置更新
	UpdatePosLog();				// 位置ログを更新
	GetFramePos();				// モデルフレーム位置を取得

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

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, m_enemyIndex, m_pState->GetStateName()); // 敵の情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_colData[m_enemyIndex]);// 全身(紫色)
	//debug.DrawArmCol(m_colData[m_enemyIndex]);	// 腕(水色)
	//debug.DrawLegCol(m_colData[m_enemyIndex]);	// 脚(黄色)
#endif
}

void EnemyBase::DrawUi()
{
	// 特殊敵
	bool isSpecialEnemy = (m_enemyIndex == CharacterBase::CharaType::kEnemy_boss) || (m_enemyIndex == CharacterBase::CharaType::kEnemy_bob) ||
		(m_enemyIndex == CharacterBase::CharaType::kEnemy_sato) || (m_enemyIndex == CharacterBase::CharaType::kEnemy_abe);
	if (isSpecialEnemy)
	{
		// 専用のHPバーを表示
		m_pUiBar->DrawSpecialEnemyHpBar(*this);
		// 名前表示
		DrawFormatStringFToHandle(kSpecialEnemyNamePos.x, kSpecialEnemyNamePos.y, Color::kColorW,
				Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialEnemyName)], "%s", m_enemyName.c_str());
	}
	// 通常敵
	else
	{
		// プレイヤーに近づいたら敵名を表示する
		VECTOR modelTopPos = VAdd(m_pos, VGet(0.0f, kAdjDispNamePos.y, 0.0f));
		VECTOR screenPos = ConvWorldPosToScreenPos(modelTopPos);
		bool isViewClip = CheckCameraViewClip(modelTopPos); // カメラの視界内に入っているか(true:視界に入っていない)

		bool isDispName = VSize(m_eToPVec) < kDispNameRange && !isViewClip;
		if (isDispName)
		{
			m_pUiBar->DrawEnemyHpBar(*this);
			DrawFormatStringFToHandle(screenPos.x - kAdjDispNamePos.x, screenPos.y - kAdjDispNamePos.y, Color::kColorW,
				Font::m_fontHandle[static_cast<int>(Font::FontId::kEnemyName)], "%s", m_enemyName.c_str());
		}
	}
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

void EnemyBase::SetEnemySpawnPos(const Player& pPlayer, int index)
{
	// チュートリアル
	if (m_enemyIndex == CharacterBase::CharaType::kEnemy_tuto)
	{
		m_pos = kTutoSpwnPos;
	}
	// ボス
	else if (m_enemyIndex == CharacterBase::CharaType::kEnemy_boss)
	{
		m_pos = kBossSpwnPos;
	}
	else
	{
		// 1体目の敵位置を保存する
		static VECTOR firstEnemyPos;

		// 1体目の位置を決める
		if (index == 0)
		{
			// 範囲内にスポーンさせる
			VECTOR spawnPos;
			while (true)
			{
				float firstRandPosX = pPlayer.GetPos().x + GetRand(static_cast<int>(kFirstSpawnRange) * 2) - kFirstSpawnRange;
				float firstRandPosZ = pPlayer.GetPos().z + GetRand(static_cast<int>(kFirstSpawnRange) * 2) - kFirstSpawnRange;

				spawnPos = VGet(firstRandPosX, pPlayer.GetPos().y, firstRandPosZ);
				float dist = VSize(VSub(spawnPos, pPlayer.GetPos()));

				if (dist >= kFirstSpawnMinDist)
				{
					break;
				}
			}

			m_pos = spawnPos;
			firstEnemyPos = m_pos;
		}
		// 2体目以降の敵位置を決める
		else
		{
			// 1体目の位置を基準にランダムな位置を生成
			float randPosX = firstEnemyPos.x + GetRand(static_cast<int>(kSpawnRange) * 2) - kSpawnRange;
			float randPosZ = firstEnemyPos.z + GetRand(static_cast<int>(kSpawnRange) * 2) - kSpawnRange;
			m_pos = VGet(randPosX, firstEnemyPos.y, randPosZ);
		}
	}
}

void EnemyBase::RecoveryHp()
{
	m_hp += kRecoveryHp;
	m_hp = std::min(m_hp, GetStatus().maxHp);
}

void EnemyBase::RecoveryMaxHp()
{
	m_hp = std::max(m_hp, GetStatus().maxHp);
}

void EnemyBase::GetFramePos()
{
	std::string enemyRig;
	if (m_enemyIndex == CharaType::kEnemy_01)
	{
		enemyRig = "mixamorig12:";
	}
	else if(m_enemyIndex == CharaType::kEnemy_03)
	{
		enemyRig = "mixamorig10:";
	}
	else if (m_enemyIndex == CharaType::kEnemy_tuto)
	{
		enemyRig = "mixamorig9:";
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