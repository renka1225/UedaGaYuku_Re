#pragma once

/// <summary>
/// アイテムの基底クラス
/// </summary>
class ItemBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ItemBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

private:
	int m_possessNum; // 所持しているアイテム数
};

