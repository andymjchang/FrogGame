#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypes.h"
#include "RoomManager.generated.h"

class URoomDefinition;

UCLASS()
class FROG_API ARoomManager : public APawn
{
    GENERATED_BODY()
    
public: 
    ARoomManager();

protected:
    virtual void BeginPlay() override;

    /** Creates a room at the specified index if valid. Returns true if successful. */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool CreateRoom(const FHexIndex Index, URoomDefinition* Definition, ARoomActor* RoomActor);

    /** Returns the Room Node struct. Returns a default empty node if not found. */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool GetRoom(const FHexIndex Index, FRoomNode& OutRoom);

    /** Checks if a room exists at the index. */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    bool HasRoomAt(const FHexIndex Index) const;

    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool MoveSelector(ERoomInputDirection InputDir);

    UFUNCTION(BlueprintPure, Category = "Room Logic")
    FVector GetRelativeLocationFromHex(FHexIndex Index) const;
    
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool IsIndexValid(const FHexIndex Index) const;
        
    UFUNCTION()
    void HandleOnRequestNewRoom(FHexIndex HexIndex, ERoomDirection FacingDirection, int32 UnlockPrice);
    
    ERoomDirection ToRoomDirection(ERoomInputDirection InputDir) const;
    int32 GetMaxOccupiedFloor() const;
    static ERoomDirection GetOppositeDirection(ERoomDirection InputDir);
    bool TryMoveDirection(const FHexIndex Origin, ERoomDirection Direction, FHexIndex& OutIndex) const;
    
    void CalculateWallHeights(const FHexIndex RoomIndex);
    void CalculateDoorTypes(const FHexIndex RoomIndex);

    /** * Returns the neighbor index in a specific direction. 
     * Does NOT check if the room exists, just calculates the math.
     */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    static FHexIndex GetNeighborIndex(const FHexIndex Origin, ERoomDirection Direction);

    /*
     * Returns if there is a valid room in the adjacent room in the input direction.
     */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    bool CheckAdjacency(const FHexIndex Origin, ERoomDirection Direction) const;

    /**
     * "Line cast" search. Moves in 'Direction' from 'Origin' until it hits:
     * 1. An existing room (Target)
     * 2. An empty valid spot (Target)
     * 3. The boundary (Returns false)
     */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool FindNextSpotInDirection(const FHexIndex Origin, ERoomDirection Direction, FHexIndex& OutResult, bool& bHitExistingRoom);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Data")
    TMap<FHexIndex, FRoomNode> RoomMap;
    
    UPROPERTY(VisibleAnywhere, Category = "Room Data")
    TMap<int32, int32> FloorRoomCounts;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Data")
    FHexIndex CurrentIndex;

    // Accounts for overlapping walls
    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    float HexSize = 962.75f;

    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    float FloorHeight = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    int MaxHexRadius = 1;
};