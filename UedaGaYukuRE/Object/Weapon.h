#pragma once
#include "ObjectBase.h"
#include "CharacterBase.h"

class Player;
class Stage;
class CharacterBase;

/// <summary>
/// 武器クラス
/// </summary>
class Weapon : public ObjectBase
{
public:
	// 武器の情報
	struct WeaponData
	{
		std::string id = "";	// 武器ID
		std::string name = "";	// 武器名
		int durability = 0;		// 耐久力
		VECTOR colStartPos = VGet(0.0f ,0.0f, 0.0f); // 武器の当たり判定始点
		VECTOR colEndPos = VGet(0.0f, 0.0f, 0.0f);	 // 武器の当たり判定終点
		float colRadius = 0.0f;						 // 武器の当たり判定半径
	};

	// 当たり判定更新データ
	struct UpdateColData
	{
		VECTOR colStartPos = VGet(0.0f, 0.0f, 0.0f); // 武器の当たり判定始点
		VECTOR colEndPos = VGet(0.0f, 0.0f, 0.0f);	// 武器の当たり判定終点
		float colRadius = 0.0f;	// 武器の当たり判定半径
	};

private:
	// 配置情報データ
	struct LocationData
	{
		std::string id = "";	// 武器ID
		std::string name = "";	// 名前
		std::string tag = "";	// タグ
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);		// 座標
		VECTOR rot = VGet(0.0f, 0.0f, 0.0f);		// 回転
		VECTOR scale = VGet(0.0f, 0.0f, 0.0f);		// スケール
		VECTOR initPos = VGet(0.0f, 0.0f, 0.0f);	// 初期位置
		VECTOR initRot = VGet(0.0f, 0.0f, 0.0f);	// 初期回転量
		UpdateColData updateCol;	// 更新後の当たり判定データ
		int durability = 0;			// 武器の耐久力
		bool isGrab = false;		// 武器が掴まれた状態か
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	Weapon(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Weapon();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(Stage& stage);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 武器UI描画
	/// </summary>
	void DrawWeaponUi();

	/// <summary>
	/// 武器の耐久力を減らす
	/// </summary>
	void DecrementDurability();

	/// <summary>
	/// 武器の当たり判定をチェックする
	/// </summary>
	/// <param name="colData">キャラクターの当たり判定情報</param>
	/// <param name="player">プレイヤー情報参照</param>
	/// <returns>当たっているかどうか</returns>
	bool CheckWeaponCol(const CharacterBase::ColData& colData, Player& player);

	/// <summary>
	/// 攻撃中の武器当たり状態をセットする
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHitAttack(bool isHit) { m_isHitAttack = isHit; }

	/// <summary>
	/// 武器の掴み状態を更新
	/// </summary>
	/// <param name="isGrab">掴み状態</param>
	void UpdateIsGrab(bool isGrab);

	/// <summary>
	/// プレイヤーの近くにある武器のタグ名を取得する
	/// </summary>
	std::string GetNearWeaponTag() const;

private:
	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadLocationData();

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void UpdateCol(auto& loc);

	/// <summary>
	/// 武器を持った際に位置を更新する
	/// </summary>
	/// <param name="loc">配置データ</param>
	/// <param name="frameMatrix">モデルの行列</param>
	void SetModelFramePos(auto& loc, MATRIX frameMatrix);

protected:
	std::vector<LocationData> m_locationData;		// 配置データ
	std::shared_ptr<Player> m_pPlayer;				// プレイヤーのポインタ
	std::map<std::string, int> m_objHandle;			// 読み込むオブジェクトのハンドル
	std::map<std::string, WeaponData> m_weaponData; // 武器のデータ
	int m_loadLocationData;	// 読み込む配置データ
	bool m_isHitAttack;		// 攻撃中に当たっているかどうか
	int m_handle;			// 画像ハンドル
};