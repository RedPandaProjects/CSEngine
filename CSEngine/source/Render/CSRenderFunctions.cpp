#include "CSEngine.hpp"
static BearColor LSetColor;
static HIMAGE LHImage = 0;
static void	PICSetExports(HIMAGE hPic, int r, int g, int b, int a)
{
	LHImage = hPic;
	LSetColor.SetAsUint8(static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b), static_cast<uint8>(a));
}
static void	PICDrawExports(int x, int y, int width, int height, const wrect_t* prc)
{
	CSTexture2D* Texture = GResourcesManager->GetTexture(LHImage);
	BearVector4<float> Rect;
	Rect.set(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
	BearVector4<float> UV;
	if (prc)
	{
		UV.set(static_cast<float>(prc->left) / static_cast<float>(Texture->GetSize().x), static_cast<float>(prc->top) / static_cast<float>(Texture->GetSize().y), static_cast<float>(prc->right) / static_cast<float>(Texture->GetSize().x), static_cast<float>(prc->bottom) / static_cast<float>(Texture->GetSize().y));

	}
	else
	{
		UV.set(0, 0, 1, 1);
	}
	GRender->PushUIRender(Rect,UV, LSetColor, LHImage);
}
static void DrawCharacterExport(int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont)
{
	float	row, col, size;
	float	s1, t1, s2, t2;

	ch &= 255;

	if (ch == ' ') return;
	if (y < -height) return;


	col = (ch & 15) * 0.0625f + (0.5f / 256.0f);
	row = (ch >> 4) * 0.0625f + (0.5f / 256.0f);
	size = 0.0625f - (1.0f / 256.0f);

	s1 = col;
	t1 = row;
	s2 = s1 + size;
	t2 = t1 + size;
	LSetColor.SetAsUint32(static_cast<uint32>(ulRGBA));
	bear_swap(LSetColor.R32F, LSetColor.B32F);
	BearVector4<float> Rect;
	Rect.set(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
	BearVector4<float> UV;
	UV.set(s1, t1, s2, t2);
	GRender->PushUIRender(Rect, UV, LSetColor, hFont, CSRender::FUIR_Font);
}
static int DrawConsoleStringExport(int x, int y, const char* string)
{
	int count = 0;
	count =static_cast<int>( GRender->PushUIText(string, static_cast<float>(x), static_cast<float>(y), LSetColor));
	return count;
}
static void DrawSetTextColorExports(int r, int g, int b, int alpha)
{
	LSetColor.SetAsUint8(static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b), static_cast<uint8>(alpha));
}
static void FillRGBAExport(int x, int y, int width, int height, int r, int g, int b, int a)
{
	LSetColor.SetAsUint8(static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b), static_cast<uint8>(a));
	BearVector4<float> Rect;
	Rect.set(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
	BearVector4<float> UV;
	UV.set(0, 0, 1, 1);
	GRender->PushUIRender(Rect, UV, LSetColor, 0);
}
void CSRender::FunctionToC()
{
	GMenuEngineFunctions.PIC_Set = PICSetExports;
	GMenuEngineFunctions.PIC_Draw = PICDrawExports;
	GMenuEngineFunctions.PIC_DrawAdditive = PICDrawExports;
	GMenuEngineFunctions.PIC_DrawHoles = PICDrawExports;
	GMenuEngineFunctions.PIC_DrawTrans = PICDrawExports;
	GMenuEngineFunctions.DrawCharacter = DrawCharacterExport;
	GMenuEngineFunctions.DrawConsoleString = DrawConsoleStringExport;
	GMenuEngineFunctions.DrawSetTextColor = DrawSetTextColorExports;
	GMenuEngineFunctions.FillRGBA = FillRGBAExport;

}

