#pragma once
class  CSBlenderScreenSet :public CSBlenderCompiler
{
public:
	CSBlenderScreenSet();
	virtual ~CSBlenderScreenSet();
	virtual void Initialize();
	virtual void Compile(CSShaderElement& shader);
	//virtual void Compile(XRayShader&shader);
private:
};