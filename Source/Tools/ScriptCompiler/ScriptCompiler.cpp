//
// Copyright (c) 2008-2020 the Urho3D project.
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

#include <Atomic/Atomic.h>

#include <Atomic/AngelScript/Script.h>
#include <Atomic/AngelScript/ScriptFile.h>
#include <Atomic/Core/Context.h>
#include <Atomic/Core/ProcessUtils.h>
#include <Atomic/Engine/Engine.h>
#include <Atomic/Engine/EngineDefs.h>
#include <Atomic/IO/File.h>
#include <Atomic/IO/FileSystem.h>
#include <Atomic/IO/Log.h>
#include <Atomic/Resource/ResourceCache.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <Atomic/DebugNew.h>

using namespace Atomic;

void CompileScript(Context* context, const String& fileName);

int main(int argc, char** argv)
{
    #ifdef WIN32
    const Vector<String>& arguments = ParseArguments(GetCommandLineW());
    #else
    const Vector<String>& arguments = ParseArguments(argc, argv);
    #endif

    bool dumpApiMode = false;
    String sourceTree;
    String outputFile;

    if (arguments.Size() < 1)
        ErrorExit("Usage: ScriptCompiler <input file> [resource path for includes]\n"
                  "       ScriptCompiler -dumpapi <source tree> <Doxygen output file> [C header output file]");
    else
    {
        if (arguments[0] != "-dumpapi")
            outputFile = arguments[0];
        else
        {
            dumpApiMode = true;
            if (arguments.Size() > 2)
            {
                sourceTree = arguments[1];
                outputFile = arguments[2];
            }
            else
                ErrorExit("Usage: ScriptCompiler -dumpapi <source tree> <Doxygen output file> [C header output file]");
        }
    }

    SharedPtr<Context> context(new Context());
    SharedPtr<Engine> engine(new Engine(context));
    context->RegisterSubsystem(new Script(context));

    // In API dumping mode initialize the engine and instantiate LuaScript system if available so that we
    // can dump attributes from as many classes as possible
    if (dumpApiMode)
    {
        VariantMap engineParameters;
        engineParameters[EP_HEADLESS] = true;
        engineParameters[EP_WORKER_THREADS] = false;
        engineParameters[EP_LOG_NAME] = String::EMPTY;
        engineParameters[EP_RESOURCE_PATHS] = String::EMPTY;
        engineParameters[EP_AUTOLOAD_PATHS] = String::EMPTY;
        engine->Initialize(engineParameters);
    }

    auto* log = context->GetSubsystem<Log>();
    // Register Log subsystem manually if compiled without logging support
    if (!log)
    {
        context->RegisterSubsystem(new Log(context));
        log = context->GetSubsystem<Log>();
    }

    log->SetLevel(LOG_WARNING);
    log->SetTimeStamp(false);

    if (!dumpApiMode)
    {
        String path, file, extension;
        SplitPath(outputFile, path, file, extension);

        auto* cache = context->GetSubsystem<ResourceCache>();

        // Add resource path to be able to resolve includes
        if (arguments.Size() > 1)
            cache->AddResourceDir(arguments[1]);
        else
            cache->AddResourceDir(cache->GetPreferredResourceDir(path));

        if (!file.StartsWith("*"))
            CompileScript(context, outputFile);
        else
        {
            Vector<String> scriptFiles;
            context->GetSubsystem<FileSystem>()->ScanDir(scriptFiles, path, file + extension, SCAN_FILES, false);
            for (unsigned i = 0; i < scriptFiles.Size(); ++i)
                CompileScript(context, path + scriptFiles[i]);
        }
    }
    else
    {
        if (!outputFile.Empty())
        {
            log->SetQuiet(true);
            log->Open(outputFile);
        }
        // If without output file, dump to stdout instead
        context->GetSubsystem<Script>()->DumpAPI(DOXYGEN, sourceTree);

        // Only dump API as C Header when an output file name is explicitly given
        if (arguments.Size() > 3)
        {
            outputFile = arguments[3];
            log->Open(outputFile);
            context->GetSubsystem<Script>()->DumpAPI(C_HEADER, sourceTree);
        }
    }

    return EXIT_SUCCESS;
}

void CompileScript(Context* context, const String& fileName)
{
    PrintLine("Compiling script file " + fileName);

    File inFile(context, fileName, FILE_READ);
    if (!inFile.IsOpen())
        ErrorExit("Failed to open script file " + fileName);

    ScriptFile script(context);
    if (!script.Load(inFile))
        ErrorExit();

    String outFileName = ReplaceExtension(fileName, ".asc");
    File outFile(context, outFileName, FILE_WRITE);
    if (!outFile.IsOpen())
        ErrorExit("Failed to open output file " + fileName);

    script.SaveByteCode(outFile);
}
