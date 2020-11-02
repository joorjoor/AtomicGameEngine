//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Poco/Exception.h>
#include <Poco/Environment.h>


#include <Atomic/IO/Log.h>
#include <Atomic/IO/FileSystem.h>

#include "../ToolEvents.h"
#include "../ToolSystem.h"
#include "../ToolEnvironment.h"
#include "../Subprocess/SubprocessSystem.h"
#include "../Project/Project.h"
#include "../Project/ProjectSettings.h"

#include "NETProjectSystem.h"
#include "NETProjectGen.h"
#include "NETBuildSystem.h"

#ifdef ATOMIC_PLATFORM_WINDOWS
#include <Poco/WinRegistryKey.h>
#endif

namespace ToolCore
{
    NETBuild::NETBuild(Context* context, const String& solutionPath) :
        Object(context),
        solutionPath_(solutionPath),
        status_(NETBUILD_PENDING)
    {

    }


    NETBuild::NETBuild(Context* context, const String& solutionPath, const StringVector& platforms, const StringVector& configurations) :
        Object(context),
        solutionPath_(solutionPath),
        status_(NETBUILD_PENDING)
    {
        for (unsigned i = 0; i < platforms.Size() ; i++)
        {
            platforms_.Push(platforms[i].ToLower());
        }

        for (unsigned i = 0; i < configurations.Size() ; i++)
        {
            String config = configurations[i];
            config.Replace("release", "Release");
            config.Replace("debug", "Debug");
            configurations_.Push(config);
        }
    }

    NETBuildSystem::NETBuildSystem(Context* context) :
        Object(context),
        verbose_(false)
    {
        SubscribeToEvent(E_TOOLUPDATE, ATOMIC_HANDLER(NETBuildSystem, HandleToolUpdate));
        SubscribeToEvent(E_NETBUILDATOMICPROJECT, ATOMIC_HANDLER(NETBuildSystem, HandleBuildAtomicProject));
    }

    NETBuildSystem::~NETBuildSystem()
    {

    }

    void NETBuildSystem::HandleSubprocessOutput(StringHash eventType, VariantMap& eventData)
    {
        if (curBuild_.Null())
        {
            ATOMIC_LOGERRORF("NETBuildSystem::HandleSubprocessOutput - output received without current build");
            return;
        }

        const String& text = eventData[SubprocessOutput::P_TEXT].GetString();

        if (verbose_)
            ATOMIC_LOGINFOF(text.CString());

        curBuild_->output_ += text;

    }

    void NETBuildSystem::HandleCompileProcessComplete(StringHash eventType, VariantMap& eventData)
    {
        UnsubscribeFromEvent(E_SUBPROCESSCOMPLETE);
        UnsubscribeFromEvent(E_SUBPROCESSOUTPUT);

        if (curBuild_.Null())
        {
            ATOMIC_LOGERROR("NETBuildSystem::HandleCompileProcessComplete - called with no current build");
            return;
        }
        curBuild_->status_ = NETBUILD_COMPLETE;

        int code = eventData[SubprocessComplete::P_RETCODE].GetInt();

        using namespace NETBuildResult;
        VariantMap buildEventData;

        buildEventData[P_BUILD] = curBuild_;

        bool success = true;
        String errorMsg;

        if (verbose_)
        {
            ATOMIC_LOGINFOF("AtomicNET Build Command: %s", curBuild_->allArgs_.CString());
        }

        if (!code)
        {

        }
        else
        {
            success = false;
            errorMsg = curBuild_->output_;
            errorMsg += ToString("\nCompilation Command: %s", curBuild_->allArgs_.CString());
        }

        buildEventData[P_SUCCESS] = success;

        if (!success)
        {
            buildEventData[P_ERRORTEXT] = errorMsg;
        }

        curBuild_->SendEvent(E_NETBUILDRESULT, buildEventData);

        curBuild_ = nullptr;
    }

    void NETBuildSystem::CurrentBuildError(String errorText)
    {
        if (curBuild_.Null())
        {
            ATOMIC_LOGERRORF("NETBuildSystem::CurrentBuildError - Error %s with no current build", errorText.CString());
            return;
        }

        using namespace NETBuildResult;
        VariantMap buildEventData;

        buildEventData[P_BUILD] = curBuild_;
        buildEventData[P_SUCCESS] = false;
        buildEventData[P_ERRORTEXT] = errorText;
        curBuild_->SendEvent(E_NETBUILDRESULT, buildEventData);
        curBuild_ = nullptr;

    }

