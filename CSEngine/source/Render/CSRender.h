#pragma once
class CSRender
{
	BEAR_CLASS_WITHOUT_COPY(CSRender);
public:
	CSRender();
	void Resize(bsize w,bsize h);
	~CSRender();
	enum FUIRender
	{
		FUIR_None=0,
		FUIR_Font=1<<0,
	};
	void PushUIRender(const BearVector4<float>&rect,const BearVector4<float>&uv,const BearColor&color,HIMAGE image,BearFlags<uint32> Flags= FUIR_None);
	void FlushUI();
	bool SetShader(BearFactoryPointer<BearRHI::BearRHIContext>&context, CSShaderElement&element, ShaderVertexDeclaration fvf);
private:
	void FunctionToC();
	BearVector<CSShader> m_UIShaders;
	struct SUIRender
	{
		BearVector4<float> UV;
		BearVector4<float> Rect;
		BearColor Color;
		HIMAGE Image;
		BearFlags<uint32> Flags;
	};
	BearVector<SUIRender> m_UIRenders;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_UIVertexBuffer;
private:
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_ScreenTransformation;
};
extern CSRender* GRender;