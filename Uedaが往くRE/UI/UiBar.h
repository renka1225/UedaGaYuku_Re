#pragma once
#include "UiBase.h"
#include <memory>
#include <vector>

class EnemyBase;

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
	/// <param name="currentHp">現在のHP</param>
	/// <param name="maxHp">最大HP</param>
	void DrawPlayerHpBar(float currentHp, float maxHp);

	/// <summary>
	/// プレイヤーのゲージバーを表示
	/// </summary>
	/// <param name="currentGauge">現在のゲージ量</param>
	/// <param name="maxGauge">最大ゲージ量</param>
	void DrawPlayerGaugeBar(float currentGauge, float maxGauge);

	/// <summary>
	/// 敵のHPバーを表示
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	void DrawEnemyHpBar(EnemyBase& pEnemy);

	/// <summary>
	/// ダメージを受けた際にタイマーをセットする
	/// </summary>
	void SetDamageTimer();

	/// <summary>
	/// ダメージ量を設定
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void SetDamage(float damage);
	
private:
	float m_damage;			// ダメージ量
	int m_hpDecreaseTime;	// HPバーが減少するまでの時間
};

