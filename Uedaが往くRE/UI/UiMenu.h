#pragma once
#include "UiBase.h"

/// <summary>
/// メニュー画面のUI
/// </summary>
class UiMenu : public UiBase
{
public:
	UiMenu(std::shared_ptr<Player> pPlayer);
	virtual ~UiMenu();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

private:
	/// <summary>
	/// 所持金額を表示
	/// </summary>
	void DrawMoney();

private:
	// 仮ハンドル
	int m_moneyBg;
	int m_bgHandle;
};

