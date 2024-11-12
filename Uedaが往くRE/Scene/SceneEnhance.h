#pragma once
#include "SceneBase.h"
#include <string>
#include <map>

class Player;

/// <summary>
/// プレイヤーの強化画面
/// </summary>
class SceneEnhance : public SceneBase
{
public:
	/// <summary>
	/// プレイヤーの強化データ
	/// </summary>
	struct EnhanceData
	{
		std::string id;				// ID
		std::string skillName;		// スキル名
		std::string skillExplain;	// スキル説明
		float upAmount;				// 上昇する倍率
		int requiredMoney;			// 必要金額
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneEnhance(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneEnhance() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

private:
	/// <summary>
	/// 選択中のスキルを強化する
	/// </summary>
	void SetEnhance();

	/// <summary>
	/// 現在選択中のスキル名を表示する
	/// </summary>
	void DrawSelectSkillName();

	/// <summary>
	/// 現在選択中のスキルの強化段階を取得する
	/// </summary>
	int GetSelectSkillStep();

private:
	std::map<std::string, EnhanceData> m_enhanceData;	// 強化データ
};

