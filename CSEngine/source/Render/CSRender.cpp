#include "CSEngine.hpp"
CSRender* GRender = 0;

CSRender::CSRender()
{
	{
		m_ScreenTransformation = BearRenderInterface::CreateUniformBuffer();
		m_ScreenTransformation->Create(sizeof(float) * 4, true);
	}
	{
#ifdef DEVELOPER_VERSION
		BearStringPath path;
		GFS->UpdatePath(TEXT("%game%"), 0, path);
		BearFileManager::PathCombine(path, TEXT("fonts"),TEXT("default.ttf"));
		BEAR_ASSERT(m_DefaultFont.LoadFromTTF(path, TEXT("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm!@#$%^&*()_-=+/\\|\"\'"), 1024, 768, 24));
		GFS->UpdatePath(TEXT("%game%"), 0, path);
		BearFileManager::PathCombine(path, TEXT("fonts"), TEXT("default.fon"));
		BEAR_ASSERT(m_DefaultFont.SaveToFile(path));
#else
		BEAR_ASSERT(m_DefaultFont.LoadFromStream(**GFS->Read(TEXT("%game%"), TEXT("fonts" BEAR_PATH "default.fon"))));
#endif

		GResourcesManager->GetTexture(TEXT("font/default"))->Texture2D = m_DefaultFont.CreateTexture();
		m_DefauktFontImage = GResourcesManager->TextureToHimage(TEXT("font/default"));
	}
	Resize(1024, 768);
	FunctionToC();


}

void CSRender::Resize(bsize w, bsize h)
{
	BearVector4<float> ScreenTransformation;
	ScreenTransformation.set(static_cast<float>(w), static_cast<float>(h), 1.f / static_cast<float>(w), 1.f / static_cast<float>(h));
	bear_copy(m_ScreenTransformation->Lock(), ScreenTransformation.Array, sizeof(float) * 4);
	m_ScreenTransformation->Unlock();
}

CSRender::~CSRender()
{
}

void CSRender::PushUIRender(const BearVector4<float>& rect, const BearVector4<float>& uv, const BearColor& color, HIMAGE image, BearFlags<uint32> Flags)
{
	SUIRender in;
	in.UV = uv;
	in.Rect = rect;
	in.Color = color;
	in.Image = image;
	in.Flags = Flags;
	m_UIRenders.push_back(in);
}

bsize CSRender::PushUIText(const char* str, float x_, float y_, const BearColor& color)
{
	bsize count = 0;
	BearVector2<float> pos(x_, y_);
	while (*str)
	{
		count++;
		auto&map= m_DefaultFont.GetListChars();
		auto item =  map.find(BearEncoding::ToUTF16(*str));
		if(item != map.end())
		{
			float x = pos.x + item->second.Position.x, y = pos.y + item->second.Position.y;
			float width = item->second.Size.x;
			float height = item->second.Size.y;

			SUIRender in;
			in.UV = item->second.TextureUV;
			in.UV.x1 += in.UV.x;
			in.UV.y1 += in.UV.y;
			in.Rect .set(x,y,width,height);
			in.Color = color;
			in.Image = m_DefauktFontImage;
			in.Flags = CSRender::FUIR_Font;
			m_UIRenders.push_back(in);
			pos.x += item->second.Advance.x;
		}
		str++;
	}
	return count;
}



