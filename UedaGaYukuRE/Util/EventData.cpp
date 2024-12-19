#include "DebugDraw.h"
#include "DxLib.h"
#include "EventData.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	const char* kCsvFilePath = "data/csv/eventData.csv";	// 読み込むcsvファイルのパス

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

EventData::EventData()
{
	Load();
}

EventData::~EventData()
{
}

void EventData::Init()
{
}

void EventData::Update()
{
}

void EventData::Draw()
{
#ifdef _DEBUG
	DebugDraw debug;
	// イベント用の当たり判定を表示
	for (const auto& col : m_eventData)
	{
		debug.DrawEventCol(col.pos, col.radius);
	}
#endif
}

void EventData::Load()
{
	std::ifstream ifs(kCsvFilePath);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');

		try
		{
			EventColData data;
			data.eventId = strvec[0];
			data.pos = VGet(std::stof(strvec[1]), std::stof(strvec[2]), std::stof(strvec[3]));
			data.radius = std::stof(strvec[4]);

			m_eventData.push_back(data);
		}
		catch (const std::invalid_argument&)
		{
		}
	}
}
