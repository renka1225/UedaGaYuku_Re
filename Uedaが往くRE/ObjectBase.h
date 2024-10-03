#pragma once

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

protected:
	VECTOR m_pos;	 // 現在位置
	float m_gravity; // 重力
};

