#include "CSEngine.hpp"
FGET_SERVER_FUNCTIONS FGETServerAPI;
FGET_SERVER_NEW_FUNCTIONS FGETServerNewAPI;
FSET_SERVERENGINE_FUNCTIONS FSETServerAPI;
CSMPManager* GMPManager = 0;
static SServerFunction LServerFunction;
static SServerNewFunction LServerNewFunction;
globalvars_t GV;
CSMPManager::CSMPManager()
{
}

CSMPManager::~CSMPManager()
{
}

void CSMPManager::Initialize()
{
	int version = INTERFACE_VERSION;
	BEAR_ASSERT(FGETServerAPI(&LServerFunction, version));
	version = NEW_DLL_FUNCTIONS_VERSION;
	BEAR_ASSERT(FGETServerNewAPI(&LServerNewFunction, &version));

	bear_fill(GV);
	FSETServerAPI(&GServerEngineFunction, &GV);
//	LServerFunction.GameInit();

	//LServerFunction.StartFrame();
}
