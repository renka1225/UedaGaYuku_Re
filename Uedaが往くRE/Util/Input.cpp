#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include <string>
#include <algorithm>

void Input::Update()
{
	// 直前の入力をコピー
	m_lastInputData = m_inputData;

	/*ハードウェア入力チェック*/
	// パッド情報の取得
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);
	// 現在のキーボード入力を取得
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// m_inputDataの内容を更新
	for (const auto& cmd : m_commandTable)
	{
		auto& input = m_inputData[cmd.first];	// コマンドの名前から入力データを作る
		for (const auto& hardIO : cmd.second)
		{
			// キーボードのチェック
			input = false;
			if (hardIO.first == InputType::keybd)
			{
				if (keystate[hardIO.second])
				{
					input = true;
					break;
				}
			}
			// パッドのチェック
			else if (hardIO.first == InputType::pad)
			{
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}

bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end())
	{
		return false;
	}

	return(m_inputData.at(command) && !m_lastInputData.at(command));
}

bool Input::IsPressing(const char* command) const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end()	)
	{
		return false;
	}

	return m_inputData.at(command);
}

bool Input::IsReleased(const char* command) const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end())
	{
		return false;
	}

	return(!m_inputData.at(command) && m_lastInputData.at(command));
}

Input::Input()
{
	m_commandTable[InputId::kOk] = { {InputType::keybd, KEY_INPUT_RETURN},
							 {InputType::pad, PAD_INPUT_A} };

	m_commandTable[InputId::kBack] = { {InputType::keybd, KEY_INPUT_Z},
							   {InputType::pad, PAD_INPUT_B} };

	m_commandTable[InputId::kA] = { {InputType::keybd, KEY_INPUT_RETURN},
							{InputType::pad, PAD_INPUT_A} };

	m_commandTable[InputId::kB] = { {InputType::keybd, KEY_INPUT_B},
							{InputType::pad, PAD_INPUT_B} };

	m_commandTable[InputId::kX] = { {InputType::keybd, KEY_INPUT_X},
							{InputType::pad, PAD_INPUT_C} };

	m_commandTable[InputId::kY] = { {InputType::keybd, KEY_INPUT_Y},
							{InputType::pad, PAD_INPUT_X} };

	m_commandTable[InputId::kRb] = { { InputType::keybd, KEY_INPUT_P },
									{InputType::pad, PAD_INPUT_Z } },

	m_commandTable[InputId::kLb] = { { InputType::keybd, KEY_INPUT_Q },
								{InputType::pad, PAD_INPUT_Y } },

	m_commandTable[InputId::kUp] = { {InputType::keybd, KEY_INPUT_UP},
							 {InputType::pad, PAD_INPUT_UP} };

	m_commandTable[InputId::kDown] = { {InputType::keybd, KEY_INPUT_DOWN},
							   {InputType::pad, PAD_INPUT_DOWN} };
	
	m_commandTable[InputId::kLeft] = { {InputType::keybd, KEY_INPUT_LEFT},
							   {InputType::pad, PAD_INPUT_LEFT} };

	m_commandTable[InputId::kRight] = { {InputType::keybd, KEY_INPUT_RIGHT},
							    {InputType::pad, PAD_INPUT_RIGHT} };

	m_commandTable[InputId::kMenu] = { {InputType::keybd, KEY_INPUT_TAB},
								{InputType::pad, PAD_INPUT_R} };

	/*攻撃*/
	m_commandTable[InputId::kPunch] = { {InputType::keybd, KEY_INPUT_Z},
								{InputType::pad, PAD_INPUT_C} };
	m_commandTable[InputId::kKick] = { {InputType::keybd, KEY_INPUT_X},
							    {InputType::pad, PAD_INPUT_X} };
	m_commandTable[InputId::kAvoid] = { { InputType::keybd, KEY_INPUT_C },
								{InputType::pad, PAD_INPUT_A } },
	m_commandTable[InputId::kFighting] = { { InputType::keybd, KEY_INPUT_SPACE },
								{InputType::pad, PAD_INPUT_Z } },
	m_commandTable[InputId::kGuard] = { { InputType::keybd, KEY_INPUT_Q },
								{InputType::pad, PAD_INPUT_Y } },
	m_commandTable[InputId::kGrab] = { { InputType::keybd, KEY_INPUT_L },
								{InputType::pad, PAD_INPUT_B } },
	m_commandTable[InputId::kSpecial] = { { InputType::keybd, KEY_INPUT_W },
								{InputType::pad, PAD_INPUT_B } },

	/*カメラ回転*/
	// 左
	m_commandTable[InputId::kRotateL] = { {InputType::keybd, KEY_INPUT_A},
								{InputType::pad, PAD_INPUT_Y} };
	// 右
	m_commandTable[InputId::kRotateR] = { {InputType::keybd, KEY_INPUT_D},
								{InputType::pad, PAD_INPUT_Z} };
	// 上
	m_commandTable[InputId::kRotateU] = { {InputType::keybd, KEY_INPUT_W},
							    {InputType::pad, PAD_INPUT_10} };
	// 下
	m_commandTable[InputId::kRotateD] = { {InputType::keybd, KEY_INPUT_S},
								{InputType::pad, PAD_INPUT_10} };

	/*デバッグ機能*/
	m_commandTable[InputId::kDebugDamage] = { {InputType::keybd, KEY_INPUT_SPACE},
								{InputType::pad, PAD_INPUT_11} };
	m_commandTable[InputId::kDebugSpawn] = { {InputType::keybd, KEY_INPUT_LSHIFT},
								{InputType::pad, PAD_INPUT_Z} };
	m_commandTable[InputId::kDebugEnding] = { {InputType::keybd, KEY_INPUT_0},
							{InputType::pad, PAD_INPUT_11} };
	m_commandTable["debug_pause"] = { {InputType::keybd, KEY_INPUT_P},
								{InputType::pad, PAD_INPUT_11} };
	m_commandTable["debug_enter"] = { {InputType::keybd, KEY_INPUT_RETURN},
								{InputType::pad, PAD_INPUT_11} };
	m_commandTable[InputId::kEnd] = { {InputType::keybd, KEY_INPUT_ESCAPE},
								{InputType::pad, PAD_INPUT_10} };
}