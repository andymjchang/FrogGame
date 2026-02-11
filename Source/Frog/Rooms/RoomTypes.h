#pragma once

#include "CoreMinimal.h"
#include "RoomTypes.generated.h"

class URoomDefinition;
class ARoomActor;

static constexpr int NUM_ROOM_SIDES = 6;

UENUM(BlueprintType)
enum class EDoorTypes : uint8
{
    None,
    Locked,
    Blocked
};

UENUM(BlueprintType)
enum class ERoomInputDirection : uint8
{
    North,
    South,
    West,
    East,
    Up,
    Down
};

/**
 * Directions for 8-way connectivity + Verticality
 * Clockwise order starting from North
 */
UENUM(BlueprintType)
enum class ERoomDirection : uint8
{
    North       UMETA(DisplayName = "North (-1, 0)"),
    NorthEast   UMETA(DisplayName = "North East (-1, +1)"),
    SouthEast   UMETA(DisplayName = "South East (0, +1)"),
    South       UMETA(DisplayName = "South (+1, 0)"),
    SouthWest   UMETA(DisplayName = "South West (+1, -1)"),
    NorthWest   UMETA(DisplayName = "North West (0, -1)"),
    Up          UMETA(DisplayName = "Up (Z+1)"),
    Down        UMETA(DisplayName = "Down (Z-1)")
};

static constexpr ERoomDirection LowerDirs[] = {ERoomDirection::SouthWest, ERoomDirection::South, ERoomDirection::SouthEast};
static constexpr ERoomDirection UpperDirs[] = {ERoomDirection::NorthWest, ERoomDirection::North, ERoomDirection::NorthEast};

/**
 * The unique coordinate identifier for a room slot.
 * Axial Coordinates (Q, R) + Floor Index (Z).
 */
USTRUCT(BlueprintType)
struct FHexIndex
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coordinates")
    int32 Q = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coordinates")
    int32 R = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coordinates")
    int32 FloorIndex = 0;

    FHexIndex() {}
    FHexIndex(int32 InQ, int32 InR, int32 InFloor) : Q(InQ), R(InR), FloorIndex(InFloor) {}

    // Equality operator required for TMap
    bool operator==(const FHexIndex& Other) const
    {
        return Q == Other.Q && R == Other.R && FloorIndex == Other.FloorIndex;
    }

    // Inequality operator
    bool operator!=(const FHexIndex& Other) const
    {
        return !(*this == Other);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("(Q:%d, R:%d, Z:%d)"), Q, R, FloorIndex);
    }
};

/**
 * Global hashing function required for TMap<FHexIndex, ...>
 * Must be outside the struct in the global scope.
 */
FORCEINLINE uint32 GetTypeHash(const FHexIndex& Index)
{
    // Combine hash of Q and R, then combine that result with FloorIndex
    return HashCombine(HashCombine(GetTypeHash(Index.Q), GetTypeHash(Index.R)), GetTypeHash(Index.FloorIndex));
}

/**
 * Represents the data of a single room node in the graph.
 */
USTRUCT(BlueprintType)
struct FRoomNode
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    FHexIndex HexIndex;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    TObjectPtr<ARoomActor> RoomActor;
    
    UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Room")
    TArray<EDoorTypes> DoorTypeArray;

    UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Room")
    TArray<bool> WallTypeArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    URoomDefinition* RoomDefinition = nullptr;
    
    FRoomNode() 
    {
        InitializeDefaults();
    }
    
    FRoomNode(const FHexIndex InHex, ARoomActor* InActor) : HexIndex(InHex), RoomActor(InActor)
    {
        InitializeDefaults();
    }
    
    void InitializeDefaults()
    {
        DoorTypeArray.Init(EDoorTypes::None, 6); 
        WallTypeArray.Init(false, 6); 
    }
};