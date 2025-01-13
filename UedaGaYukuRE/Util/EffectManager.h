#pragma once
#include "DxLib.h"
#include <vector>
#include <map>
#include <string>

class Player;
class EnemyBase;

// エフェクト名
namespace EffectName
{
	const std::string kAttack = "attack";
	const std::string kGuard = "guard";
	const std::string kItemDrop = "itemDrop";
}

/// <summary>
/// エフェクトの管理を行う
/// </summary>
class EffectManager
{
public:
	// エフェクトの情報
	struct EffectData
	{
		int effektHandle = -1;	// エフェクトのハンドル
		int playingHandle = -1;	// 再生中のハンドル
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);	// 表示位置
		VECTOR adjPos = VGet(0.0f, 0.0f, 0.0f);	// 表示位置の調整量
		VECTOR rotate = VGet(0.0f, 0.0f, 0.0f);	// 回転率
		float scale = 0.0f;		// 拡大率
		int playTime = 0;		// トータルの再生時間
		int elapsedTime = 0;	// 現在の再生時間
		bool isPlaying = false;	// エフェクトを再生中か
		bool isLoop = false;	// ループ再生するかどうか
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
	void Delete(const std::string& name);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトを追加する
	/// </summary>
	/// <param name="name">追加するエフェクト名</param>
	/// <param name="enemyPos">キャラクターの位置</param>
	/// <param name="adjust">表示位置調整</param>
	void Add(const std::string& name, const VECTOR& pos, float adjust = 0.0f);

	/// <summary>
	/// ドロップアイテムをのエフェクトを停止する
	/// </summary>
	/// <param name="pos">エフェクト位置</param>
	void StopItemEffect(const std::string& name, const VECTOR& pos);

	/// <summary>
	/// エフェクトが再生中か取得する
	/// </summary>
	/// <param name="name">確認したいエフェクト名</param>
	bool GetIsPlaying(const std::string& name) { return m_effectData[name].isPlaying; }

private:
	EffectManager() = default;
	virtual ~EffectManager() = default;
	static EffectManager* m_instance;	 // インスタンス

private:
	std::map<std::string, EffectData> m_effectData; // エフェクトのデータ
	std::vector<EffectData> m_effects; // エフェクトを複数管理する
};