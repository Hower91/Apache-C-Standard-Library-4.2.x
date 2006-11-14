//
// $Id$
//
// projects.js - Definitions of the solution projects
//
//////////////////////////////////////////////////////////////////////
//
// Licensed to the Apache Software  Foundation (ASF) under one or more
// contributor  license agreements.  See  the NOTICE  file distributed
// with  this  work  for  additional information  regarding  copyright
// ownership.   The ASF  licenses this  file to  you under  the Apache
// License, Version  2.0 (the  "License"); you may  not use  this file
// except in  compliance with the License.   You may obtain  a copy of
// the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the  License is distributed on an  "AS IS" BASIS,
// WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
// implied.   See  the License  for  the  specific language  governing
// permissions and limitations under the License.
// 
//////////////////////////////////////////////////////////////////////

var commonDefines = "_RWSTD_USE_CONFIG;debug?_RWSTDDEBUG;dll?_RWSHARED";
var commonIncludes = "$(SolutionDir)%CONFIG%\\include";
var stdlibIncludes = "%SRCDIR%\\include;%SRCDIR%\\include\\ansi;" + commonIncludes;
var rwtestIncludes = "%SRCDIR%\\tests\\include;" + stdlibIncludes;
var commonLibs = "kernel32.lib user32.lib";

var ProjectsDir = "%BUILDDIR%\\Projects";

// projects which requires RTTI support
var RTTIProjects = new Array("18.exception");

var rxExcludedFolders = 
    new RegExp("^(?:\\.svn|Release.*|Debug.*|in|out|CVS)$","i");

// fill and return array of ProjectDef objects
// with definitions of the solution projects
// copyDll - if true then stdlib and rwtest dlls will be copied
//           to the target folder after build
// buildLocales - if true then generate projects for build locales
// testLocales - if true then generate projects for test locales
function CreateProjectsDefs(copyDll, buildLocales, testLocales)
{
    var projectDefs = new Array();
 
 ///////////////////////////////////////////////////////////////////////////////   
    var configureDef = new ProjectDef(".configure", typeGeneric);
    configureDef.VCProjDir = ProjectsDir;
    configureDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, ".cpp;.c;.cxx;.s;.cc", eFileTypeCppCode, false).
            addFilesByMask("%SRCDIR%\\etc\\config\\src", rxExcludedFolders, null));
    configureDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, ".h;.hpp;.hxx", eFileTypeCppHeader, false).
            addFilesByMask("%SRCDIR%\\etc\\config\\src", rxExcludedFolders, null));
    configureDef.FilterDefs.push(
        new FilterDef("Script Files", null, ".js;.wsf", eFileTypeScript, false).
            addFiles("%SRCDIR%\\etc\\config\\windows",
                new Array("configure.wsf", "config.js", "data.js", "utilities.js")));
    configureDef.OutDir = "$(SolutionDir)%CONFIG%\\include";
    configureDef.IntDir = configureDef.OutDir;
    configureDef.CustomBuildFile = "configure.wsf";
    configureDef.CustomBuildCmd = "cscript /nologo \"%CUSTOMFILE%\"" +
        " /SolutionName:\"%SOLUTION%\"" +
        " /ConfigurationName:\"%CFGNAME%\"" +
        " /SrcDir:\"%SRCDIR%\\etc\\config\\src\"" +
        " /IncludeDir:\"%SRCDIR%\\include\"" +
        " /OutDir:\"$(OutDir)\"" +
        " /OutFile:\"$(OutDir)\\config.h\"" +
        " /LogFile:\"$(OutDir)\\config.log\"";
    configureDef.CustomBuildOut = "$(OutDir)\\config.h";
    configureDef.CustomBuildDeps = "%FILES%";

    projectDefs.push(configureDef);

