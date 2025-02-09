﻿#pragma once
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "SceneEnhance.h"
#include "Weapon.h"
#include "Item.h"
#include "UiBase.h"
#include <string>
#include <map>

/// <summary>
/// シングルトンを使用してCSVファイル読み込みを行う
/// </summary>
class LoadCsv
{
public:
	// コピーコンストラクタの禁止
	LoadCsv(const LoadCsv&) = delete;
	// ムーブコンストラクタの禁止
	LoadCsv(LoadCsv&&) = delete;
	// 代入演算子の禁止
	LoadCsv& operator= (const LoadCsv&) = delete;
	LoadCsv& operator= (LoadCsv&&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	/// <summary>
	/// プレイヤーの強化データを読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadEnhanceData(std::map<std::string, SceneEnhance::EnhanceData>& data);

	/// <summary>
	/// キャラクターのステータス情報を読み込む
	/// </summary>
	/// <param name="data">ステータス情報</param>
	/// <param name="charaName">キャラクター名</param>
	void LoadStatus(CharacterBase::Status& data, std::string charaName);

	/// <summary>
	/// 当たり判定のデータを読み込む
	/// </summary>
	/// <param name="data">当たり判定情報</param>
	/// <param name="charType">キャラクター名</param>
	void LoadColData(CharacterBase::ColData& data, std::string charaName);

	/// <summary>
	/// アニメーション情報を読み込む
	/// </summary>
	/// <param name="data">アニメーション情報</param>
	/// <param name="charaName">キャラクター名</param>
	void LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data);

	/// <summary>
	/// 武器情報を読み込む
	/// </summary>
	/// <param name="data">武器情報</param>
	/// <param name="weaponName">武器名</param>
	void LoadWeaponData(std::map<std::string, Weapon::WeaponData>& data);

	/// <summary>
	/// アイテム情報を読み込む
	/// </summary>
	void LoadItemData(std::map<Item::ItemType, Item::ItemData>& data);

	/// <summary>
	/// UI表示情報を読み込む
	/// </summary>
	/// <param name="data">UI情報</param>
	void LoadUiData(std::map<std::string, UiBase::UiData>& data);

	/// <summary>
	/// メッセージを読み込む
	/// </summary>
	void LoadMessage();

	/// <summary>
	/// 会話テキストを読み込む
	/// </summary>
	void LoadConversation();

	/// <summary>
	/// 敵の名前情報を読み込む
	/// </summary>
	void LoadEnemyName();

	/// <summary>
	/// UI情報を取得する
	/// </summary>
	/// <param name="id">取得したいUI情報のID</param>
	/// <returns>UI情報</returns>
	UiBase::UiData GetUiData(std::string id);

	/// <summary>
	/// string型のメッセージの取得
	/// </summary>
	/// <param name="id">取得したいメッセージのID</param>
	/// <returns>メッセージ</returns>
	std::string Get_sMessage(std::string id);

	/// <summary>
	/// char型のメッセージの取得
	/// </summary>
	/// <param name="id">取得したいメッセージのID</param>
	/// <returns>メッセージ</returns>
	const char* Get_cMessage(std::string id);

	/// <summary>
	/// 会話中の人物名を取得する
	/// </summary>
	/// <param name="id">取得したいID</param>
	/// <returns>名前</returns>
	const char* GetConversationName(std::string id);

	/// <summary>
	/// 会話テキストを取得
	/// </summary>
	/// <param name="id">取得したいID</param>
	/// <returns>テキスト</returns>
	const char* GetConversationText(std::string id);

	/// <summary>
	/// 敵名を取得する
	/// </summary>
	/// <param name="enemyIndex">取得する敵の番号</param>
	/// <returns>敵の名前</returns>
	std::string GetEnemyName(int enemyIndex);

private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;
	static LoadCsv* m_instance;	 // インスタンス

private:
	// 会話情報
	struct Talk
	{
		std::string name = "";	// 会話している人の名前
		std::string text = "";	// テキスト
	};

	std::vector<std::string> m_enemyNameData;			// 敵名のデータ
	std::map<std::string, UiBase::UiData> m_uiData;		// UI情報のデータ
	std::map<std::string, std::string> m_messageData;	// メッセージのデータ
	std::map<std::string, Talk> m_conversationData;		// 会話のデータ
};

