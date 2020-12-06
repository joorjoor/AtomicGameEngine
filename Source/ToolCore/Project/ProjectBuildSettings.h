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

#pragma once

#include <Atomic/Core/Object.h>
#include <Atomic/Resource/JSONValue.h>

using namespace Atomic;

namespace ToolCore
{

class MacBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(MacBuildSettings)

public:

    MacBuildSettings() {}

    const String& GetAppName() const { return appName_; }
    const String& GetPackageName() const { return packageName_; }
    const String& GetCompanyName() const { return companyName_; }
    const String& GetProductName() const { return productName_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetPackageName(const String& packageName) { packageName_ = packageName; }
    void SetCompanyName(const String& companyName) { companyName_ = companyName; }
    void SetProductName(const String& productName) { productName_ = productName; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String packageName_;
    String companyName_;
    String productName_;
};

class WebBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(WebBuildSettings)

public:

    WebBuildSettings() 
    {
        pageTheme_ = 0;  
        gameWidth_ = "800";
        gameHeight_ = "512";
    }

    const String& GetAppName() const { return appName_; }
    const String& GetGameWidth() const { return gameWidth_; }
    const String& GetGameHeight() const { return gameHeight_; }
    const String& GetFaviconName() const { return faviconName_; }
    int GetPageTheme() const { return pageTheme_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetGameWidth(const String& packageName) { gameWidth_ = packageName; }
    void SetGameHeight(const String& companyName) { gameHeight_ = companyName; }
    void SetFaviconName(const String& productName) { faviconName_ = productName; }
    void SetPageTheme( int number ) { pageTheme_ = number; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String gameWidth_;
    String gameHeight_;
    String faviconName_;
    int pageTheme_;
};

class WindowsBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(WindowsBuildSettings)

public:

    WindowsBuildSettings() {}

    const String& GetAppName() const { return appName_; }
    const String& GetPackageName() const { return packageName_; }
    const String& GetCompanyName() const { return companyName_; }
    const String& GetProductName() const { return productName_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetPackageName(const String& packageName) { packageName_ = packageName; }
    void SetCompanyName(const String& companyName) { companyName_ = companyName; }
    void SetProductName(const String& productName) { productName_ = productName; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String packageName_;
    String companyName_;
    String productName_;
};

class AndroidBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(AndroidBuildSettings)

public:

    AndroidBuildSettings() {}

    const String& GetAppName() const { return appName_; }
    const String& GetPackageName() const { return packageName_; }
    const String& GetCompanyName() const { return companyName_; }
    const String& GetProductName() const { return productName_; }

    const String& GetSDKVersion() const { return targetSDKVersion_; }
    const String& GetMinSDKVersion() const { return minSDKVersion_; }
    const String& GetActivityName() const { return activityName_; }
    const String& GetIconPath() const { return iconPath_; }

	const String& GetVersion() const { return version_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetPackageName(const String& packageName) { packageName_ = packageName; }
    void SetCompanyName(const String& companyName) { companyName_ = companyName; }
    void SetProductName(const String& productName) { productName_ = productName; }

    void SetSDKVersion(const String& value) { targetSDKVersion_ = value; }
    void SetMinSDKVersion(const String& value) { minSDKVersion_ = value; }
    void SetActivityName(const String& value) { activityName_ = value; }
    void SetIconPath(const String& value) { iconPath_ = value; }

    void SetVersion(const String& version) { version_ = version; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String packageName_;
    String companyName_;
    String productName_;

    String targetSDKVersion_;
    String minSDKVersion_;
    String activityName_;
    String iconPath_;
	
	String version_;
};

class IOSBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(IOSBuildSettings)

public:

    IOSBuildSettings() {}

    const String& GetAppName() const { return appName_; }
    const String& GetPackageName() const { return packageName_; }
    const String& GetCompanyName() const { return companyName_; }
    const String& GetProductName() const { return productName_; }

    const String& GetProvisionFile() const { return provisionFile_; }
    const String& GetAppIDPrefix() const { return appidPrefix_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetPackageName(const String& packageName) { packageName_ = packageName; }
    void SetCompanyName(const String& companyName) { companyName_ = companyName; }
    void SetProductName(const String& productName) { productName_ = productName; }

    void SetProvisionFile(const String& value) { provisionFile_ = value; }
    void SetAppIDPrefix(const String& value) { appidPrefix_ = value; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String packageName_;
    String companyName_;
    String productName_;

    String provisionFile_;
    String appidPrefix_;
};

class LinuxBuildSettings : public RefCounted
{
    ATOMIC_REFCOUNTED(LinuxBuildSettings)

public:

    LinuxBuildSettings() {}

    const String& GetAppName() const { return appName_; }
    const String& GetPackageName() const { return packageName_; }
    const String& GetCompanyName() const { return companyName_; }
    const String& GetProductName() const { return productName_; }

    void SetAppName(const String& name) { appName_ = name; }
    void SetPackageName(const String& packageName) { packageName_ = packageName; }
    void SetCompanyName(const String& companyName) { companyName_ = companyName; }
    void SetProductName(const String& productName) { productName_ = productName; }

    void Write(JSONValue& parent);
    void Read(JSONValue& parent);

private:

    String appName_;
    String packageName_;
    String companyName_;
    String productName_;
};


class ProjectBuildSettings : public Object
{
    ATOMIC_OBJECT(ProjectBuildSettings, Object);

public:
    /// Construct.
    ProjectBuildSettings(Context* context);
    /// Destruct.
    virtual ~ProjectBuildSettings();

    MacBuildSettings* GetMacBuildSettings() { return macBuildSettings_; }
    WindowsBuildSettings* GetWindowsBuildSettings() { return windowsBuildSettings_; }
    WebBuildSettings* GetWebBuildSettings() { return webBuildSettings_; }
    AndroidBuildSettings* GetAndroidBuildSettings() { return androidBuildSettings_; }
    IOSBuildSettings* GetIOSBuildSettings() { return iosBuildSettings_; }
    LinuxBuildSettings* GetLinuxBuildSettings() { return linuxBuildSettings_; }

    bool Load(const String& path);
    void Save(const String& path);

private:

    SharedPtr<MacBuildSettings> macBuildSettings_;
    SharedPtr<WindowsBuildSettings> windowsBuildSettings_;
    SharedPtr<WebBuildSettings> webBuildSettings_;
    SharedPtr<AndroidBuildSettings> androidBuildSettings_;
    SharedPtr<IOSBuildSettings> iosBuildSettings_;
    SharedPtr<LinuxBuildSettings> linuxBuildSettings_;

};

}
