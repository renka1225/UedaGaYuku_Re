#pragma once
#include "CharacterBase.h"

/// <summary>
/// セレクトシーンで使用するプレイヤー
/// </summary>
class PlayerSelect : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 表示位置を更新
	/// </summary>
	/// <param name="select">現在の選択状態</param>
	void ChangePos(int select);

private:
	std::string m_playAnimName; // 再生するアニメーション名
};

