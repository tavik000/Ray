// Fill out your copyright notice in the Description page of Project Settings.


#include "RayMultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void PrintString(const FString& Str)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, Str);
	}
}

URayMultiplayerSessionSubsystem::URayMultiplayerSessionSubsystem()
{
	ShouldCreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Ray Session Name");
}

void URayMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		// PrintString("MSS Initialize");

		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString("Using SubsystemName: " + SubsystemName);
		

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &URayMultiplayerSessionSubsystem::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &URayMultiplayerSessionSubsystem::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &URayMultiplayerSessionSubsystem::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
				this, &URayMultiplayerSessionSubsystem::OnJoinSessionComplete);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No OnlineSubsystem found");
	}
}

void URayMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void URayMultiplayerSessionSubsystem::CreateServer(FString ServerName)
{
	PrintString("CreateServer called");
	if (ServerName.IsEmpty())
	{
		PrintString("ServerName is empty");
		ServerCreateDel.Broadcast(false);
		return;
	}

	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SessionInterface is null");
		UE_LOG(LogTemp, Error, TEXT("SessionInterface is null"));
		return;
	}
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);

	if (ExistingSession)
	{
		FString Msg = FString::Printf(TEXT("Session %s already exists, destroying it."), *MySessionName.ToString());
		PrintString(Msg);
		ShouldCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);

		ServerCreateDel.Broadcast(false);
		return;
	}
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	bool IsLAN = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	SessionSettings.bIsLANMatch = IsLAN;
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void URayMultiplayerSessionSubsystem::FindServer(FString ServerName)
{
	PrintString("JoinServer called");

	if (ServerName.IsEmpty())
	{
		PrintString("ServerName is empty");

		ServerJoinDel.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void URayMultiplayerSessionSubsystem::OnCreateSessionComplete(FName ToCreateSessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %s"), WasSuccessful ? TEXT("True") : TEXT("False")));
	ServerCreateDel.Broadcast(WasSuccessful);
	if (WasSuccessful)
	{
		FString Path = "/Game/Main/Demo/Maps/L_Demo?listen";
		if (!GameMapPath.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *GameMapPath);
		}
		GetWorld()->ServerTravel(Path);
	}
}

void URayMultiplayerSessionSubsystem::OnDestroySessionComplete(FName ToDestroySessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(
		TEXT("OnDestroySessionComplete, Session Name %s, Was Successful: %s"), *ToDestroySessionName.ToString(),
		WasSuccessful ? TEXT("True") : TEXT("False")));

	if (!WasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Failed to destroy session %s"), *ToDestroySessionName.ToString()));
		return;
	}

	if (ShouldCreateServerAfterDestroy)
	{
		ShouldCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void URayMultiplayerSessionSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (!WasSuccessful)
	{
		ServerJoinDel.Broadcast(false);
		return;
	}
	if (ServerNameToFind.IsEmpty())
	{
		ServerJoinDel.Broadcast(false);
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = 0;
	if (Results.Num() <= 0)
	{
		PrintString("No sessions found");

		ServerJoinDel.Broadcast(false);
		return;
	}

	FString Msg = FString::Printf(TEXT("Found %d sessions"), Results.Num());
	PrintString(Msg);
	for (FOnlineSessionSearchResult Result : Results)
	{
		if (!Result.IsValid()) continue;
		FString FoundSessionName = Result.GetSessionIdStr();
		FString ServerName = "No-name";
		Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);
		if (ServerName.Equals(ServerNameToFind))
		{
			CorrectResult = &Result;
			Msg = FString::Printf(TEXT("Found session %s with server name %s"), *FoundSessionName, *ServerName);
			PrintString(Msg);
			break;
		}
	}

	if (!CorrectResult)
	{
		PrintString("No session with the correct server name found");
		ServerNameToFind = "";

		ServerJoinDel.Broadcast(false);
		return;
	}

	SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
}

void URayMultiplayerSessionSubsystem::OnJoinSessionComplete(FName ToJoinSessionName,
                                                            EOnJoinSessionCompleteResult::Type Result)
{
	if (!Result == EOnJoinSessionCompleteResult::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,
		                                 FString::Printf(
			                                 TEXT("Failed to join session %s"), *ToJoinSessionName.ToString()));
		ServerJoinDel.Broadcast(false);
		return;
	}

	FString Msg = FString::Printf(TEXT("Joined session %s"), *ToJoinSessionName.ToString());
	PrintString(Msg);
	FString Address = "";
	bool IsSuccessful = SessionInterface->GetResolvedConnectString(ToJoinSessionName, Address);
	if (!IsSuccessful)
	{
		PrintString("GetResolvedConnectString failed");

		ServerJoinDel.Broadcast(false);
		return;
	}
	PrintString(FString::Printf(TEXT("Resolved connect string: %s"), *Address));
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PlayerController)
	{
		PrintString("PlayerController is null");

		ServerJoinDel.Broadcast(false);
		return;
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
