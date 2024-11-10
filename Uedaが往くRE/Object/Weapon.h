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
		int durability;		// 耐久力
		VECTOR colStartPos; // 武器の当たり判定始点
		VECTOR colEndPos;	// 武器の当たり判定終点
		float colRadius;	// 武器の当たり判定半径
	};

	// 当たり判定更新データ
	struct UpdateColData
	{
		VECTOR colStartPos; // 武器の当たり判定始点
		VECTOR colEndPos;	// 武器の当たり判定終点
		float colRadius;	// 武器の当たり判定半径
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
	/// 武器の耐久力を減らす
	/// </summary>
	void DecrementDurability();

	/// <summary>
	/// 武器の当たり判定をチェックする
	/// </summary>
	/// <param name="colData">キャラクターの当たり判定情報</param>
	/// <param name="player">プレイヤー情報参照</param>
	/// <returns>当たっているかどうか</returns>
	bool CheckWeaopnCol(const CharacterBase::ColData& colData, Player& player);

	/// <summary>
	/// 攻撃中の武器当たり状態をセットする
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHitAttack(bool isHit) { m_isHitAttack = isHit; }

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
	/// <param name="modelHandle">キャラクターの3Dモデル</param>
	/// <param name="frameName">セットするフレーム名</param>
	/// <param name="setModelHandle">セットする武器のモデル</param>
	/// <param name="setModelHandle">配置データ</param>
	/// <param name="frameMatrix">モデルの行列</param>
	void SetModelFramePos(int modelHandle, const char* frameName, int setModelHandle, auto& loc, MATRIX frameMatrix);

protected:
	// 配置情報データ
	struct LocationData
	{
		std::string name;	// 名前
		std::string tag;	// タグ
		VECTOR pos;			// 座標
		VECTOR rot;			// 回転
		VECTOR scale;		// スケール
		VECTOR initPos;		// 初期位置
		VECTOR initRot;		// 初期回転量
	};
	std::vector<LocationData> m_locationData;

protected:
	std::shared_ptr<Player> m_pPlayer; // プレイヤーのポインタ
	std::unordered_map<std::string, int> m_objHandle; // 読み込むオブジェクトのハンドル
	WeaponData m_weaponData;	// 武器のデータ
	UpdateColData m_updateCol;	// 更新後の当たり判定データ
	int m_locationDataHandle;	// 読み込む配置データ
	int m_durability;			// 武器の耐久力
	bool m_isHitAttack;			// 攻撃中に当たっているかどうか
	int m_handle;				// 画像ハンドル
};