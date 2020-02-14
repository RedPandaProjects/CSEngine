#pragma once
enum ShaderTypeTransformation
{
	STT_Screen,
	STT_Matrix,
};
enum ShaderVertexDeclaration
{
	SVD_UI,
	SVD_UIT,
	SVD_Count,
};
enum ShaderSamplerState
{
	SSS_None,
	SSS_Default,
};
class  CSShaderElement
{
public:
	
	CSShaderElement() { bear_fill(Textures); for (bsize i = 0; i < 16; i++)SamplerStates[i] = SSS_None; }
	~CSShaderElement();
	void Clear();
	void Copy(const CSShaderElement& right);
	void Swap(CSShaderElement& right);
	
	CSShaderElement(const CSShaderElement& right)	{ bear_fill(Textures); Copy(right);}
	CSShaderElement( CSShaderElement&& right){  bear_fill(Textures); for (bsize i = 0; i < 16; i++)SamplerStates[i] = SSS_None;;	Swap(right);}
	CSShaderElement& operator=(const CSShaderElement& right) { Copy(right); return*this; }
	CSShaderElement& operator=( CSShaderElement&& right) { Swap(right); return*this; }

	BearFactoryPointer<BearRHI::BearRHIPipeline> Pipeline[SVD_Count];
	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;
	ShaderTypeTransformation TypeTransformation;
	CSTexture2D* Textures[16];
	ShaderSamplerState SamplerStates[16];
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> TransformationBuffer;
	void CreateDescriptorHeap();
	inline bool Set(BearFactoryPointer<BearRHI::BearRHIContext>& context, ShaderVertexDeclaration fvf)
	{
		//BEAR_ASSERT(!Pipeline[fvf].empty());
		if (Pipeline[fvf].empty())return false;
		context->SetPipeline(Pipeline[fvf]);
		context->SetDescriptorHeap(DescriptorHeap);
		return true;
	}
private:
	

};
struct CSShader
{
	CSShaderElement E[6];
	inline void Clear()
	{
		for (bsize i = 0; i < 6; i++)E[i].Clear();
	}
};