#pragma once

/// <summary>
/// ゲーム画面の定数
/// </summary>
namespace Game
{
	constexpr int kScreenWidth = 1920;	// 横幅
	constexpr int kScreenHeight = 1080;	// 縦幅
	constexpr int kColorDepth = 32;
}

/// <summary>
/// テキスト色の定数
/// </summary>
namespace Color
{
	constexpr int kColorW = 0xffffff;	// 白色
	constexpr int kColorBK = 0x000000;	// 黒色
	constexpr int kColorR = 0xff0000;	// 赤色
	constexpr int kColorG = 0x00ff00;	// 緑色
	constexpr int kColorB = 0x0000ff;	// 青色
	constexpr int kColorY = 0xffff00;	// 黄色
	constexpr int kColorLB = 0x00ffff;	// 水色
	constexpr int kColorP = 0xff00ff;	// 紫色
}

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
	const char* const kPause = "PAUSE";
	const char* const kSelect = "SELECT";
	const char* const kCancel = "CANCEL";

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
	const char* const kEnd = "end";
}

/// <summary>
/// アニメーション名
/// </summary>
namespace AnimName
{
	const char* const kIdleStand = "idle_stand";
	const char* const kIdleFight = "idle_fight";
	const char* const kKick = "kick";
	const char* const kKickRound = "kick_round";
	const char* const kKickHeat = "kick_heat";
	const char* const kPunch = "punch";
	const char* const kPunchStrong = "punch_strong";
	const char* const kRun = "run";
	const char* const kWalk = "walk";
	const char* const kWalkFight = "walk_fight";
	const char* const kAvoid = "avoid";
	const char* const kGuard = "guard";
	const char* const kDamage = "damage";
	const char* const kDown = "down";
	const char* const kGrabEnemy = "grab_enemy";
	const char* const kGrabOneHandWeapon = "grab_oneHandWeapon";
	const char* const kGrabTwoHandWeapon= "grab_twoHandWeapon";
}