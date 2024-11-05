#include "DxLib.h"
#include "LoadCsv.h"
#include "ItemBase.h"

ItemBase::ItemBase()
{
	LoadCsv::GetInstance().LoadItemData(m_itemData);
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
	DrawFormatString(300, 400, 0xffffff, "アイテム名:%s", m_itemData["hp_large"].itemName.c_str());
#endif
}
