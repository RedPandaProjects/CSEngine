using BearBuildTool.Projects;
using System.IO;
using System;
public class CSMP :Project
{
	public CSMP(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include","common"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","engine"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","public"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","version"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","regamedll"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","game_shared"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","pm_shared"));
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
			
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
		
	Warning = false;
	Defines.Public.Add("REGAMEDLL_ADD");
	Defines.Public.Add("REGAMEDLL_API");
	Defines.Public.Add("REGAMEDLL_SSE");
	Defines.Public.Add("REGAMEDLL_FIXES");
	Defines.Public.Add("REGAMEDLL_SELF");
	Defines.Public.Add("UNICODE_FIXES");
	Defines.Public.Add("REGAMEDLL_CHECKS");
	Defines.Public.Add("CLIENT_WEAPONS");
	Defines.Public.Add("USE_BREAKPAD_HANDLER");
	Defines.Public.Add("USE_QSTRING");
	Defines.Public.Add("DEDICATED");
		
	}
} 