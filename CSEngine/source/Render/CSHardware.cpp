#include "CSEngine.hpp"
CSHardware* GHW = 0;
CSHardware::CSHardware():Window()
{
	BEAR_ASSERT(BearRenderInterface::Initialize(TEXT("bear_render_vulkan1_0")));
	{
		BearViewportDescription Description;
		Description.Clear = true;
		Description.ClearColor = BearColor::Black;

		Viewport = BearRenderInterface::CreateViewport(Window, Description);
	}
	{
		BearRenderPassDescription Description;
		Description.RenderTargets[0].Format = Viewport->GetFormat();
		RenderPass = BearRenderInterface::CreateRenderPass( Description);
	}
	Context = BearRenderInterface::CreateContext();
	Context->AttachViewportAsFrameBuffer(Viewport);
	Context->SetViewport(0, 0, Window.GetSizeFloat().x,  Window.GetSizeFloat().y);
}

CSHardware::~CSHardware()
{
	Context.clear();
	Viewport.clear();

	BearRenderInterface::Destroy();
}
