#include "CSEngine.hpp"
CSHardware* GHW = 0;
CSHardware::CSHardware():Window()
{
	BEAR_ASSERT(BearRenderInterface::Initialize(TEXT("bear_render_dx11")));
	{
		BearViewportDescription Description;
		Description.Clear = true;
		Description.ClearColor = BearColor::Black;

		Viewport = BearRenderInterface::CreateViewport(Window, Description);
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
