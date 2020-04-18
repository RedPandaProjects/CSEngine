#include "CSEngine.hpp"
class CSMPManager
{
	BEAR_CLASS_WITHOUT_COPY(CSMPManager);
public:
	CSMPManager();
	~CSMPManager();
	void Initialize();
private:
	
};

extern CSMPManager* GMPManager;
extern FGET_SERVER_FUNCTIONS FGETServerAPI;
extern FGET_SERVER_NEW_FUNCTIONS FGETServerNewAPI;
extern FSET_SERVERENGINE_FUNCTIONS FSETServerAPI;