///////////////////////////////////////////////////////////////////////////////   
    var stdlibDef = new ProjectDef(".stdlib", typeLibrary);
    stdlibDef.VCProjDir = ProjectsDir;
    stdlibDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, sourceFilterExts, eFileTypeCppCode, false).
            addFilesByMask("%SRCDIR%\\src", rxExcludedFolders, null));
    stdlibDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
            addFilesByMask("%SRCDIR%\\src", rxExcludedFolders, null).
            addFilter(new FilterDef("Include", headerFilterUuid, headerFilterExts + ";.", eFileTypeCppHeader, true).
                addFilesByMask("%SRCDIR%\\include", rxExcludedFolders, null)));
    stdlibDef.Defines = commonDefines;
    stdlibDef.Includes = stdlibIncludes;
    stdlibDef.OutDir = "$(SolutionDir)lib";
    stdlibDef.IntDir = "$(SolutionDir)%CONFIG%\\src";
    stdlibDef.Libs = commonLibs;
    stdlibDef.OutFile = "$(OutDir)\\stdlib%CONFIG%%EXT%";
    stdlibDef.PrjDeps.push(configureDef);

    projectDefs.push(stdlibDef);

///////////////////////////////////////////////////////////////////////////////   
    var rwtestDef = new ProjectDef(".rwtest", typeLibrary);
    rwtestDef.VCProjDir = ProjectsDir;
    rwtestDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, sourceFilterExts, eFileTypeCppCode, false).
            addFilesByMask("%SRCDIR%\\tests\\src", rxExcludedFolders, null));
    rwtestDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
            addFilesByMask("%SRCDIR%\\tests\\src", rxExcludedFolders, null).
            addFilter(new FilterDef("Include", headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
                addFilesByMask("%SRCDIR%\\tests\\include", rxExcludedFolders, null)));
    rwtestDef.Defines = commonDefines;
    rwtestDef.Includes = rwtestIncludes;
    rwtestDef.OutDir = "$(SolutionDir)%CONFIG%\\tests";
    rwtestDef.IntDir = rwtestDef.OutDir + "\\src";
    rwtestDef.PrjRefs.push(stdlibDef);

    projectDefs.push(rwtestDef);

///////////////////////////////////////////////////////////////////////////////   
    var execDef = new ProjectDef("util_exec", typeApplication);
    execDef.VCProjDir = ProjectsDir + "\\util";
    execDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, sourceFilterExts, eFileTypeCppCode, false).
            addFiles("%SRCDIR%\\util",
                new Array("cmdopt.cpp", "display.cpp", "exec.cpp", "output.cpp", "runall.cpp", "util.cpp")));
    execDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
            addFiles("%SRCDIR%\\util",
                new Array("cmdopt.h", "display.h", "exec.h", "output.h", "target.h", "util.h")));
    execDef.Defines = "_RWSTD_USE_CONFIG";
    execDef.Includes = commonIncludes;
    execDef.OutDir = "$(SolutionDir)%CONFIG%\\bin";
    execDef.Libs = commonLibs;
    execDef.OutFile = "$(OutDir)\\exec.exe";
    execDef.PrjDeps.push(configureDef);

    projectDefs.push(execDef);

///////////////////////////////////////////////////////////////////////////////   
    var localedefDef = new ProjectDef("util_localedef", typeApplication);
    localedefDef.VCProjDir = ProjectsDir + "\\util";
    localedefDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, sourceFilterExts, eFileTypeCppCode, false).
            addFiles("%SRCDIR%\\util",
                new Array("aliases.cpp", "charmap.cpp", "codecvt.cpp",
                          "collate.cpp", "ctype.cpp", "def.cpp",
                          "diagnostic.cpp", "localedef.cpp", "messages.cpp",
                          "monetary.cpp", "numeric.cpp", "path.cpp",
                          "scanner.cpp", "time.cpp")));
    localedefDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
            addFiles("%SRCDIR%\\util",
                new Array("aliases.h", "charmap.h", "def.h", "diagnostic.h",
                "loc_exception.h", "localedef.h", "path.h", "scanner.h")));
    localedefDef.Defines = commonDefines;
    localedefDef.Includes = stdlibIncludes;
    localedefDef.OutDir = "$(SolutionDir)%CONFIG%\\bin";
    localedefDef.Libs = commonLibs;
    localedefDef.OutFile = "$(OutDir)\\localedef.exe";
    localedefDef.PrjRefs.push(stdlibDef);

    projectDefs.push(localedefDef);

