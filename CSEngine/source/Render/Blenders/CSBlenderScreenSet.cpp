#include "CSEngine.hpp"
#include "CSBlenderScreenSet.h"

#define		VER_2_oBlendCount	7
#define		VER_4_oBlendCount	9
#define		VER_5_oBlendCount	10

CSBlenderScreenSet::CSBlenderScreenSet()
{
}

CSBlenderScreenSet::~CSBlenderScreenSet()
{
}

void CSBlenderScreenSet::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;

	
	CreatePipeline(PipelineDescription,TEXT("notransform"), TEXT("default"),SVD_UIT);

}
void CSBlenderScreenSet::Compile(CSShaderElement& shader)
{
	if (IDShader == 0)
	{
		//SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Screen;
	}
}
/*
void CSBlenderScreenSet::Compile(XRayShader & shader)
{
	shader.SetVertexState(FVF::F_TL);
	shader.SetShader( "default_tl", "notransform_tl");
	shader.SetTransformType(shader.TT_Screen);
	switch (oBlend.IDselected)
	{
	case 6:
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	default:
		break;
	}
	switch (oBlend.IDselected)
	{
	case 0:	// SET
		break;
	case 1: // BLEND
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	case 2:	// ADD
		shader.SetBlend(XRayShader::BF_ONE, XRayShader::BF_ONE);
		break;
	case 3:	// MUL
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_ZERO);
		break;
	case 4:	// MUL_2X
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_SRC_COLOR);
		break;
	case 5:	// ALPHA-ADD
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_ONE);
		break;
	case 6:	// MUL_2X + A-test
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_SRC_COLOR);
		break;
	case 7:	// SET (2r)
		break;
	case 8: // BLEND (2r)
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	case 9: // BLEND (2r)
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	}
}

void CSBlenderScreenSet::Destroy()
{
	bear_delete(this);
}
*/