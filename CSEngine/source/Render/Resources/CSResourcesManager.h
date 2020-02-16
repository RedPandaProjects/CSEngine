#pragma once
class CSResourcesManager
{
	BEAR_CLASS_WITHOUT_COPY(CSResourcesManager);
public:
	CSResourcesManager();
	~CSResourcesManager();

	CSTexture2D* GetTexture(HIMAGE image);
	CSTexture2D* GetTexture(BearStringConteniar name);
	CSTexture2D* GetTexture(BearStringConteniar name,void *data,bsize size);
	void Free(CSTexture2D* texture);
	HIMAGE TextureToHimage(BearStringConteniar name);

	BearFactoryPointer<BearRHI::BearRHIShader> GetPixelShader(BearStringConteniar name);
	BearFactoryPointer<BearRHI::BearRHIShader> GetVertexShader(BearStringConteniar name);

	BearStringConteniar HimageToTexture(HIMAGE id);

	void Compile(CSShader& shader, BearStringConteniar Name, const bchar* textures);

	BearFactoryPointer<BearRHI::BearRHISampler> SamplerDefault;

private:

	void FunctionToC();
	BearMap< BearStringConteniar, CSTexture2D*> m_Textures;
	BearVector<BearStringConteniar> m_Textures2Himage;
	BearMap<BearStringConteniar,bool> m_NotFoundedTextures;
	BearMap< BearStringConteniar, BearFactoryPointer<BearRHI::BearRHIShader>> m_PShaders;
	BearMap< BearStringConteniar, BearFactoryPointer<BearRHI::BearRHIShader>> m_VShaders;

	BearMap< BearStringConteniar, CSBlenderCompiler*> m_Blenders;
};
extern CSResourcesManager* GResourcesManager;