///////////////////////////////////////////////////////////////////////////////   
    var localeDef = new ProjectDef("util_locale", typeApplication);
    localeDef.VCProjDir = ProjectsDir + "\\util";
    localeDef.FilterDefs.push(
        new FilterDef(sourceFilterName, sourceFilterUuid, sourceFilterExts, eFileTypeCppCode, false).
            addFiles("%SRCDIR%\\util",
                new Array("aliases.cpp", "charmap.cpp", "codecvt.cpp",
                          "collate.cpp", "ctype.cpp", "def.cpp",
                          "diagnostic.cpp", "locale.cpp", "memchk.cpp",
                          "messages.cpp", "monetary.cpp", "numeric.cpp",
                          "path.cpp", "scanner.cpp", "time.cpp")));
    localeDef.FilterDefs.push(
        new FilterDef(headerFilterName, headerFilterUuid, headerFilterExts, eFileTypeCppHeader, true).
            addFiles("%SRCDIR%\\util",
                new Array("aliases.h", "charmap.h", "def.h", "diagnostic.h",
                "loc_exception.h", "memchk.h", "path.h", "scanner.h")));
    localeDef.Defines = commonDefines;
    localeDef.Includes = stdlibIncludes;
    localeDef.OutDir = "$(SolutionDir)%CONFIG%\\bin";
    localeDef.Libs = commonLibs;
    localeDef.OutFile = "$(OutDir)\\locale.exe";
    localeDef.PrjRefs.push(stdlibDef);

    projectDefs.push(localeDef);

///////////////////////////////////////////////////////////////////////////////   
    var utilsDef = new ProjectDef(".stdlib_utils", typeGeneric);
    utilsDef.VCProjDir = ProjectsDir + "\\util";
    utilsDef.OutDir = "$(SolutionDir)%CONFIG%\\bin";
    utilsDef.IntDir = utilsDef.OutDir;
    utilsDef.PrjDeps.push(execDef);
    utilsDef.PrjDeps.push(localedefDef);
    utilsDef.PrjDeps.push(localeDef);

    projectDefs.push(utilsDef);

///////////////////////////////////////////////////////////////////////////////   
    var exampleTplDef = new ProjectDef(null, typeApplication);
    exampleTplDef.VCProjDir = ProjectsDir + "\\examples";
    exampleTplDef.Defines = commonDefines;
    exampleTplDef.Includes = "%SRCDIR%\\examples\\include;" + stdlibIncludes;
    exampleTplDef.OutDir = "$(SolutionDir)%CONFIG%\\examples";
    exampleTplDef.Libs = commonLibs;
    exampleTplDef.PrjRefs.push(stdlibDef);
    
    var exampleDefs = exampleTplDef.createProjectDefsFromFolder(
        "%SRCDIR%\\examples",
        new RegExp("^.+\\.(?:cpp)$", "i"),
        new RegExp("^(?:\\.svn|Release.*|Debug.*|in|out|CVS)$", "i"),
        new RegExp("^(?:rwstdmessages.cpp)$", "i"));

    projectDefs = projectDefs.concat(exampleDefs);
    
