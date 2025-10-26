// Copyright DX_IC


#include "DX_StaticFunlib.h"
#include "CollisionQueryParams.h"

bool UDX_StaticFunlib::GetAllFilesInFolder(const FString& RootDir, TArray<FString>& OutSubDirs, TArray<FString>& OutSubFiles, const FString& ExtentName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	/*FString AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	AbsolutePath = AbsolutePath.Append(RootDir);*/
	FString AbsolutePath = RootDir;

	// ���Ŀ¼�Ƿ����
	if (!PlatformFile.DirectoryExists(*AbsolutePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Directory does not exist: %s"), *AbsolutePath);
		return false;
	}

	// ʹ��IterateDirectory���б���
	PlatformFile.IterateDirectory(*AbsolutePath, [&](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
		{
			if (bIsDirectory)
			{
				// ���˵���ǰĿ¼(.)���ϼ�Ŀ¼(..)
				FString DirName = FPaths::GetCleanFilename(FilenameOrDirectory);
				if (DirName != TEXT(".") && DirName != TEXT(".."))
				{
					OutSubDirs.Add(FString(FilenameOrDirectory));
				}
			}
			else
			{
				FString FilePath = FString(FilenameOrDirectory);

				if (!ExtentName.IsEmpty())
				{
					// ����ļ���չ��
					FString Extension = FPaths::GetExtension(FilePath);
					if (Extension.Equals(ExtentName, ESearchCase::IgnoreCase))
					{
						OutSubFiles.Add(FilePath);
					}
				}
				else
					OutSubFiles.Add(FilePath);
			}
			return true; // ����true�Լ�������
		});

	return true;
}

void UDX_StaticFunlib::MakeCollisionParam(const AActor* Avatar, FCollisionQueryParams& CollisionQueryParams, FCollisionObjectQueryParams& ObjectQueryParams, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, bool bTraceComplex, bool bIgnoreSelf)
{
	static const FName LineTraceTag(TEXT("LineTraceMultiForObjects"));
	//ConfigureCollisionParams(LineTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, MeshComp->GetOwner());
	CollisionQueryParams = FCollisionQueryParams(LineTraceTag, bTraceComplex);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	if (bIgnoreSelf)
	{
		if (Avatar)
		{
			CollisionQueryParams.AddIgnoredActor(Avatar);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = Avatar;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				Avatar = Cast<AActor>(CurrentObject);
				if (Avatar)
				{
					CollisionQueryParams.AddIgnoredActor(Avatar);
					break;
				}
			}
		}
	}

	ObjectQueryParams = FCollisionObjectQueryParams(TraceObjectTypes);
}
