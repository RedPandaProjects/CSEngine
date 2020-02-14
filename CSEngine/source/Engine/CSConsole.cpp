#include "CSEngine.hpp"

CSConsole* GConsole = 0;

CSConsole::CSConsole()
{
	FunctionToC();
}

CSConsole::~CSConsole()
{
}

int CSConsole::AddCommand(const char * cmd_name, void(*function)(void))
{
	m_commands.insert(cmd_name, function);
	return 0;
}

void CSConsole::ClientCmd(int execute_now, const char * szCmdString)
{
	m_stack.push_back(szCmdString);
	if (execute_now)Execute();
}

void CSConsole::ClientCmd(const char * szCmdString)
{
	m_stack.push_back(szCmdString);
}

void CSConsole::DeleteCommand(const char * cmd_name)
{
	m_commands.erase(cmd_name);
}

int CSConsole::CmdArgc(void)
{
	return static_cast<int>(m_argc);
}

char * CSConsole::CmdArgv(int argc)
{
	return m_argv[argc];
}

char * CSConsole::CmdArgs(void)
{
	return m_args;
}

void CSConsole::Execute()
{
	auto b = m_stack.begin();
	auto e = m_stack.end();
	if (b != e)
	{
		bchar8*command = BearString::Duplicate(**b);
		bchar8* args = BearString::ToChar(command, ' ');
		if (args&&args[0])
		{
			*args = 0; 
			args++;
		}
		args = BearString::SubSpaceInBegin(args);
		
		if (args)
		{
			m_args = args;
			while (args[0])
			{
				args = BearString::ToChar(args, ' ');
				if (args&&args[0])
				{
					*args = 0;
					args++;
				}
				args=BearString::SubSpaceInBegin(args);
				if (args&&args[0])
				{
					m_argv[m_argc] = args;
					m_argc++;
				}
			}

		}
		else
		{
			m_args = "";
			m_argc = 0;
			m_argv[0] = args;
		}

	

		bear_free(command);
		b++;
	}
}


static int AddCommandExport(const char* cmd_name, void(*function)(void))
{
	return GConsole->AddCommand(cmd_name, function);
}
static void ClientCmdExport(int execute_now, const char* szCmdString)
{
	GConsole->ClientCmd(execute_now, szCmdString);
}
static void    DeleteCommandExport(const char* cmd_name)
{
	GConsole->DeleteCommand(cmd_name);
}
static int     CmdArgcExport(void)
{
	return GConsole->CmdArgc();
}
static char* CmdArgvExport(int argc)
{
	return GConsole->CmdArgv(argc);

}
static char* CmdArgsExport(void)
{
	return GConsole->CmdArgs();
}
void CSConsole::FunctionToC()
{
	FUIEngine.pfnClientCmd = ClientCmdExport;
	FUIEngine.pfnAddCommand = AddCommandExport;
	FUIEngine.pfnDelCommand = DeleteCommandExport;
	FUIEngine.pfnCmdArgc = CmdArgcExport;
	FUIEngine.pfnCmdArgv = CmdArgvExport;
	FUIEngine.pfnCmd_Args = CmdArgsExport;
}