///////////////////////////////////////////////////////////////////////////////   
    var runexamplesDef = new ProjectDef(".stdlib_examples", typeGeneric);
    runexamplesDef.VCProjDir = ProjectsDir + "\\examples";
    runexamplesDef.FilterDefs.push(
        new FilterDef("Script Files", null, ".js;.wsf", eFileTypeScript, false).
            addFiles("%SRCDIR%\\etc\\config\\windows",
                new Array("runall.wsf", "config.js", "utilities.js",
                          "summary.js")));
    runexamplesDef.OutDir = "$(SolutionDir)%CONFIG%\\examples";
    runexamplesDef.IntDir = runexamplesDef.OutDir;
    runexamplesDef.PreBuildCmd =
        "if exist \"$(OutDir)\\summary.htm del\" \"$(OutDir)\\summary.htm\"\r\n" +
        "if exist \"$(OutDir)\\runexamples.log\" del \"$(OutDir)\\runexamples.log\"";
    runexamplesDef.CustomBuildFile = "runall.wsf";
    runexamplesDef.CustomBuildCmd = "cscript /nologo \"%CUSTOMFILE%\"" +
        " /INOUTDIR:\"%SRCDIR%\\examples\"" +
        " /EXEDIR:\"$(OutDir)\"" +
        " /BUILDTYPE:\"%CONFIG%\"" +
        " /CONFIG:\"%SOLUTION%\"" +
        " /LOGFILE:\"runexamples.log\"" +
        " /COPYDLL:" + (copyDll ? "false" : "true") +
        " /LIBDIR:\"$(SolutionDir)lib\"";
    runexamplesDef.CustomBuildOut = "$(OutDir)\\runexamples.log";
    runexamplesDef.CustomBuildDeps = "%FILES%";
    runexamplesDef.PrjDeps = exampleDefs;
    runexamplesDef.PrjDeps.push(execDef);

    projectDefs.push(runexamplesDef);

///////////////////////////////////////////////////////////////////////////////   
    var testTplDef = new ProjectDef(null, typeApplication);
    testTplDef.VCProjDir = ProjectsDir + "\\tests";
    testTplDef.Defines = commonDefines;
    testTplDef.Includes = rwtestIncludes;
    testTplDef.OutDir = "$(SolutionDir)%CONFIG%\\tests";
    testTplDef.Libs = commonLibs;
    testTplDef.PrjRefs.push(stdlibDef);
    testTplDef.PrjRefs.push(rwtestDef);
    
    var testDefs = testTplDef.createProjectDefsFromFolder(
        "%SRCDIR%\\tests",
        new RegExp("^.+\\.(?:cpp)$", "i"),
        new RegExp("^(?:\\.svn|Release.*|Debug.*|in|out|CVS|src|include)$","i"),
        new RegExp("^(?:rwstdmessages.cpp)$","i"));

    projectDefs = projectDefs.concat(testDefs);
    
///////////////////////////////////////////////////////////////////////////////   
    var runtestsDef = new ProjectDef(".stdlib_tests", typeGeneric);
    runtestsDef.VCProjDir = ProjectsDir + "\\tests";
    runtestsDef.FilterDefs.push(
        new FilterDef("Script Files", null, ".js;.wsf", eFileTypeScript, false).
            addFiles("%SRCDIR%\\etc\\config\\windows",
                new Array("runall.wsf", "config.js", "utilities.js",
                          "summary.js")));
    runtestsDef.OutDir = "$(SolutionDir)%CONFIG%\\tests";
    runtestsDef.IntDir = runtestsDef.OutDir;
    runtestsDef.PreBuildCmd =
        "if exist \"$(OutDir)\\summary.htm\" del \"$(OutDir)\\summary.htm\"\r\n" +
        "if exist \"$(OutDir)\\runtests.log\" del \"$(OutDir)\\runtests.log\"";
    runtestsDef.CustomBuildFile = "runall.wsf";
    runtestsDef.CustomBuildCmd = "cscript /nologo \"%CUSTOMFILE%\"" +
        " /INOUTDIR:\"%SRCDIR%\\tests\"" +
        " /EXEDIR:\"$(OutDir)\"" +
        " /BUILDTYPE:\"%CONFIG%\"" +
        " /CONFIG:\"%SOLUTION%\"" +
        " /LOGFILE:\"runtests.log\"" +
        " /COPYDLL:" + (copyDll ? "false" : "true") +
        " /COPYRWTESTDLL:" + (copyDll ? "false" : "true") +
        " /LIBDIR:\"$(SolutionDir)lib\"";
    runtestsDef.CustomBuildOut = "$(OutDir)\\runtests.log";
    runtestsDef.CustomBuildDeps = "%FILES%";
    runtestsDef.PrjDeps = testDefs;
    runtestsDef.PrjDeps.push(execDef);

    projectDefs.push(runtestsDef);

