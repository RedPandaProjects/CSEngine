#pragma once
class CSResourcesManager
{
	BEAR_CLASS_WITHOUT_COPY(CSResourcesManager);
public:
	CSResourcesManager();
	~CSResourcesManager();
	CSTexture2D* GetTexture(BearStringConteniar name);
	CSTexture2D* GetTexture(BearStringConteniar name,void *data,bsize size);
	void Free(CSTexture2D* texture);
	HIMAGE TextureToHimage(CSTexture2D* texture);

	BearFactoryPointer<BearRHI::BearRHIShader> GetPixelShader(BearStringConteniar name);
	BearFactoryPointer<BearRHI::BearRHIShader> GetVertexShader(BearStringConteniar name);

	CSTexture2D* HimageToTexture(HIMAGE id);

	void Compile(CSShader& shader, BearStringConteniar Name);
private:

	void FunctionToC();
	BearMap< BearStringConteniar, CSTexture2D*> m_Textures;
	BearVector<CSTexture2D*> m_Textures2Himage;

	BearMap< BearStringConteniar, BearFactoryPointer<BearRHI::BearRHIShader>> m_PShaders;
	BearMap< BearStringConteniar, BearFactoryPointer<BearRHI::BearRHIShader>> m_VShaders;

	BearMap< BearStringConteniar, CSBlenderCompiler*> m_Blenders;
};
extern CSResourcesManager* GResourcesManager;