#include "CSEngine.hpp"


CSCVars* GCVars = 0;

CSCVars::CSCVars()
{
	m_begin = 0; m_back = 0;
	FunctionToC();
}

CSCVars::~CSCVars()
{
	if (!m_begin)return;
	auto next = m_begin->next;
	while (next)
	{
		bool need_free = !(next->flags | FCVAR_NOFREE);
		if(need_free)
		bear_free(next->name);
		if (need_free&&next->string)bear_free(next->string);
		next = next->next;
		if (need_free)	bear_free(next);
	}
}

void CSCVars::RegisterVariable(cvar_t* item)
{
	item->flags |= FCVAR_NOFREE;
	if (m_back)
		m_back->next = item;
	m_back = item;
	if (!m_begin)m_begin = m_back;
}

cvar_t* CSCVars::GetPointer(const char* szName)
{
	auto next = m_begin->next;
	while (next)
	{
		if (BearString::Compare(next->name, szName) == 0)
		{
			return next;
		}
		next = next->next;
	}
	return 0;
}

cvar_t * CSCVars::RegisterVariable(const char * szName, const char * szValue, int flags)
{
	cvar_t *item= bear_new<cvar_t>();
	item->name = BearString::Duplicate(szName);
	item->string = szValue?BearString::Duplicate(szValue):0;
	item->value = 0;
	item->next = 0;
	item->flags = flags;

	if (m_back)
		m_back->next = item;
	m_back = item;
	if (!m_begin)m_begin = m_back;
	return item;
}

float CSCVars::GetFloat(const char * szName)
{
	auto next = m_begin->next;
	while (next)
	{
		if (BearString::Compare(next->name, szName) == 0)
		{
			return next->value;
		}
		next = next->next;
	}
	return 0.0f;
}

char * CSCVars::GetString(const char * szName)
{
	auto next = m_begin->next;
	while (next)
	{
		if (BearString::Compare(next->name, szName) == 0)
		{
			return next->string;
		}
		next = next->next;
	}
	return "";
}

void CSCVars::SetString(const char * szName, const char * szValue)
{
	auto next = m_begin->next;
	while (next)
	{
		if (BearString::Compare(next->name, szName) == 0)
		{
			if ((!(next->flags | FCVAR_NOFREE))&&next->string)bear_free(next->string);
			next->string = BearString::Duplicate(szValue);
		}
		next = next->next;
	}
}

void CSCVars::SetValue(const char * szName, float flValue)
{
	auto next = m_begin->next;
	while (next)
	{
		if (BearString::Compare(next->name, szName) == 0)
		{
			if (next->string)next->value = flValue;
		}
		next = next->next;
	}
}


static float GetFloatExport(const char* szName)
{
	return GCVars->GetFloat(szName);
}
static char* GetStringExport(const char* szName)
{
	return GCVars->GetString(szName);
}
static void  SetStringExport(const char* szName, const char* szValue)
{
	GCVars->SetString(szName, szValue);
}
static void  SetValueExport(const char* szName, float flValue)
{
	GCVars->SetValue(szName, flValue);
}
static cvar_t* RegisterVariableExport1(const char* szName, const char* szValue, int flags)
{
	return GCVars->RegisterVariable(szName, szValue, flags);
}
static void RegisterVariableExport2(cvar_t*var)
{
	return GCVars->RegisterVariable(var);
}
static cvar_t* GetPointerExport(const char* szName)
{
	return GCVars->GetPointer(szName);
}
void CSCVars::FunctionToC()
{
	GMenuEngineFunctions.RegisterVariable = RegisterVariableExport1;
	GMenuEngineFunctions.CvarSetString = SetStringExport;
	GMenuEngineFunctions.CvarSetValue = SetValueExport;
	GMenuEngineFunctions.GetCvarFloat = GetFloatExport;
	GMenuEngineFunctions.GetCvarString = GetStringExport;

	GServerEngineFunction.CVarRegister = RegisterVariableExport2;
	GServerEngineFunction.RegisterVariable = RegisterVariableExport2;
	GServerEngineFunction.CVarSetString = SetStringExport;
	GServerEngineFunction.CVarSetFloat = SetValueExport;
	GServerEngineFunction.CVarGetFloat = GetFloatExport;
	GServerEngineFunction.CVarGetString = GetStringExport;
	GServerEngineFunction.CVarGetPointer = GetPointerExport;
}
