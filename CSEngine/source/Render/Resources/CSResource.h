#pragma once
class CSResource
{
public:
	CSResource() { Count = 0; UniqueNumber = -1; }
	inline ~CSResource() { BEAR_ASSERT(Count==0); }
	bsize Count;
	int32 UniqueNumber;
};
class CSResourceNamed:public CSResource
{
public:
	CSResourceNamed(BearStringConteniar Name):m_Name(Name) {  }
	inline ~CSResourceNamed() {  }
	inline BearStringConteniar GetName()const 
	{
		return m_Name;
	}
private:
	BearStringConteniar m_Name;
};