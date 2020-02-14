#pragma once

class CSCommonParser
{
	BEAR_CLASS_WITHOUT_COPY(CSCommonParser);
public:
	CSCommonParser();
	~CSCommonParser();
	 bchar* Load(const bchar* path, const bchar* file);
	 const bchar* NextToken(const bchar* buffer, BearString& token);
	  void Free(void* buffer);
private:
	 void FunctionToC();
};
extern CSCommonParser* GCommonParser;