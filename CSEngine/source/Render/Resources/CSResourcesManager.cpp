#include "CSEngine.hpp"
#include "../Blenders/CSBlenderScreenSet.h"

 CSResourcesManager* GResourcesManager = 0;
CSResourcesManager::CSResourcesManager()
{
	FunctionToC();
	m_Blenders.insert(TEXT("hud/default"), bear_new< CSBlenderScreenSet>());
	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		GResourcesManager = this;
		b->second->Initialize();
	}
	{
		BearSamplerDescription Description;
		SamplerDefault = BearRenderInterface::CreateSampler(Description);
	}
}

CSResourcesManager::~CSResourcesManager()
{
	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		bear_delete(b->second);
	}
	for (auto b = m_Textures2Himage.begin(), e = m_Textures2Himage.end(); b != e; b++)
	{
		if ((*b).size())
		{
			Free(m_Textures[*b]);
		}
	}
	BEAR_RASSERT(m_Textures.size() == 0);
}
CSTexture2D* CSResourcesManager::GetTexture(HIMAGE image)
{
	if (image < 1)return 0;
	auto texture = m_Textures.find(m_Textures2Himage[image-1]);
	BEAR_ASSERT(texture != m_Textures.end());
	return texture->second;
}
CSTexture2D* CSResourcesManager::GetTexture(BearStringConteniar name)
{
	if (m_NotFoundedTextures.find(name) != m_NotFoundedTextures.end())return 0;
	auto texture = m_Textures.find(name);
	if (texture == m_Textures.end())
	{

		BearLog::Printf(TEXT("Load texture [%s]"), *name);

		CSTexture2D*  t = bear_new<CSTexture2D>(name);
		if (t->Empty())
		{
			m_NotFoundedTextures.insert(name,true);
			BearLog::Printf(TEXT("! Can't load texture [%s]"), *name);
			bear_delete(t);
			return 0;
		}
		m_Textures.insert(name, t);
		texture = m_Textures.find(name);
	}
	texture->second->Count++;
	return texture->second;
}

CSTexture2D* CSResourcesManager::GetTexture(BearStringConteniar name, void* data, bsize size)
{
	auto texture = m_Textures.find(name);
	if (texture == m_Textures.end())
	{

		BearLog::Printf(TEXT("Load texture [%s]"), *name);
		m_Textures.insert(name, bear_new<CSTexture2D>(name, data,size));
		texture = m_Textures.find(name);
	}
	texture->second->Count++;
	return texture->second;
}

class CSShaderIncluder :public BearIncluder
{
public:
	CSShaderIncluder() {}
	virtual BearRef<BearInputStream> OpenAsStream(const bchar* name)
	{
		return GFS->Read(TEXT("%shaders%"), name);
	}
	virtual BearRef<BearBufferedReader> OpenAsBuffer(const bchar* name)
	{
		return BearRef<BearBufferedReader>(bear_new<BearMemoryStream>(**GFS->Read(TEXT("%shaders%"), name)));
	}
};


BearFactoryPointer<BearRHI::BearRHIShader> CSResourcesManager::GetPixelShader(BearStringConteniar name)
{
	auto item = m_PShaders.find(name);
	if (item == m_PShaders.end())
	{
		BearLog::Printf(TEXT("Load shader [%s]"), *name);

		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreatePixelShader();
		BearString Text;
		{
			auto File = GFS->Read(TEXT("%shaders%"), *name, TEXT(".ps"));
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{

			CSShaderIncluder Includer;
			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, &Includer))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BEAR_RASSERT(0);
			}
			if (out.size())
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_PShaders.insert(name, shader);
		return shader;
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIShader> CSResourcesManager::GetVertexShader(BearStringConteniar name)
{
	auto item = m_VShaders.find(name);
	if (item == m_VShaders.end())
	{
		BearLog::Printf(TEXT("Load shader [%s]"), *name);

		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreateVertexShader();
		BearString Text;
		{
			auto File = GFS->Read(TEXT("%shaders%"), *name, TEXT(".vs"));
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{


			CSShaderIncluder Includer;
			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, &Includer))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BEAR_RASSERT(0);
			}
			if (out.size())
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_VShaders.insert(name, shader);
		return shader;
	}
	return item->second;
}

void CSResourcesManager::Free(CSTexture2D* texture)
{
	if (!texture)return;
	texture->Count--;
	if (texture->Count == 0)
	{
		if (texture->UniqueNumber>=0)
		{

			m_Textures2Himage[texture->UniqueNumber].set(TEXT(""));
		}
		BearLog::Printf(TEXT("Unload texture [%s]"),* texture->GetName());
		m_Textures.erase(texture->GetName());
		bear_delete(texture);
	}
}

HIMAGE CSResourcesManager::TextureToHimage(BearStringConteniar name)
{
	CSTexture2D* Texture = 0;
	{
		auto item = m_Textures.find(name);
		if (item == m_Textures.end())
		{
			Texture = GetTexture(name);
		}
		else
		{
			if (item->second->UniqueNumber >= 0)
			{
				return item->second->UniqueNumber+1;
			}
			Texture = item->second;
		}
	}
	for (auto b = m_Textures2Himage.begin(),  e = m_Textures2Himage.end(); b != e; b++)
	{
		if ((*b).size()==0)
		{
			(*b) = name;
			Texture->UniqueNumber = static_cast<int32>(b - m_Textures2Himage.begin());
			return Texture->UniqueNumber+1;
		}
	}
	Texture->UniqueNumber = static_cast<int32>(m_Textures2Himage.size());
	m_Textures2Himage.push_back(name);
	return Texture->UniqueNumber+1;
}

BearStringConteniar CSResourcesManager::HimageToTexture(HIMAGE id)
{
	if (id == 0)return TEXT("null.tga");
	BEAR_ASSERT(id > 0);
	return m_Textures2Himage[id-1];
}

void CSResourcesManager::Compile(CSShader& shader, BearStringConteniar Name, const bchar* textures)
{
	auto item = m_Blenders.find(Name);
	BEAR_ASSERT(item != m_Blenders.end());
	item->second->Compile(shader, textures);
}

