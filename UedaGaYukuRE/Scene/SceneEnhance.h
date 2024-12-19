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
	virtual ~SceneEnhance();

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
	/// 円の表示状態を更新する
	/// </summary>
	void UpdateSkillCircle();

	/// <summary>
	/// 強化状態を円で表示する
	/// </summary>
	void DrawSkillCircle();

	/// <summary>
	/// スキル情報を表示する
	/// </summary>
	void DrawSkillInfo();

	/// <summary>
	/// 現在選択中のスキル情報を表示する
	/// </summary>
	void DrawSelectSkill();

	/// <summary>
	/// 現在選択中のスキルの強化段階を取得する
	/// </summary>
	int GetSelectSkillStep();

	/// <summary>
	/// スキルIDを取得する
	/// </summary>
	/// <param name="select">選択番号</param>
	/// <param name="nowStep">現在の強化段階</param>
	/// <returns>スキルID</returns>
	std::string GetSkillId(int select, int nowStep);

	/// <summary>
	/// スキル名を取得する
	/// </summary>
	/// <param name="skillId">スキルID</param>
	/// <returns>スキル名</returns>
	std::string GetSkillName(std::string skillId);

	/// <summary>
	/// スキル詳細を取得する
	/// </summary>
	/// <param name="skillId">スキルID</param>
	/// <returns>スキル詳細</returns>
	std::string GetSkillExplain(std::string skillId);

	/// <summary>
	/// スキルアップに必要な金額を取得する
	/// </summary>
	/// <param name="skillId">スキルID</param>
	/// <returns>必要金額</returns>
	int GetSkillRequiredMoney(std::string skillId);

private:
	std::map<std::string, EnhanceData> m_enhanceData;	// 強化データ
	int m_circleBlinkTime;	// 円の点滅時間
	int m_circleAlpha;		// 円のアルファ値
};

