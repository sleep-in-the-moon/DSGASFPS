// Copyright DX_IC


#include "AsyncTasks/DXAsyncTasks.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2D.h"
#include "Async/Async.h"

ULoadImageAsync* ULoadImageAsync::LoadLocalImageAsync(const FString& LocalPath)
{
	ULoadImageAsync* Task = NewObject<ULoadImageAsync>();
	Task->Path = LocalPath;
	return Task;
}

void ULoadImageAsync::Activate()
{
	Async(EAsyncExecution::Thread, [this]()
		{
			// 1. �ں�̨�̣߳���ȡ�ļ����ݲ�����
			TArray<uint8> CompressedData;
			if (!FFileHelper::LoadFileToArray(CompressedData, *Path))
			{
				// �������ļ���ȡʧ�ܣ��ص����̻߳ص�
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG); // PNG����ʽ����չ

			if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
			{
				// ����������ʧ�ܣ��ص����̻߳ص�
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			TArray<uint8> UncompressedData;
			if (!ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedData))
			{
				// ��������ȡԭʼ����ʧ��
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			// 2. ��ȡͼƬ�ߴ���Ϣ
			int32 Width = ImageWrapper->GetWidth();
			int32 Height = ImageWrapper->GetHeight();

			// 3. �ص���Ϸ���̴߳���������Ϊ����������̲߳���ȫ�ģ�
			Async(EAsyncExecution::TaskGraphMainThread, [Width, Height, UncompressedData = MoveTemp(UncompressedData), this]()
				{
					// ������ʱ����
					UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
					if (Texture)
					{
						// �����������ݲ�����
						FByteBulkData& BulkData = Texture->GetPlatformData()->Mips[0].BulkData;
						void* TextureData = BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());
						BulkData.Unlock();
						// ����������Դ
						Texture->UpdateResource();
					}
					// �ص����ݴ����õ�����
					OnSuccess.Broadcast(Texture);
				});
		});
}
