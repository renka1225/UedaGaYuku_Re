#pragma once
#include "EnemyBase.h"

/// <summary>
/// 特殊敵のクラス
/// </summary>
class EnemySpecial : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pUi">UIポインタ</param>
	/// <param name="pItem">アイテムポインタ</param>
	/// <param name="pPlayer">プレイヤー参照</param>
	EnemySpecial(std::shared_ptr<UiBar> pUi, std::shared_ptr<Item> pItem, Player& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemySpecial();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pStage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照</param>
	virtual void Update(Stage& pStage, Player& pPlayer) override;
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	virtual void Draw(Player& player) override;

private:
	/// <summary>
	/// 敵の初期位置をセットする
	/// </summary>
	virtual void SetEnemySpawnInitPos();
};