    void NETBuildSystem::HandleToolUpdate(StringHash eventType, VariantMap& eventData)
    {
        if (curBuild_.Null() && !builds_.Size())
            return;

        if (curBuild_.Null())
        {
            // kick off a new build

            curBuild_ = builds_.Front();
            builds_.PopFront();


            FileSystem* fileSystem = GetSubsystem<FileSystem>();

            // Ensure solution still exists
            if (!fileSystem->FileExists(curBuild_->solutionPath_))
            {
                CurrentBuildError(ToString("Solution does not exist(%s)", curBuild_->solutionPath_.CString()));
                return;
            }

            String solutionPath = curBuild_->solutionPath_;

            String ext = GetExtension(solutionPath);

            bool requiresNuGet = true;

            if (ext == ".sln")
            {
                // TODO: handle projects that require nuget
                requiresNuGet = false;

                if (!fileSystem->FileExists(solutionPath))
                {
                    CurrentBuildError(ToString("Generated solution does not exist (%s : %s)", curBuild_->solutionPath_.CString(), solutionPath.CString()));
                    return;
                }

            }
            else if (ext == ".json")
            {
                SharedPtr<NETProjectGen> gen(new NETProjectGen(context_));

                gen->SetSupportedPlatforms(curBuild_->platforms_);
                gen->SetRewriteSolution(true);

                if (!gen->LoadJSONProject(solutionPath))
                {
                    CurrentBuildError(ToString("Error loading project (%s)", solutionPath.CString()));
                    return;
                }

                if (!gen->Generate())
                {
                    CurrentBuildError(ToString("Error generating project (%s)", solutionPath.CString()));
                    return;
                }

                solutionPath = gen->GetSolution()->GetOutputFilename();
                requiresNuGet = gen->GetRequiresNuGet();

                if (!fileSystem->FileExists(solutionPath))
                {
                    CurrentBuildError(ToString("Generated solution does not exist (%s : %s)", curBuild_->solutionPath_.CString(), solutionPath.CString()));
                    return;
                }

            }

            ToolEnvironment* tenv = GetSubsystem<ToolEnvironment>();
            const String& nugetBinary = tenv->GetAtomicNETNuGetBinary();

            if (requiresNuGet && !fileSystem->FileExists(nugetBinary))
            {
                CurrentBuildError(ToString("NuGet binary is missing (%s)", nugetBinary.CString()));
                return;
            }

            StringVector stringVector;
            String platforms;
            StringVector processedPlatforms;
            String configs;

            for (unsigned i = 0; i < curBuild_->configurations_.Size(); i++)
            {
                stringVector.Push(ToString("/p:Configuration=%s", curBuild_->configurations_[i].CString()));
            }

            configs = String::Joined(stringVector, " ");
            stringVector.Clear();

            for (unsigned i = 0; i < curBuild_->platforms_.Size(); i++)
            {
                // map platform
                String platform = curBuild_->platforms_[i];

                if (platform == "windows" || platform == "macosx" || platform == "linux")
                {
                    ATOMIC_LOGINFOF("Platform \"%s\" mapped to \"desktop\"", platform.CString());
                    platform = "desktop";
                }

                if (processedPlatforms.Contains(platform))
                {
                    ATOMIC_LOGWARNINGF("Platform \"%s\" is duplicated, skipping", platform.CString());
                    continue;
                }

                processedPlatforms.Push(platform);

                if (platform == "desktop" || platform == "android")
                {
                    platform = "\"Any CPU\"";
                }
                else if (platform == "ios")
                {

                    platform = "\"Any CPU\"";
                    // TODO
                    // platform = "iPhone";
                }
                else
                {
                    ATOMIC_LOGERRORF("Unknown platform: %s, skipping", platform.CString());
                    continue;
                }

                platform = ToString("/p:Platform=%s", platform.CString());

                if (stringVector.Contains(platform))
                {
                    // This can happen when specifying Desktop + Android for example
                    continue;
                }

                stringVector.Push(platform);
            }

            platforms = String::Joined(stringVector, " ");
            stringVector.Clear();

            Vector<String> args;

#ifdef ATOMIC_PLATFORM_WINDOWS

            // VS2015
            String vs2015ToolsPath = Poco::Environment::get("VS140COMNTOOLS", "").c_str();

            // validate still installed

            String installCheck = vs2015ToolsPath;
            installCheck.Replace("Tools\\", "IDE\\devenv.exe");

            if (!fileSystem->FileExists(installCheck))
                vs2015ToolsPath.Clear();

            // VS2017
            String vs2017ToolsPath;
            Poco::WinRegistryKey regKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VS7", true);
            if (regKey.exists() && regKey.exists("15.0"))
                vs2017ToolsPath = regKey.getString("15.0").c_str();

            if (vs2017ToolsPath.Length())
            {
                vs2017ToolsPath += "Common7\\Tools\\";
            }
			
			// VS2019
            String vs2019ToolsPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\";

            // validate still installed

            installCheck = vs2019ToolsPath;
            installCheck.Replace("Tools\\", "IDE\\devenv.exe");

            if (!fileSystem->FileExists(installCheck))
                vs2019ToolsPath.Clear();
			
            String cmdToolsPath;
		
			if (vs2019ToolsPath.Length())
			{
				cmdToolsPath = vs2019ToolsPath;
			}
			else if (!vs2019ToolsPath.Length() || !vs2015ToolsPath.Length() || (toolVersion_ == "VS2017" && vs2017ToolsPath.Length()))
            {
                cmdToolsPath = vs2017ToolsPath;
            }
			else
            {
                cmdToolsPath = vs2015ToolsPath;
            }

            if (!cmdToolsPath.Length())
            {
                CurrentBuildError("VS140COMNTOOLS environment variable and VS2017 registry key not found, cannot proceed.\nOr VS2015, VS2017, VS2019 don't installed!");
                return;
            }

            if (!cmdToolsPath.EndsWith("\\"))
            {
                cmdToolsPath += "\\";
            }

            String msbuildcmd = ToString("%sVsMSBuildCmd.bat", cmdToolsPath.CString());

            String cmd = "cmd";

            args.Push("/A");
            args.Push("/C");

            // vcvars bat
            String compile = ToString("\"\"%s\" ", msbuildcmd.CString());

            if (requiresNuGet)
            {
                compile += ToString("&& \"%s\" restore \"%s\" ", nugetBinary.CString(), solutionPath.CString());
            }

            compile += ToString("&& msbuild \"%s\" %s %s", solutionPath.CString(), platforms.CString(), configs.CString());

            if (curBuild_->targets_.Size()) {

                StringVector targets;

                for (unsigned i = 0; i < curBuild_->targets_.Size(); i++)
                {
                    const char* tname = curBuild_->targets_[i].CString();
                    targets.Push(ToString("/t:\"%s:Rebuild\"", tname));
                }

                compile += " " + String::Joined(targets, " ");

            }

            // close out quote
            compile += "\"";

            args.Push(compile);

#else

            String compile;

            String cmd = "bash";
            args.Push("-c");

            String xbuildBinary = tenv->GetMonoExecutableDir() + "xbuild";

            if (requiresNuGet)
            {
#ifdef ATOMIC_PLATFORM_OSX
                compile += ToString("\"%s\" restore \"%s\" && ", nugetBinary.CString(), solutionPath.CString());
#else
                compile += ToString("mono \"%s\" restore \"%s\" && ", nugetBinary.CString(), solutionPath.CString());
#endif
            }

            compile += ToString("\"%s\" \"%s\" %s %s", xbuildBinary.CString(), solutionPath.CString(), platforms.CString(), configs.CString());

            if (curBuild_->targets_.Size()) {

                StringVector targets;

                for (unsigned i = 0; i < curBuild_->targets_.Size(); i++)
                {
                    const char* tname = curBuild_->targets_[i].CString();
                    targets.Push(ToString("%s:Rebuild", tname));
                }

                compile += " /target:\"" + String::Joined(targets, ";") + "\"";

            }

            args.Push(compile);

#endif

            curBuild_->allArgs_.Join(args, " ");

            SubprocessSystem* subs = GetSubsystem<SubprocessSystem>();
            Subprocess* subprocess = nullptr;

            ATOMIC_LOGINFOF("%s : %s", cmd.CString(), curBuild_->allArgs_.CString());

            try
            {
                subprocess = subs->Launch(cmd, args, "");
            }
            catch (Poco::SystemException)
            {
                subprocess = nullptr;
            }

            if (!subprocess)
            {
                CurrentBuildError(ToString("NETCompile::Compile - Unable to launch MSBuild subprocess\n%s", curBuild_->allArgs_.CString()));
                return;
            }

            VariantMap buildBeginEventData;
            buildBeginEventData[NETBuildBegin::P_BUILD] = curBuild_;
            SendEvent(E_NETBUILDBEGIN, buildBeginEventData);

            SubscribeToEvent(subprocess, E_SUBPROCESSCOMPLETE, ATOMIC_HANDLER(NETBuildSystem, HandleCompileProcessComplete));
            SubscribeToEvent(subprocess, E_SUBPROCESSOUTPUT, ATOMIC_HANDLER(NETBuildSystem, HandleSubprocessOutput));

            curBuild_->status_ = NETBUILD_BUILDING;

        }

    }


