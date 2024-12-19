#pragma once
#include <map>
#include <string>

/// <summary>
/// イベント用のフラグを管理する
/// </summary>
class EventData
{
public:
	// イベント用の当たり判定データ
	struct EventColData
	{
		std::string eventId; // イベントID名
		VECTOR pos;		// 当たり判定位置
		float radius;	// 当たり判定半径
	};

public:
	EventData();
	~EventData();
	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// イベント情報を取得する
	/// </summary>
	/// <returns></returns>
	const std::vector<EventColData>& GetEventData() const { return m_eventData; }

private:
	/// <summary>
	/// イベント判定のデータを読み込む
	/// </summary>
	void Load();

private:
	std::vector<EventColData> m_eventData; // イベントデータの情報
};

