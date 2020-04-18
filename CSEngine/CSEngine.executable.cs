using BearBuildTool.Projects;
using System.IO;
using System;
public class CSEngine :Executable
{
	public CSEngine(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		AddSourceFiles(Path.Combine(ProjectPath, "source"),true);
		PCHFile =Path.Combine(ProjectPath,"source","CSEnginePCH.cpp");
		PCHIncludeFile= "CSEngine.hpp";
		Projects.Public.Add("bear_core");
		Projects.Public.Add("bear_graphics");
		Defines.Private.Add("CSENGINE_EXPORTS");
		IncludeAutonomousProjects.Add("CSMenu");
		IncludeAutonomousProjects.Add("CSMP");
		if (BearBuildTool.Config.Global.DevVersion)
		{
			Projects.Private.Add("freetype");
		}
		Console = true;
    }
} 