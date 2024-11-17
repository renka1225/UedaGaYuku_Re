#pragma once
#include "UiBase.h"
#include <memory>
#include <vector>

class EnemyBase;
class Player;

/// <summary>
/// バーの表示を管理するクラス
/// </summary>
class UiBar : public UiBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiBar();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UiBar();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 画像の読み込みを行う
	/// </summary>
	void LoadHandle();
	
	/// <summary>
	/// プレイヤーのHPバーを表示
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	/// <param name="maxHp">最大HP</param>
	void DrawPlayerHpBar(Player& player, float maxHp);

	/// <summary>
	/// プレイヤーのゲージバーを表示
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	/// <param name="maxGauge">最大ゲージ量</param>
	void DrawPlayerGaugeBar(Player& player, float maxGauge);

	/// <summary>
	/// 敵のHPバーを表示
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	void DrawEnemyHpBar(EnemyBase& pEnemy);

	/// <summary>
	/// プレイヤーが受けたダメージ量を設定
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void SetPlayerDamage(float damage);

	/// <summary>
	/// 敵が受けたダメージ量を設定
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void SetEnemyDamage(float damage);
	
private:
	float m_playerDamage;		// プレイヤーが受けたダメージ量
	float m_enemyDamage;		// 敵が受けたダメージ量
	int m_playerHpDecreaseTime;	// プレイヤーのHPバーが減少するまでの時間
	int m_enemyHpDecreaseTime;	// 敵のHPバーが減少するまでの時間
};

