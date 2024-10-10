#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <string>
#include <vector>
#include <map>

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class CharacterBase : public ObjectBase
{
public:
	// キャラクターの種類
	enum CharaType
	{
		kPlayer,	// プレイヤー
		kEnemy_01,	// 敵
	};

	// キャラクターのステータス
	struct Status
	{
		float maxHp;				// 最大HP
		float walkSpeed;			// 歩き速度
		float runSpeed;				// 走り速度
	};

	// 当たり判定情報
	struct ColData
	{
		VECTOR bodyStartPos; // 全身の当たり判定始点
		VECTOR bodyEndPos;	 // 全身の当たり判定終点
		VECTOR armStartPos;	 // 腕の当たり判定始点
		VECTOR armEndPos;	 // 腕の当たり判定終点
		VECTOR legStartPos;	 // 脚の当たり判定始点
		VECTOR legEndPos;	 // 脚の当たり判定終点
		float bodyRadius;	 // 全身の当たり判定の半径
		float aimRadius;	 // 腕の当たり判定の半径
		float legRadius;	 // 脚の当たり判定の半径
	};

	// 当たり判定更新データ
	struct UpdateColData
	{
		VECTOR bodyStartPos;	// 全身の当たり判定始点
		VECTOR bodyEndPos;		// 全身の当たり判定終点
		VECTOR armStartPos;		// 腕の当たり判定始点
		VECTOR armEndPos;		// 腕の当たり判定終点
		VECTOR legStartPos;		// 脚の当たり判定始点
		VECTOR legEndPos;		// 脚の当たり判定終点
	};

	// アニメーション情報
	struct AnimInfo
	{
		int number;			// アニメーション番号
		float loopFrame;	// 開始フレーム
		float endFrame;		// 終了フレーム
		float playSpeed;	// アニメーションの速度
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// キャラクター同士の当たり判定をチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="eCapPosTop">当たり判定カプセルの始点</param>
	/// <param name="eCapPosBottom">当たり判定カプセルの終点</param>
	/// <param name="eCapRadius">当たり判定カプセルの半径</param>
	void CheckCharaCol(ObjectBase& obj, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius);

	/// <summary>
	/// アニメーションを変更
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	void ChangeAnim(std::string animName);

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void UpdateAnim();

	/// <summary>
	/// キャラクターのステータス取得
	/// </summary>
	Status GetStatus() const { return m_status; }

protected:
	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void UpdateCol();

protected:
	std::map<std::string, AnimInfo> m_animData;		// アニメーションのデータ
	ColData m_colData;			// 初期当たり判定
	UpdateColData m_updateCol;  // 更新した当たり判定
	Status m_status;			// ステータス
	float m_angle;			    // 向いている方向の角度
	float m_hp;				    // HP

	int m_currentPlayAnim;		// 現在のアニメーション
	int m_prevPlayAnim;			// 前に再生していたアニメーション
	float m_animBlendRate;		// 現在と過去のアニメーションのブレンド率
	float m_currentAnimTime;	// 現在のアニメーション再生時間
	float m_prevAnimTime;		// 前のアニメーション再生時間
	float m_totalAnimTime;		// アニメーションの総再生時間
	float m_animPlaySpeed; 		// アニメーションの再生速度
	float m_animLoopStartTime;	// アニメーションがループが始まる時間
	float m_animLoopEndTime;	// アニメーションがループが終わる時間
	bool m_isLoopAnim;			// アニメーションをループさせるか
};