///////////////////////////////////////////////////////////////////////////////   
    if (buildLocales)
    {
        var localeTplDef = new ProjectDef(null, typeGeneric);
        localeTplDef.VCProjDir = ProjectsDir + "\\locales";
        localeTplDef.OutDir = "$(SolutionDir)nls";
        localeTplDef.IntDir = localeTplDef.OutDir + "\\Build\\$(ProjectName)";
        localeTplDef.CustomBuildDeps = "%FILES%";
        localeTplDef.PrjDeps.push(localedefDef);
        if (!copyDll)
        {
            // copy stdlibxxx.dll to the bin directory
            // before executing localedef.exe utility
            // and finally delete the copied file
            var libname = "stdlib%CONFIG%.dll";
            var src = "\"$(SolutionDir)lib\\" + libname + "\"";
            var dst = "\"$(SolutionDir)%CONFIG%\\bin\\" + libname + "\"";
            localeTplDef.PreBuildCmd = "if exist " + src + " if not exist " + dst +
                                       " copy /Y " + src + " " + dst;
            localeTplDef.PostBuildCmd = "if exist " + dst + " del " + dst;
        }
    
        var localeDefs = localeTplDef.createLocaleDefs("%SRCDIR%\\etc\\nls");
    
        projectDefs = projectDefs.concat(localeDefs);
        
        var localesDef = new ProjectDef(".stdlib_locales", typeGeneric);
        localesDef.VCProjDir = ProjectsDir + "\\locales";
        localesDef.OutDir = "$(SolutionDir)nls";
        localesDef.IntDir = localesDef.OutDir;
        localesDef.PrjDeps = localeDefs;
    
        projectDefs.push(localesDef);
    }

///////////////////////////////////////////////////////////////////////////////
    if (testLocales)
    {
        var testlocaleTplDef = new ProjectDef(null, typeGeneric);
        testlocaleTplDef.VCProjDir = ProjectsDir + "\\locales";
        testlocaleTplDef.OutDir = "$(SolutionDir)bin";
        testlocaleTplDef.IntDir = testlocaleTplDef.OutDir + "\\Build\\$(ProjectName)";
        testlocaleTplDef.CustomBuildDeps = "%FILES%";
        testlocaleTplDef.PrjDeps.push(localeDef);
        testlocaleTplDef.PrjDeps.push(localedefDef);
        if (!copyDll)
        {
            // copy stdlibxxx.dll to the bin directory
            // before executing run_locale_utils.wsf script
            // and finally delete the copied file
            var libname = "stdlib%CONFIG%.dll";
            var src = "\"$(SolutionDir)lib\\" + libname + "\"";
            var dst = "\"$(SolutionDir)%CONFIG%\\bin\\" + libname + "\"";
            testlocaleTplDef.PreBuildCmd = "if exist " + src + " if not exist " + dst +
                                           " copy /Y " + src + " " + dst;
            testlocaleTplDef.PostBuildCmd = "if exist " + dst + " del " + dst;
        }
        
        var testlocaleDefs = testlocaleTplDef.createTestLocaleDefs("%SRCDIR%\\etc\\nls");
    
        projectDefs = projectDefs.concat(testlocaleDefs);
        
        var testlocalesDef = new ProjectDef(".stdlib_testlocales", typeGeneric);
        testlocalesDef.VCProjDir = ProjectsDir + "\\locales";
        testlocalesDef.OutDir = "$(SolutionDir)bin";
        testlocalesDef.IntDir = testlocalesDef.OutDir;
        testlocalesDef.PrjDeps = testlocaleDefs;
    
        projectDefs.push(testlocalesDef);
    }
    
