#include "CSEngine.hpp"

CSTexture2D::CSTexture2D(BearStringConteniar Name):CSResourceNamed(Name)
{
	if (!GFS->ExistFile(TEXT("%game%"), *Name))return;
	bsize size = BearString::GetSize(*Name);
	bool dds = BearString::Compare(*Name + size - 4, TEXT(".dds")) == 0;
	BearImage Image;
	if (dds)
	{
		BEAR_ASSERT(Image.LoadDDSFromStream(**GFS->Read(TEXT("%game%"), *Name)));
	}
	else
	{
		BEAR_ASSERT(Image.LoadFromStream(0, **GFS->Read(TEXT("%game%"), *Name)));
	}

	m_Size = Image.GetSize();
	Texture2D = BearRenderInterface::CreateTexture2D(Image.GetSize().x, Image.GetSize().y, Image.GetMips(), Image.GetDepth(), Image.GetFormat(), TU_STATIC, *Image);

}

CSTexture2D::CSTexture2D(BearStringConteniar Name, void* data, bsize size) : CSResourceNamed(Name)
{
	BearImage Image;
	BEAR_ASSERT(Image.LoadFromBuffer(BearMemoryStream(data, size)));
	m_Size = Image.GetSize();
	Texture2D = BearRenderInterface::CreateTexture2D(Image.GetSize().x, Image.GetSize().y, Image.GetMips(), Image.GetDepth(), Image.GetFormat(), TU_STATIC, *Image);
}

CSTexture2D::~CSTexture2D()
{
}
