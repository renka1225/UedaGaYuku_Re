#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	constexpr int kEnemyNamekind = 31;	// 敵名の種類

	// ファイルのパス名
	const std::map<std::string, std::string> kPath
	{
		{"enhance", "data/csv/enhanceData.csv"},	// プレイヤー強化データ
		{"status", "data/csv/charaStatus.csv"},		// キャラクターステータス
		{"col", "data/csv/charaColData.csv"},		// キャラクターの当たり判定データ
		{"anim", "data/csv/animData.csv"},			// アニメーションデータ
		{"weapon", "data/csv/weaponData.csv"},		// 武器データ
		{"item", "data/csv/itemData.csv"},			// アイテムデータ
		{"ui", "data/csv/uiData.csv"},				// UIデータ
		{"enemyName", "data/csv/enemyName.csv"},	// 敵名データ
		{"message", "data/csv/message.csv"},		// メッセージデータ
		{"conversation", "data/csv/conversation.csv"}	// 会話テキストデータ
	};

	// 文字列のIDからItemTypeに変更する
	const std::unordered_map<std::string, Item::ItemType> kStringToItemType =
	{
		{"hp_small", Item::ItemType::kHpSmall},
		{"hp_middle", Item::ItemType::kHpMiddle},
		{"hp_large", Item::ItemType::kHpLarge},
		{"gauge_small", Item::ItemType::kGaugeSmall},
		{"gauge_large", Item::ItemType::kGaugeLarge},
		{"hp_gauge_small", Item::ItemType::kHpGaugeSmall},
		{"ha_gauge_large", Item::ItemType::kHpGaugeLarge},
		{"atk_small", Item::ItemType::kAtkSmall},
		{"atk_large",Item::ItemType::kAtkLarge}
	};

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void LoadCsv::LoadEnhanceData(std::map<std::string, SceneEnhance::EnhanceData>& data)
{
	std::ifstream ifs(kPath.at("enhance"));
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');

		// IDを設定
		std::string id = strvec[0];

		try
		{
			data[id].id = id;
			data[id].skillName = strvec[1];
			data[id].skillExplain = strvec[2];
			data[id].upAmount = std::stof(strvec[3]);
			data[id].requiredMoney = std::stoi(strvec[4]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}
	}
}

void LoadCsv::LoadStatus(CharacterBase::Status& data, std::string charaName)
{
	std::ifstream ifs(kPath.at("status"));
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* charaId = strvec[0].c_str();

		if (charaId == charaName)
		{
			try
			{
				// ステータス情報を代入する
				data.maxHp = std::stof(strvec[1]);
				data.maxGauge = std::stof(strvec[2]);
				data.walkSpeed = std::stof(strvec[3]);
				data.runSpeed = std::stof(strvec[4]);
				data.avoidDist = std::stof(strvec[5]);
				data.atkPowerPunch1 = std::stof(strvec[6]);
				data.atkPowerPunch2 = std::stof(strvec[7]);
				data.atkPowerPunch3 = std::stof(strvec[8]);
				data.atkPowerKick = std::stof(strvec[9]);
				data.atkPowerOneHandWeapon = std::stof(strvec[10]);
				data.atkPowerTwoHandWeapon = std::stof(strvec[11]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}
}

void LoadCsv::LoadColData(CharacterBase::ColData& data, std::string charaName)
{
	std::ifstream ifs(kPath.at("col"));
	std::string line;
	std::vector<std::string> strvec;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		std::string charaId = strvec[0].c_str();

		if (charaId == charaName)
		{
			try
			{
				// 外部ファイルの情報を入れる
				data.bodyStartPos.x = std::stof(strvec[1]);
				data.bodyStartPos.y = std::stof(strvec[2]);
				data.bodyStartPos.z = std::stof(strvec[3]);
				data.bodyEndPos.x = std::stof(strvec[4]);
				data.bodyEndPos.y = std::stof(strvec[5]);
				data.bodyEndPos.z = std::stof(strvec[6]);
				data.bodyRadius = std::stof(strvec[7]);
				data.armRadius = std::stof(strvec[8]);
				data.legRadius = std::stof(strvec[9]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}

			break;
		}
	}
}

void LoadCsv::LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kPath.at("anim"));
	std::string line;
	std::vector<std::string> strvec;

    while (std::getline(ifs, line))
    {
        strvec = split(line, ',');
        // アニメーション情報を設定
        std::string animName = strvec[0];

        try
		{
			data[animName].loopFrame = std::stof(strvec[1]);
			data[animName].playSpeed = std::stof(strvec[2]);
			data[animName].startupFrame = std::stoi(strvec[3]);
			data[animName].activeFrame = std::stoi(strvec[4]);
			data[animName].recoveryFrame = std::stoi(strvec[5]);
        }
        catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
        }
    }
}

