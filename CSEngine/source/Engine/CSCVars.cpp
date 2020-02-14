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
		bear_free(next->name);
		if (next->string)bear_free(next->string);
		next = next->next;
		bear_free(next);
	}
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
			if (next->string)bear_free(next->string);
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
static cvar_t* RegisterVariableExport(const char* szName, const char* szValue, int flags)
{
	return GCVars->RegisterVariable(szName, szValue, flags);
}
void CSCVars::FunctionToC()
{
	FUIEngine.pfnRegisterVariable = RegisterVariableExport;
	FUIEngine.pfnCvarSetString = SetStringExport;
	FUIEngine.pfnCvarSetValue = SetValueExport;
	FUIEngine.pfnGetCvarFloat = GetFloatExport;
	FUIEngine.pfnGetCvarString = GetStringExport;
}
