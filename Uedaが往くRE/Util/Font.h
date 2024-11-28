#pragma once
#include <vector>

// フォント関連
namespace Font
{
	// フォントの種類
	enum class FontId
	{
		// フォントサイズ、太さ
		kSize96_4,
		kSize64_4,
		kSize50_4,
		kSize50_4_EDGE,
		kSize48_4,
		kSize48_4_EDGE,
		kSize42_4,
		kSize42_4_EDGE,
		kSize40_4,
		kSize36_4,
		kSize36_4_EDGE,
		kSize34_4_EDGE,
		kSize30_4_EDGE,
		kSize28_4,
		kSize24_4,
		kSize24_4_EDGE,
		kSize16_4,
		kDebug,
		kDebug_EDGE,
		kNum,		// フォントの種類数

		/*使うフォントを指定する*/

		// 選択画面
		kSelect_explain = kSize42_4,	// 選択テキストの説明

		// メニュー画面
		kMenu_itemName = kSize40_4,		// アイテム名
		kMenu_itemExplain = kSize42_4,	// アイテムの説明

		// 強化画面
		kEnhance_skillMoney = kSize36_4,	// 必要金額
		kEnhance_skillName = kSize40_4,		// スキル名
		kEnhance_skillExplain = kSize42_4,	// スキルの詳細

		// オプション画面
		kOption_soundText = kSize36_4,		// サウンドのテキスト
		kOption_soundVol = kSize36_4,		// 現在の音量
		kOption_window = kSize36_4,			// 画面サイズのテキスト

		// デバッグ
		kDebug_animFrame = kDebug_EDGE,		// アニメーションのフレーム数
	};

	// フォントのロード、アンロード
	void Load();
	void UnLoad();

	// フォントのハンドル
	// MEMO:externを使用して異なるソースファイルで変数を共有できるようにする
	extern std::vector<int> m_fontHandle;
}

