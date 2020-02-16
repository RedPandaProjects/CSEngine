#pragma once
#pragma pack(push,4)

class CSBlenderCompiler
{
	BEAR_CLASS_WITHOUT_COPY(CSBlenderCompiler);
public:
	virtual void Initialize();
	CSBlenderCompiler();
	virtual  ~CSBlenderCompiler();
	virtual void Compile(CSShaderElement& shader);
	void Compile(CSShader& shader, const bchar* textures);
protected:
	void SetInputLayout(BearPipelineDescription& Description, ShaderVertexDeclaration VertexState);
	void SetTexture(CSShaderElement& shader,bsize id, BearStringConteniar name);
	void CreatePipeline(BearPipelineDescription& Description,bsize shader,const bchar*name_vs, const bchar* name_ps, ShaderVertexDeclaration FVFType);
	//;
protected:
	BearStringConteniar GetTexture(BearStringConteniar name);
	bsize IDShader;

	BearFactoryPointer < BearRHI::BearRHIRootSignature> RootSignature[6];
private:
	BearVector<BearStringConteniar> m_textures;
private:
	BearFactoryPointer<BearRHI::BearRHIPipeline> m_pipeline[6][SVD_Count];
};
#pragma pack(pop)