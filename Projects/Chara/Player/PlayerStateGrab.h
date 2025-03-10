﻿#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 掴み状態
/// </summary>
class PlayerStateGrab : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	PlayerStateGrab(const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateGrab() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="grabKind">掴んだ物の種類</param>
	void Init(std::string grabKind);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kGrab; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override;
#endif

private:
	std::string m_grabKind;	// 掴んだ物の種類
};

