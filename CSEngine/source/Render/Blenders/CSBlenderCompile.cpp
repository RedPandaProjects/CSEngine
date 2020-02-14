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
inline void FixTextureName(LPSTR fn)
{
	/*LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;*/
}
void CSBlenderCompiler::Compile(CSShader& shader/*, const bchar* textures*/ )
{
	/*m_textures.clear();
	if (textures && textures[0])
	{
		char* P = (char*)textures;
		svector<char, 128> N;

		while (*P)
		{
			if (*P == ',')
			{
				// flush
				N.push_back(0);
				strlwr(N.begin());

				FixTextureName(N.begin());
				m_textures.push_back(N.begin());
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
			strlwr(N.begin());

			FixTextureName(N.begin());
			m_textures.push_back(N.begin());
		}
	}
	*/
	for (bsize i = 0; i < 6; i++)
	{
		IDShader = i;
		shader.E[i].RootSignature = RootSignature;
		for (bsize a = 0; a < SVD_Count; a++)
			shader.E[i].Pipeline[a] = m_pipeline[a];
		Compile(shader.E[i]);
	}
	
}



void CSBlenderCompiler::SetInputLayout(BearPipelineDescription& Description, ShaderVertexDeclaration VertexState)
{
	switch (VertexState)
	{
	case SVD_UI:
		Description.InputLayout.Elements[0] = BearInputLayoutElement("POSITION", VF_R32G32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("COLOR", VF_R32G32B32A32_FLOAT, 8);
		Description.InputLayout.Elements[2] = BearInputLayoutElement();
		break;
	case SVD_UIT:
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


/*
void CSBlenderCompiler::SetTexture(CSShaderElement& shader, bsize id, BearStringConteniar name)
{
	shader.Textures[id] = GResourcesManager->GetTexture(GetTexture(name));
}*/

void CSBlenderCompiler::CreatePipeline(BearPipelineDescription& Description, const bchar* name_vs, const bchar* name_ps, ShaderVertexDeclaration FVFType)
{
	
	const bchar* prefix = TEXT("");
	switch (FVFType)
	{
	case SVD_UI:
		prefix = TEXT("_ui");
		break;
	case SVD_UIT:
		prefix = TEXT("_uit");
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
	Description.RootSignature = RootSignature;
	BEAR_ASSERT(m_pipeline[FVFType].empty());
	m_pipeline[FVFType] = BearRenderInterface::CreatePipeline(Description);

}


inline int ParseName(LPCSTR N)
{
	if (0 == strcmp(N, "$null"))
		return -1;
	if (0 == strcmp(N, "$base0"))
		return 0;
	if (0 == strcmp(N, "$base1"))
		return 1;
	if (0 == strcmp(N, "$base2"))
		return 2;
	if (0 == strcmp(N, "$base3"))
		return 3;
	if (0 == strcmp(N, "$base4"))
		return 4;
	if (0 == strcmp(N, "$base5"))
		return 5;
	if (0 == strcmp(N, "$base6"))
		return 6;
	if (0 == strcmp(N, "$base7"))
		return 7;
	return -1;
}
/*
BearStringConteniar CSBlenderCompiler::GetTexture(BearStringConteniar name)
{
	int id = ParseName(*name);
	if (id >= 0&&id<m_textures.size())
	{
		return m_textures[id];
	}
	return name;
}*/

/*
void CSShader::Compile(XRayShader & shader)
{
	BEAR_RASSERT(false);
}
*/