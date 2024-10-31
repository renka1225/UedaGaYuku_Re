#include "DxLib.h"
#include "ItemBase.h"

ItemBase::ItemBase():
	m_possessNum(0)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
}

void ItemBase::Update()
{
}

void ItemBase::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 400, 0xffffff, "現在のアイテム数:%d", m_possessNum);
#endif
}
