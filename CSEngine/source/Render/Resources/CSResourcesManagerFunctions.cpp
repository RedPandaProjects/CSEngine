#include "CSEngine.hpp"

HIMAGE LoadTextureExport(const char* szPicName, const byte* ucRawImage, long ulRawImageSize, long flags)
{
	CSTexture2D* Texture = 0;
	{
		if (ucRawImage)
		{
			Texture = GResourcesManager->GetTexture(
#ifdef UNICODE
				* BearEncoding::FastToUnicode(szPicName)
#else
			 szPicName
#endif
				, const_cast<void*>(reinterpret_cast<const void*>( ucRawImage)),ulRawImageSize
			);
		}
		else
		{

			Texture = GResourcesManager->GetTexture(
#ifdef UNICODE
				* BearEncoding::FastToUnicode(szPicName)
#else
				szPicName
#endif
			);
		}
	}
	if(Texture)
	return GResourcesManager->TextureToHimage(
#ifdef UNICODE
		* BearEncoding::FastToUnicode(szPicName)
#else
		szPicName
#endif
	);
	return 0;
}
void FreeTextureExport(const char* szPicName)
{

	CSTexture2D* Texture =  GResourcesManager->GetTexture(
#ifdef UNICODE
		* BearEncoding::FastToUnicode(szPicName)
#else
		szPicName
#endif
	);

	GResourcesManager->Free(Texture);
	GResourcesManager->Free(Texture);
}
static int GetWidthExport(HIMAGE image)
{
	return static_cast<int>(GResourcesManager->GetTexture(image)->GetSize().x);
}
static int GetHeightExport(HIMAGE image)
{
	return static_cast<int>(GResourcesManager->GetTexture(image)->GetSize().y);
}
void CSResourcesManager::FunctionToC()
{
	FUIEngine.pfnPIC_Load = LoadTextureExport;
	FUIEngine.pfnPIC_Free = FreeTextureExport;
	FUIEngine.pfnPIC_Width = GetWidthExport;
	FUIEngine.pfnPIC_Height = GetHeightExport;
}