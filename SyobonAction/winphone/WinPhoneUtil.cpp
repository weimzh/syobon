#include <wrl.h>
#include <stdio.h>

const char *
GetRootPath()
{
	static char buf[1024] = "";
	if (buf[0] == '\0')
	{
		Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
		const char16 *begin = localfolder->Begin();
		WideCharToMultiByte(CP_ACP, 0, begin, -1, buf, 1024, NULL, FALSE);
	}
	return buf;
}

const char *
GetInstallPath()
{
	static char buf[1024] = "";
	if (buf[0] == '\0')
	{
		Platform::String^ installfolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
		const char16 *begin = installfolder->Begin();
		WideCharToMultiByte(CP_ACP, 0, begin, -1, buf, 1024, NULL, FALSE);
	}
	return buf;
}
