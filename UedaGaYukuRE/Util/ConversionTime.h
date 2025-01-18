#pragma once

// フレーム数からタイムに変換する
namespace Conversion
{
	/// <summary>
	/// 時間に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>時間</returns>
	int ChangeHour(int frame);

	/// <summary>
	/// 分に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>分</returns>
	int ChangeMin(int frame);

	/// <summary>
	/// 秒に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>秒</returns>
	int ChangeSec(int frame);

	/// <summary>
	/// ミリ秒に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>ミリ秒</returns>
	int ChangeMilliSec(int frame);
}