    NETBuild* NETBuildSystem::GetBuild(const String& solutionPath, const StringVector& platforms, const StringVector& configurations)
    {
        List<SharedPtr<NETBuild>>::ConstIterator itr = builds_.Begin();

        while (itr != builds_.End())
        {
            NETBuild* build = *itr;

            if (build->solutionPath_ == solutionPath && build->platforms_ == platforms && build->configurations_ == configurations)
                return build;

            itr++;
        }

        return nullptr;

    }

    NETBuild* NETBuildSystem::BuildAtomicProject(Project* project)
    {
        StringVector platforms;
        StringVector configurations;

        platforms.Push("desktop");

#ifdef ATOMIC_DEBUG
        configurations.Push("Debug");
#else
        configurations.Push("Release");
#endif

        AtomicNETCopyAssemblies(context_, project->GetProjectPath() + "AtomicNET/Lib/");

        String solutionPath = project->GetProjectPath() + "AtomicNET/Solution/" + project->GetProjectSettings()->GetName() + ".sln";

        NETBuild* build = Build(solutionPath, platforms, configurations);

        if (build)
        {
            ProjectSettings* settings = project->GetProjectSettings();

            // This path is currently only hit when refreshing for desktop
            if (settings->GetSupportsAndroid() || settings->GetSupportsIOS())
            {
                // Build the PCL, which will get copied to Resources
                build->targets_.Push(project->GetProjectSettings()->GetName());

                // Build the Desktop executable, so we can run it
                // IMPORTANT NOTE: msbuild requires replacing '.' with '_' when in project name
                build->targets_.Push(project->GetProjectSettings()->GetName() + "_Desktop");
            }

            build->project_ = project;

        }

        ATOMIC_LOGINFOF("Received build for project %s", project->GetProjectFilePath().CString());

        return build;

    }


    void NETBuildSystem::HandleBuildAtomicProject(StringHash eventType, VariantMap& eventData)
    {
        using namespace NETBuildAtomicProject;

        Project* project = static_cast<Project*>(eventData[P_PROJECT].GetPtr());

        if (!project)
        {
            ATOMIC_LOGERROR("NETBuildSystem::HandleBuildAtomicProject - null project");
            return;
        }

        BuildAtomicProject(project);

    }

    NETBuild* NETBuildSystem::Build(const String& solutionPath, const StringVector& platforms, const StringVector& configurations)
    {

        FileSystem* fileSystem = GetSubsystem<FileSystem>();

        if (!fileSystem->FileExists(solutionPath))
        {
            ATOMIC_LOGERRORF("NETBuildSystem::Build - Solution does not exist (%s)", solutionPath.CString());
            return 0;
        }

        // Get existing build
        SharedPtr<NETBuild> build(GetBuild(solutionPath, platforms, configurations));

        if (build.NotNull())
            return build;

        // Create a new build
        build = new NETBuild(context_, solutionPath, platforms, configurations);

        builds_.Push(build);

        return build;
    }


}
