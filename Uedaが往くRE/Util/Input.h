#pragma once
#include <array>
#include <map>
#include<unordered_map>
#include<string>

/// <summary>
/// 入力管理クラス
/// </summary>

enum class InputType
{
	keybd,	// キーボード
	pad,	// パッド
	mouse	// マウス
};

// コマンドの種類
using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class Input
{
public:
	/// <summary>
	/// 入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 押された瞬間を取得
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>ture:押された瞬間</returns>
	bool IsTriggered(const char* command)const;

	/// <summary>
	/// 今押しているかを取得
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>ture:押されている</returns>
	bool IsPressing(const char* command)const;

	/// <summary>
	/// 離された瞬間を取得
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:離された瞬間</returns>
	bool IsReleased(const char* command)const;

	/// <summary>
	/// コマンドの種類
	/// </summary>
	Input();

private:
	// コマンド名と入力をペアにしたテーブル
	InputTable_t m_commandTable;

	// コマンドの入力を覚えておく
	std::map<std::string, bool> m_inputData;		// 現在の入力
	std::map<std::string, bool> m_lastInputData;	// 直前の入力
};