void CSRender::FlushUI()
{
	if (m_UIRenders.size() == 0)return;
	if (m_UIVertexBuffer.empty())
	{
		m_UIVertexBuffer = BearRenderInterface::CreateVertexBuffer();
	}
	if (m_UIVertexBuffer->GetCount()< m_UIRenders.size()*6)
	{
		m_UIVertexBuffer->Create(sizeof(F_UI), bear_recommended_size(m_UIRenders.size()) * 6, true, 0);
	}
	HIMAGE max_image = 0;
	for (auto b = m_UIRenders.begin(), e = m_UIRenders.end(); b != e; b++)
	{
		max_image = BearMath::max(b->Image, max_image);
	}
	if (static_cast<int>( m_UIShaders.size()) < max_image + 1)
	{
		m_UIShaders.resize(max_image + 1);
	}
	for (auto b = m_UIRenders.begin(), e = m_UIRenders.end(); b != e; b++)
	{
		if (m_UIShaders[b->Image].E[0].Empty())
		{
			GResourcesManager->Compile(m_UIShaders[b->Image], TEXT("hud/default"), *GResourcesManager->HimageToTexture(b->Image));
		}
	}
	F_UI* pv = (F_UI*)m_UIVertexBuffer->Lock();
	for (auto b = m_UIRenders.begin(), e = m_UIRenders.end(); b != e; b++)
	{
		pv->set(b->Rect.x, b->Rect.y, b->Color, b->UV.x, b->UV.y); pv++;
		pv->set(b->Rect.x+ b->Rect.x1, b->Rect.y, b->Color, b->UV.x1, b->UV.y); pv++;
		pv->set(b->Rect.x, b->Rect.y+ b->Rect.y1, b->Color, b->UV.x, b->UV.y1); pv++;

		pv->set(b->Rect.x + b->Rect.x1, b->Rect.y, b->Color, b->UV.x1, b->UV.y); pv++;
		pv->set(b->Rect.x + b->Rect.x1, b->Rect.y + b->Rect.y1, b->Color, b->UV.x1, b->UV.y1); pv++;
		pv->set(b->Rect.x, b->Rect.y + b->Rect.y1, b->Color, b->UV.x, b->UV.y1); pv++;
	}
	m_UIVertexBuffer->Unlock();
	GHW->Context->SetVertexBuffer(m_UIVertexBuffer);
	bsize offset = 0;
	for (auto b = m_UIRenders.begin(), e = m_UIRenders.end(); b != e; b++)
	{
		bsize id = 0;
		if (b->Flags.test(FUIR_Font))
		{
			id = 5;
		}
		if(SetShader(GHW->Context, m_UIShaders[b->Image].E[id], SVD_UI))
			GHW->Context->Draw(6, offset);
		offset += 6;
	}
	m_UIRenders.clear_not_free();
}

bool CSRender::SetShader(BearFactoryPointer<BearRHI::BearRHIContext>& context, CSShaderElement& ShaderElement, ShaderVertexDeclaration fvf)
{
	if (ShaderElement.RootSignature.empty())return false;
	if (ShaderElement.DescriptorHeap.empty())
	{
		ShaderElement.CreateDescriptorHeap();
	}
	switch (ShaderElement.TypeTransformation)
	{
	case STT_Screen:
		ShaderElement.DescriptorHeap->SetUniformBuffer(0, m_ScreenTransformation);
		break;
	case STT_Matrix:
	{
		/*BEAR_ASSERT(sizeof(ContatMatrix) == sizeof(float) * 16 * 4)
			m_MWV.mul_43(m_MView, m_MWorld);
		m_MVPW.mul(m_MProject, m_MWV);

		if (ShaderElement.TransformationBuffer.empty())
		{
			ShaderElement.TransformationBuffer = BearRenderInterface::CreateUniformBuffer();
			ShaderElement.TransformationBuffer->Create(sizeof(ContatMatrix), true);
		}

		ContatMatrix* matrix = (ContatMatrix*)ShaderElement.TransformationBuffer->Lock();
		matrix->View = m_MView;
		matrix->Project = m_MProject;
		matrix->World = m_MWorld;
		matrix->VPW = m_MVPW;
		ShaderElement.TransformationBuffer->Unlock();
		ShaderElement.DescriptorHeap->SetUniformBuffer(0, ShaderElement.TransformationBuffer);*/
	}
	break;
	default:
		break;
	}
	for (bsize i = 0; i < 16; i++)
	{
		if (ShaderElement.Textures[i])
		{
			//ShaderElement.Textures[i]->Update();
			ShaderElement.DescriptorHeap->SetShaderResource(i, ShaderElement.Textures[i]->Texture2D);
		}
	}
	return ShaderElement.Set(context, fvf);
}

