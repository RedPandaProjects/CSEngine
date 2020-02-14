#pragma once

class CSCVars
{
	BEAR_CLASS_WITHOUT_COPY(CSCVars);
public:
	CSCVars();
	~CSCVars();
	cvar_t* RegisterVariable(const char* szName, const char* szValue, int flags);
	float GetFloat(const char* szName);
	char* GetString(const char* szName);
	void  SetString(const char* szName, const char* szValue);
	void  SetValue(const char* szName, float flValue);
private:
	cvar_t* m_back;
	cvar_t* m_begin;

	void FunctionToC();
};
extern CSCVars* GCVars;