///////////////////////////////////////////////////////////////////////////////   
    if (copyDll)
    {
        // if project type is application and
        //   if it depends on stdlib project then
        //     copy stdlibxxx.dll to project output directory
        //   if it depends on rwtest project then
        //     copy rwtest.dll to project output directory
        for (var i = 0; i < projectDefs.length; ++i)
        {
            var projectDef = projectDefs[i];

            if (projectDef.Type != typeApplication)
                continue;

            var arrDeps = projectDef.PrjRefs.concat(projectDef.PrjDeps);
            var command = "";

            if (0 <= arrayIndexOf(arrDeps, stdlibDef))
            {
                var libname = "stdlib%CONFIG%.dll";
                var src = "\"$(SolutionDir)lib\\" + libname + "\"";
                var dst = "\"$(OutDir)\\" + libname + "\"";
                var cmd = "if exist " + src + " (\r\n" +
                          "del " + dst + "\r\n" +
                          "copy /Y " + src + " " + dst + "\r\n" +
                          ")";
                if (0 == command.length)
                    command = cmd;
                else
                    command += "\r\n" + cmd;
            }

            if (0 <= arrayIndexOf(arrDeps, rwtestDef))
            {
                var libname = "rwtest.dll";
                var src = "\"$(SolutionDir)%CONFIG%\\tests\\" + libname + "\"";
                var dst = "\"$(OutDir)\\" + libname + "\"";
                var cmd = "if exist " + src + " (\r\n" +
                          "del " + dst + "\r\n" +
                          "copy /Y " + src + " " + dst + "\r\n" +
                          ")";
                if (0 == command.length)
                    command = cmd;
                else
                    command += "\r\n" + cmd;
            }

            if (null == projectDef.PostBuildCmd || "" == projectDef.PostBuildCmd)
                projectDef.PostBuildCmd = command;
            else
                projectDef.PostBuildCmd = command + "\r\n" + projectDef.PostBuildCmd;
        }
    }

    return projectDefs;
}

// create VCProject's using ProjectDed definitions
// projectDefs - array with project definitions
// report - callback function to report progress
function CreateProjects(projectDefs, report)
{
    for (var i = 0; i < projectDefs.length; ++i)
    {
        var projectDef = projectDefs[i];

        // turn on RTTI support if project in RTTIProjects array
        if (0 <= arrayIndexOf(RTTIProjects, projectDef.Name))
            projectDef.RTTI = true;

        projectDef.createVCProject(VCProjectEngine, report);
    }
}

// configure dependencies between projects
// (insert <References> section to the .vcproj file)
// projectDefs - array with project definitions
function ConfigureDependencies(projectDefs)
{
    for (var i = 0; i < projectDefs.length; ++i)
    {
        var projectDef = projectDefs[i];
        var VCProject = projectDef.VSProject;

        var prjrefs = projectDef.PrjRefs;
        if (0 == prjrefs.length)
            continue;

        var file = fso.OpenTextFile(VCProject.ProjectFile, 1, false);
        var text = file.ReadAll();
        file.Close();
        var refs = "";
        for (var j = 0; j < prjrefs.length; ++j)
        {
            refs += "\t\t<ProjectReference\n";
            refs += "\t\t\tReferencedProjectIdentifier=\"" +
                    prjrefs[j].VSProject.ProjectGUID + "\"\n";
            refs += "\t\t/>\n";
        }
        var pos = text.indexOf("\t</References>");
        if (0 > pos)
        {
            var str = "\t</Configurations>";
            pos = text.indexOf(str);
            if (0 <= pos)
            {
                refs = "\n\t<References>\n" + refs + "\t</References>";
                pos += str.length;
            }
        }
        text = text.substr(0, pos) + refs + text.substr(pos);
        text.replace("\t</References>", refs);
        file = fso.CreateTextFile(VCProject.ProjectFile, true, false);
        file.Write(text);
        file.Close();
    }
}