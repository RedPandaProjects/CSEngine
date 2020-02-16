#include "CSEngine.hpp"

void CSBlenderCompiler::Initialize()
{
}





CSBlenderCompiler::CSBlenderCompiler()
{
	
}

CSBlenderCompiler::~CSBlenderCompiler()
{
}
inline  bsize GetSize(BearVertexFormat format)
{
	switch (format)
	{
	case VF_R32_FLOAT:
		return sizeof(float) * 1;
	case VF_R32G32_FLOAT:
		return sizeof(float) * 2;
	case VF_R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case VF_R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case VF_R32_INT:
		return sizeof(int32);
	case VF_R8:
		return sizeof(int8) * 1;
	case VF_R8G8:
		return sizeof(int8) * 2;
	case VF_R8G8B8A8:
		return sizeof(int8) * 4;
	default:
		return 0;
	}
}
void CSBlenderCompiler::Compile(CSShaderElement& shader)
{
}
inline void FixTextureName(const bchar* fn)
{
	/*LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;*/
}
void CSBlenderCompiler::Compile(CSShader& shader, const bchar* textures)
{
	m_textures.clear();
	if (textures && textures[0])
	{
		bchar* P = (bchar*)textures;
		BearVector<bchar> N;
		N.reserve(128);
		while (*P)
		{
			if (*P == ',')
			{
				// flush
				N.push_back(0);
				//BearString::ToLower (N.data());

				FixTextureName(N.data());
				m_textures.push_back(N.data());
				N.clear();
			}
			else
			{
				N.push_back(*P);
			}
			P++;
		}
		if (N.size())
		{
			// flush
			N.push_back(0);
			//BearString::ToLower(N.data());

			FixTextureName(N.data());
			m_textures.push_back(N.data());
		}
	}
	
	for (bsize i = 0; i < 6; i++)
	{
		IDShader = i;
		shader.E[i].RootSignature = RootSignature[i];
		for (bsize a = 0; a < SVD_Count; a++)
			shader.E[i].Pipeline[a] = m_pipeline[i][a];
		Compile(shader.E[i]);
	}
	
}



void CSBlenderCompiler::SetInputLayout(BearPipelineDescription& Description, ShaderVertexDeclaration VertexState)
{
	switch (VertexState)
	{
	case SVD_UI:
		Description.InputLayout.Elements[0] = BearInputLayoutElement("POSITION", VF_R32G32_FLOAT,0 );
		Description.InputLayout.Elements[1] = BearInputLayoutElement("COLOR", VF_R32G32B32A32_FLOAT, 8);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("UV", VF_R32G32_FLOAT, 24);
		Description.InputLayout.Elements[3] = BearInputLayoutElement();
		break;
	default:
		BEAR_ASSERT(0);
		break;
	}
}



void CSBlenderCompiler::SetTexture(CSShaderElement& shader, bsize id, BearStringConteniar name)
{
	shader.Textures[id] = GResourcesManager->GetTexture(GetTexture(name));
}

void CSBlenderCompiler::CreatePipeline(BearPipelineDescription& Description, bsize shader, const bchar* name_vs, const bchar* name_ps, ShaderVertexDeclaration FVFType)
{
	
	const bchar* prefix = TEXT("");
	switch (FVFType)
	{
	case SVD_UI:
		prefix = TEXT("_ui");
		break;
	default:
		BEAR_RASSERT(0);
		break;
	}
	BearStringPath NameVS;
	BearString::Printf(NameVS, TEXT("%s%s"), name_vs, prefix);
	BearString::ToLower(NameVS);
	SetInputLayout(Description, FVFType);
	Description.Shaders.Pixel = GResourcesManager->GetPixelShader(name_ps);
	Description.Shaders.Vertex = GResourcesManager->GetVertexShader(NameVS);
	Description.RootSignature = RootSignature[shader];
	BEAR_ASSERT(m_pipeline[shader][FVFType].empty());
	m_pipeline[shader][FVFType] = BearRenderInterface::CreatePipeline(Description);

}


inline int ParseName(const bchar* N)
{
	if (0 == BearString::CompareWithoutCase(N,TEXT( "$null")))
		return -1;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base0")))
		return 0;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base1")))
		return 1;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base2")))
		return 2;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base3")))
		return 3;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base4")))
		return 4;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base5")))
		return 5;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base6")))
		return 6;
	if (0 == BearString::CompareWithoutCase(N, TEXT("$base7")))
		return 7;
	return -1;
}

BearStringConteniar CSBlenderCompiler::GetTexture(BearStringConteniar name)
{
	int id = ParseName(*name);
	if (id >= 0&&id<m_textures.size())
	{
		return m_textures[id];
	}
	return name;
}

/*
void CSShader::Compile(XRayShader & shader)
{
	BEAR_RASSERT(false);
}
*/