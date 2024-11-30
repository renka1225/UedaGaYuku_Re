#pragma once
#include "SceneBase.h"

/// <summary>
/// ロード中のクラス
/// </summary>
class SceneLoad : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneLoad();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneLoad();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	int m_sSyncLoadNum; // 非同期読み込みの数
};

