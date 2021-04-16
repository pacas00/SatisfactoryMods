// ILikeBanas


#include "FDirectoryLister.h"

bool FDirectoryLister::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (!bIsDirectory) return false;

	if (!FPaths::IsRelative(FilenameOrDirectory))
	{
		// Pass back the file name
		new(Result)FString(FPaths::GetCleanFilename(FilenameOrDirectory));
	}
	

	return true;
}
