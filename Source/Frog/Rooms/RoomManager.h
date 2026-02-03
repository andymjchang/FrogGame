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
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Data")
    TMap<FHexIndex, FRoomNode> RoomMap;
    
    UPROPERTY(VisibleAnywhere, Category = "Room Data")
    TMap<int32, int32> FloorRoomCounts;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Data")
    FHexIndex CurrentIndex;

    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    float HexSize = 1000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    float FloorHeight = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Room Config")
    int MaxHexRadius = 1;

public: 
    /** Creates a room at the specified index if valid. Returns true if successful. */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool CreateRoom(FHexIndex Index, URoomDefinition* Definition, ARoomActor* RoomActor);

    /** Returns the Room Node struct. Returns a default empty node if not found. */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool GetRoom(FHexIndex Index, FRoomNode& OutRoom);

    /** Checks if a room exists at the index. */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    bool HasRoomAt(FHexIndex Index) const;

    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool MoveSelector(ERoomInputDirection InputDir);

    

    // --- Spatial Queries ---

    /** * Returns the neighbor index in a specific direction. 
     * Does NOT check if the room exists, just calculates the math.
     */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    static FHexIndex GetNeighborIndex(FHexIndex Origin, ERoomDirection Direction);

    /*
     * Returns if there is a valid room in the adjacent room in the input direction.
     */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    bool CheckAdjacency(FHexIndex Origin, ERoomDirection Direction) const;

    /**
     * "Line cast" search. Moves in 'Direction' from 'Origin' until it hits:
     * 1. An existing room (Target)
     * 2. An empty valid spot (Target)
     * 3. The boundary (Returns false)
     */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool FindNextSpotInDirection(FHexIndex Origin, ERoomDirection Direction, FHexIndex& OutResult, bool& bHitExistingRoom);

    // --- Helpers ---

    /** Converts Grid Coordinates to World Space for spawning meshes */
    UFUNCTION(BlueprintPure, Category = "Room Logic")
    FVector GetRelativeLocationFromHex(FHexIndex Index) const;

    /** Validates if the hex is within the "Flower" shape (Center + 1 Ring) */
    UFUNCTION(BlueprintCallable, Category = "Room Logic")
    bool IsIndexValid(const FHexIndex& Index) const;

    /** 
     * Convert WASD input directions to hexagon movement
     * Prioritize SW/SE when moving away from center column and NW/NE when moving towards center 
     */
    ERoomDirection ToRoomDirection(ERoomInputDirection InputDir) const;
    
    int32 GetMaxOccupiedFloor() const;

    static ERoomDirection GetOppositeDirection(ERoomDirection InputDir);

    /**
     * Tries to move, NW/NE -> SW/SE if the first attempt is invalid.
     */
    bool TryMoveDirection(FHexIndex Origin, ERoomDirection Direction, FHexIndex& OutIndex) const;

    /**
     * Make the walls tall if there is no room to the N/NW/NE of the room.
     * Recursively recheck rooms connected to this room.
     */
    void CalculateWallHeights(FHexIndex RoomIndex);

    /**
     * If there should be a door at a given direction, set its EDoorType.
     * Uses the room definition of the RoomNode at this RoomIndex.
     */
    void CalculateDoorTypes(FHexIndex RoomIndex);
};