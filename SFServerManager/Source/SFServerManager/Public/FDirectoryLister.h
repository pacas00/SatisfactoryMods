// ILikeBanas

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SFSERVERMANAGER_API FDirectoryLister : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString>& Result;
	FString WildCard;
	bool bFiles;
	bool bDirectories;
	FDirectoryLister(TArray<FString>& InResult, const FString& InWildCard, bool bInFiles, bool bInDirectories)
		: Result(InResult)
		, WildCard(InWildCard)
		, bFiles(bInFiles)
		, bDirectories(bInDirectories)
	{
	}

	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
};
