#pragma once
#include "CharacterBase.h"

class Player;

/// <summary>
/// NPCの管理を行う
/// </summary>
class Npc : public CharacterBase, public std::enable_shared_from_this<Npc>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelHandle">モデルのハンドル</param>
	Npc(int modelHandle);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Npc();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pStage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照</param>
	virtual void Update(Stage& pStage, const Player& pPlayer);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// UIを表示数
	/// </summary>
	void DrawUi();
};

