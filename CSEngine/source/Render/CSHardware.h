#pragma once

class CSHardware
{
	BEAR_CLASS_WITHOUT_COPY(CSHardware);
public:
	CSHardware();
	~CSHardware();
public:
	BearWindow Window;
	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
	BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass;
	BearFactoryPointer<BearRHI::BearRHIContext> Context;
};
extern CSHardware* GHW;