void LoadCsv::LoadWeaponData(std::map<std::string, Weapon::WeaponData>& data)
{
	std::ifstream ifs(kPath.at("weapon"));
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		std::string weaponId = strvec[0];

		try
		{
			// 外部ファイルの情報を入れる
			data[weaponId].id = weaponId;
			data[weaponId].name = strvec[1];
			data[weaponId].durability = std::stoi(strvec[2]);
			data[weaponId].colStartPos.x = std::stof(strvec[3]);
			data[weaponId].colStartPos.y = std::stof(strvec[4]);
			data[weaponId].colStartPos.z = std::stof(strvec[5]);
			data[weaponId].colEndPos.x = std::stof(strvec[6]);
			data[weaponId].colEndPos.y = std::stof(strvec[7]);
			data[weaponId].colEndPos.z = std::stof(strvec[8]);
			data[weaponId].colRadius = std::stof(strvec[9]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}
		
	}
}

void LoadCsv::LoadItemData(std::map<Item::ItemType, Item::ItemData>& data)
{
	std::ifstream ifs(kPath.at("item"));
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');

		// アイテム情報を設定
		std::string itemName = strvec[0];

		// アイテムのIDをItemType型に変換
		auto it = kStringToItemType.find(itemName);
		if (it == kStringToItemType.end()) continue;
		
		Item::ItemType itemType = it->second;

		try
		{
			data[itemType].itemName = strvec[1];
			data[itemType].itemExplain = strvec[2];
			data[itemType].recoveryHP = std::stof(strvec[3]);
			data[itemType].recoveryGauge = std::stof(strvec[4]);
			data[itemType].atkUp = std::stof(strvec[5]);
			data[itemType].time = std::stoi(strvec[6]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}
	}
}

void LoadCsv::LoadUiData(std::map<std::string, UiBase::UiData>& data)
{
	std::ifstream ifs(kPath.at("ui"));
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');

		std::string uiId = strvec[0];
		try
		{
			data[uiId].LTposX = std::stof(strvec[1]);
			data[uiId].LTposY = std::stof(strvec[2]);
			data[uiId].RBposX = std::stof(strvec[3]);
			data[uiId].RBposY = std::stof(strvec[4]);
			data[uiId].width = std::stof(strvec[5]);
			data[uiId].height = std::stof(strvec[6]);
			data[uiId].scale = std::stof(strvec[7]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}

		m_uiData[strvec.at(0)] = data[uiId];
	}
}

void LoadCsv::LoadMessage()
{
	std::ifstream ifs(kPath.at("message"));
	std::string line;
	m_messageData.clear();

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		m_messageData[strvec.at(0)] = strvec.at(1);
	}
}

void LoadCsv::LoadConversation()
{
	std::ifstream ifs(kPath.at("conversation"));
	std::string line;
	m_conversationData.clear();

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		m_conversationData[strvec.at(0)] = { strvec.at(1), strvec.at(2) };
	}
}

void LoadCsv::LoadEnemyName()
{
	std::ifstream ifs(kPath.at("enemyName"));
	std::string line;
	std::vector<std::string> strvec;

	for (int i = 0; i < kEnemyNamekind; i++)
	{
		while (std::getline(ifs, line))
		{
			std::vector<std::string> strvec = split(line, ',');
			if (!strvec.empty())
			{
				m_enemyNameData.push_back(strvec[i]);
			}
		}
	}
}

UiBase::UiData LoadCsv::GetUiData(std::string id)
{
	return m_uiData[id];
}

std::string LoadCsv::Get_sMessage(std::string id)
{
	return m_messageData[id];
}

const char* LoadCsv::Get_cMessage(std::string id)
{
	return m_messageData[id].c_str();
}

const char* LoadCsv::GetConversationName(std::string id)
{
	return m_conversationData[id].name.c_str();
}

const char* LoadCsv::GetConversationText(std::string id)
{
	return m_conversationData[id].text.c_str();
}

std::string LoadCsv::GetEnemyName(int enemyIndex)
{
	return m_enemyNameData[enemyIndex];
}
