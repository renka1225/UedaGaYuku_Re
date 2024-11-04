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
	UiBar(std::shared_ptr<Player> pPlayer):UiBase(pPlayer) {}

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
	/// プレイヤーのHPバーを表示
	/// </summary>
	void DrawPlayerHpBar();

	/// <summary>
	/// プレイヤーのゲージバーを表示
	/// </summary>
	void DrawPlayerGaugeBar();

	/// <summary>
	/// 敵のHPバーを表示
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	void DrawEnemyHpBar(EnemyBase& pEnemy);

private:
	int m_barHandle; // バーの画像
};

