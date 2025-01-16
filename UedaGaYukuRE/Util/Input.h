#pragma once
#include <array>
#include <map>
#include<unordered_map>
#include<string>

/// <summary>
/// 入力コマンド名
/// </summary>
namespace InputId
{
	const char* const kA = "A";
	const char* const kB = "B";
	const char* const kX = "X";
	const char* const kY = "Y";
	const char* const kLb = "LB";
	const char* const kRb = "RB";

	const char* const kUp = "UP";
	const char* const kDown = "DOWN";
	const char* const kLeft = "LEFT";
	const char* const kRight = "RIGHT";

	const char* const kOk = "OK";
	const char* const kBack = "BACK";
	const char* const kMenu = "MENU";
	const char* const kSelect = "SELECT";
	const char* const kCancel = "CANCEL";
	const char* const kPause = "PAUSE";

	const char* const kPunch = "punch";
	const char* const kKick = "kick";
	const char* const kAvoid = "avoid";
	const char* const kFighting = "fighting";
	const char* const kGuard = "guard";
	const char* const kGrab = "grab";
	const char* const kSpecial = "special";

	const char* const kRotateL = "rotateL";
	const char* const kRotateR = "rotateR";
	const char* const kRotateU = "rotateU";
	const char* const kRotateD = "rotateD";

	const char* const kDebugDamage = "debug_damage";
	const char* const kDebugSpawn = "debug_spawn";
	const char* const kDebugEnding = "debug_ending";
	const char* const kDebugClear = "debug_clear";
	const char* const kDebugGameover = "debug_gameover";
	const char* const kDebugTutorial = "debug_tutorial";
	const char* const kEnd = "end";
}

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

