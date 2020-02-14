using BearBuildTool.Projects;
using System.IO;
using System;
public class CSMenu :Project
{
	public CSMenu(string ProjectPath)
	{	
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","engine"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","common"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","pm_shared"));
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
		Defines.Public.Add("MAINUI_EXPORTS");
    }
} 