#include "CSEngine.hpp"
#include "../Blenders/CSBlenderScreenSet.h"

 CSResourcesManager* GResourcesManager = 0;
CSResourcesManager::CSResourcesManager()
{
	FunctionToC();
	m_Blenders.insert(TEXT("hud/default"), bear_new< CSBlenderScreenSet>());
	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		b->second->Initialize();
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
		if ((*b) != 0)
		{
			Free((*b));
		}
	}
	BEAR_RASSERT(m_Textures.size() == 0);
}
CSTexture2D* CSResourcesManager::GetTexture(BearStringConteniar name)
{
	auto texture = m_Textures.find(name);
	if (texture == m_Textures.end())
	{

		BearLog::Printf(TEXT("Load texture [%s]"), *name);

		CSTexture2D*  t = bear_new<CSTexture2D>(name);
		if (t->Texture2D.empty())
		{
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

BearFactoryPointer<BearRHI::BearRHIShader> CSResourcesManager::GetPixelShader(BearStringConteniar name)
{
	auto item = m_PShaders.find(name);
	if (item == m_PShaders.end())
	{
		BearLog::Printf(TEXT("Load shader [%s]"), *name);

		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreatePixelShader();
		BearString Text;
		{
			auto File = GFS->Read(TEXT("%cur_shaders%"), *name, TEXT(".ps"));
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{


			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, 0))
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
			auto File = GFS->Read(TEXT("%cur_shaders%"), *name, TEXT(".vs"));
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{


			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, 0))
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
		if (texture->UniqueNumber)
		{

			m_Textures2Himage[texture->UniqueNumber] = 0;
		}
		BearLog::Printf(TEXT("Unload texture [%s]"),* texture->GetName());
		m_Textures.erase(texture->GetName());
		bear_delete(texture);
	}
}

HIMAGE CSResourcesManager::TextureToHimage(CSTexture2D* texture)
{
	for (auto b = m_Textures2Himage.begin(),  e = m_Textures2Himage.end(); b != e; b++)
	{
		if ((*b) == 0)
		{
			(*b) = texture;
			texture->UniqueNumber = static_cast<int32>(b - m_Textures2Himage.begin());
			return texture->UniqueNumber+1;
		}
	}
	texture->UniqueNumber = static_cast<int32>(m_Textures2Himage.size());
	m_Textures2Himage.push_back(texture);
	return texture->UniqueNumber+1;
}

CSTexture2D* CSResourcesManager::HimageToTexture(HIMAGE id)
{
	BEAR_ASSERT(id > 0);
	return m_Textures2Himage[id-1];
}

void CSResourcesManager::Compile(CSShader& shader, BearStringConteniar Name)
{
	auto item = m_Blenders.find(Name);
	BEAR_ASSERT(item == m_Blenders.end());
	item->second->Compile(shader);
}

