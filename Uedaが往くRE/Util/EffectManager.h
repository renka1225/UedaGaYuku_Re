#pragma once
#include "DxLib.h"
#include <vector>
#include <map>
#include <string>

class Player;
class EnemyBase;

/// <summary>
/// エフェクトの管理を行う
/// </summary>
class EffectManager
{
public:
	// エフェクトの情報
	struct EffectData
	{
		int handle;			// エフェクトのハンドル
		VECTOR pos;			// 表示位置
		float scale;		// 拡大率
		int playTime;		// トータルの再生時間
		int elapsedTime;	// 現在の再生時間
	};

	// コピーコンストラクタの禁止
	EffectManager(const EffectManager&) = delete;
	// ムーブコンストラクタの禁止
	EffectManager(EffectManager&&) = delete;
	// 代入演算子の禁止
	EffectManager& operator= (const EffectManager&) = delete;
	EffectManager& operator= (EffectManager&&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static EffectManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new EffectManager;
		}
		return *m_instance;
	}

	/// <summary>
	/// エフェクトハンドルを読み込む
	/// </summary>
	void Load();

	/// <summary>
	/// 画面上のエフェクトを削除する
	/// </summary>
	void Delete();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクトの表示位置を更新する
	/// </summary>
	/// <param name="effectName">エフェクト名</param>
	void UpdatePos(std::string effectName);

	/// <summary>
	/// 拡大率を更新する
	/// </summary>
	/// <param name="effectName">エフェクト名</param>
	void UpdateScale(std::string effectName);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトを追加する
	/// </summary>
	/// <param name="name">追加するエフェクト名</param>
	/// <param name="enemyPos">敵座標</param>
	void Add(const std::string name, const VECTOR enemyPos);

private:
	EffectManager() = default;
	virtual ~EffectManager() = default;
	static EffectManager* m_instance;	 // インスタンス

private:
	std::map<std::string, EffectData> m_effectData; // エフェクトのデータ
};