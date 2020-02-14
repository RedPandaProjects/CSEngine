#include "CSEngine.hpp"
CSCommonParser* GCommonParser = 0;


CSCommonParser::CSCommonParser()
{
	FunctionToC();
}

CSCommonParser::~CSCommonParser()
{
}

bchar* CSCommonParser::Load(const bchar* path, const bchar* file)
{
	BearString string;
	auto fs = GFS->Read(path, file);
	fs->ToString(string, BearEncoding::ANSI);
	return BearString::Duplicate(*string);
}

const bchar * CSCommonParser::NextToken(const bchar * buffer, BearString & token)
{
	bchar	c;

	token.clear_no_free();


	if (!buffer||!buffer[0])
		return NULL;

skipwhite:
	buffer = BearString::SubSpaceInBegin(buffer);
	c = buffer[0];
	// skip // comments
	if (c == '/' && buffer[1] == '/')
	{
		while (*buffer && *buffer != '\n')
			buffer++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if (c == '\"')
	{
		buffer++;
		while (1)
		{
			c = (bchar)*buffer;

			// unexpected line end
			if (!c)
			{
				return buffer;
			}
			buffer++;

			if (c == '\\' && *buffer == '"')
			{
				token.append( 1, *buffer++);
				continue;
			}

			if (c == '\"')
			{
				return buffer;
			}
			token.append(1, c);
		}
	}

	// parse single characters
	if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',')
	{
		token.append(1, c);
		return buffer + 1;
	}

	// parse a regular word
	do
	{
		token.append( 1,c);
		buffer++;
		c = ((bchar)*buffer);

		if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',')
			break;
	} while (c > 32);
	return buffer;
}

void CSCommonParser::Free(void * buffer)
{
	bear_free(buffer);
}
#ifdef UNICODE
#define FILE_NAME  *BearEncoding::FastToUnicode(file_)
#else
#define FILE_NAME  file_
#endif 

static byte*LoadExport(const char* file_, int*length)
{

	if (!GFS->ExistFile(TEXT("%game%"), FILE_NAME))return 0;
	bchar*text = GCommonParser->Load(TEXT("%game%"), FILE_NAME);
	if (length)*length = static_cast<int>(BearString::GetSize(text));
	return reinterpret_cast<byte*>(text);
}
static char*NextTokenExport(char* file, char*token)
{
	BearString token_s;
	const bchar*text = GCommonParser->NextToken(reinterpret_cast<bchar*>(file), token_s);
	BearString::Copy(token, 256,
#ifdef UNICODE	
		*BearEncoding::FastToAnsi(*token_s)
#else
		*token_s
#endif
	);
	return const_cast<char*>( reinterpret_cast<const char*>(text));
}
static void FreeExport(void*buffer)
{
	GCommonParser->Free(buffer);
}
void CSCommonParser::FunctionToC()
{
	FUIEngine.COM_LoadFile = LoadExport;
	FUIEngine.COM_ParseFile = NextTokenExport;
	FUIEngine.COM_FreeFile = FreeExport;
}
