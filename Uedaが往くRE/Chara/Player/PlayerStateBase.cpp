#include "PlayerStateBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pPlayer"></param>
PlayerStateBase::PlayerStateBase(std::shared_ptr<Player> pPlayer):
	m_analogInput({}),
	m_analogX(0),
	m_analogY(0)
{
	m_pPlayer = pPlayer;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <param name="camera">カメラ情報</param>
void PlayerStateBase::Update(const Input& input, const Camera& camera, Stage& stage)
{
	GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // アナログスティックの入力状態
}
