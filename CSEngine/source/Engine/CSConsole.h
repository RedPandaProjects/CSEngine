#pragma once
class CSConsole
{
	BEAR_CLASS_WITHOUT_COPY(CSConsole);
public:
	CSConsole();
	~CSConsole();
	int     AddCommand(const char* cmd_name, void(*function)(void));
	void    ClientCmd(int execute_now, const char* szCmdString);
	void    ClientCmd(const char* szCmdString);
	void    DeleteCommand(const char* cmd_name);
	int     CmdArgc(void);
	char* CmdArgv(int argc);
	char* CmdArgs(void);
	void    Execute();
private:
	BearMap<BearStringConteniarAnsi, void(*)(void)> m_commands;
	BearVector< BearStringConteniarAnsi> m_stack;
	bsize m_argc;
	bchar8* m_args;
	bchar8* m_argv[64];
	void FunctionToC();
};
extern CSConsole* GConsole;