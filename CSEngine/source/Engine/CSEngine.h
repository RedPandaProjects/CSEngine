#pragma once
class CSEngine
{
	BEAR_CLASS_WITHOUT_COPY(CSEngine);
public:
	CSEngine();
	~CSEngine();
	static void Startup();
private:
	void Initialize();
	void Loop();
	void DestroyDLL();
	void Destroy();
	void MakeMapList();
	void InitializeDLL();
	void InitializeGame();
private:
	void FillListFunction(void* Functions, bsize count);
	void FunctionToC();
private:
	BearVector<BearStringConteniar> MapList;
};
extern const bchar* GGameName;
extern const bchar* GBaseDir;
extern const bchar* GDLLName;
extern CSEngine* GEngine;
extern SEngineMenuFunctions GMenuEngineFunctions;
extern SServerEngineFunction GServerEngineFunction;