#include "RoomManager.h"
#include "RoomActor.h"
#include "RoomDefinition.h"

ARoomManager::ARoomManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARoomManager::BeginPlay()
{
    Super::BeginPlay();
}

bool ARoomManager::CreateRoom(const FHexIndex Index, URoomDefinition* Definition, ARoomActor* RoomActor)
{
    if (!ensure(IsValid(Definition))) return false;
    if (!ensure(IsValid(RoomActor))) return false;
    
    if (!IsIndexValid(Index)) return false;
    if (RoomMap.Contains(Index)) return false; 

    FRoomNode NewRoom(Index, RoomActor);
    NewRoom.RoomDefinition = Definition;

    RoomMap.Add(Index, NewRoom);
    CalculateWallHeights(Index);
    CalculateDoorTypes(Index);

    // Update the floor count cache
    FloorRoomCounts.FindOrAdd(Index.FloorIndex)++;

    return true;
}

bool ARoomManager::GetRoom(const FHexIndex Index, FRoomNode& OutRoom)
{
    if (const FRoomNode* FoundRoom = RoomMap.Find(Index))
    {
        OutRoom = *FoundRoom;
        return true;
    }
    return false;
}

bool ARoomManager::HasRoomAt(const FHexIndex Index) const
{
    return RoomMap.Contains(Index);
}

FHexIndex ARoomManager::GetNeighborIndex(const FHexIndex Origin, const ERoomDirection Direction)
{
    FHexIndex Next = Origin;

    switch (Direction)
    {
        case ERoomDirection::North:      Next.R -= 1; break;             // (0, -1)
        case ERoomDirection::NorthEast:  Next.Q += 1; Next.R -= 1; break;// (+1, -1)
        case ERoomDirection::SouthEast:  Next.Q += 1; break;             // (+1, 0)
        case ERoomDirection::South:      Next.R += 1; break;             // (0, +1)
        case ERoomDirection::SouthWest:  Next.Q -= 1; Next.R += 1; break;// (-1, +1)
        case ERoomDirection::NorthWest:  Next.Q -= 1; break;             // (-1, 0)
        case ERoomDirection::Up:         Next.FloorIndex += 1; break;
        case ERoomDirection::Down:       Next.FloorIndex -= 1; break;
    }

    return Next;
}

bool ARoomManager::CheckAdjacency(const FHexIndex Origin, const ERoomDirection Direction) const
{
    const FHexIndex NeighborIndex = GetNeighborIndex(Origin, Direction);
    return RoomMap.Contains(NeighborIndex);
}

bool ARoomManager::TryMoveDirection(const FHexIndex Origin, const ERoomDirection Direction, FHexIndex& OutIndex) const
{
    FHexIndex Attempt = GetNeighborIndex(Origin, Direction);
    
    if (IsIndexValid(Attempt))
    {
        OutIndex = Attempt;
        return true;
    }
    
    // If we hit a wall going NW, try SW. If NE, try SE.
    ERoomDirection FallbackDir = Direction;
    bool bHasFallback = false;

    if (Direction == ERoomDirection::NorthWest)
    {
        FallbackDir = ERoomDirection::SouthWest;
        bHasFallback = true;
    }
    else if (Direction == ERoomDirection::NorthEast)
    {
        FallbackDir = ERoomDirection::SouthEast;
        bHasFallback = true;
    }

    if (bHasFallback)
    {
        Attempt = GetNeighborIndex(Origin, FallbackDir);
        if (IsIndexValid(Attempt))
        {
            OutIndex = Attempt;
            return true;
        }
    }

    return false;
}

void ARoomManager::CalculateWallHeights(FHexIndex RoomIndex)
{
    FRoomNode* RoomNode = RoomMap.Find(RoomIndex);
    if (!RoomNode) return;
    
    static constexpr ERoomDirection LowerDirs[] = {ERoomDirection::SouthWest, ERoomDirection::South, ERoomDirection::SouthEast};
    static constexpr ERoomDirection UpperDirs[] = {ERoomDirection::NorthWest, ERoomDirection::North, ERoomDirection::NorthEast};
    
    // Lower directions
    for (ERoomDirection Direction : LowerDirs)
    {
        FHexIndex NeighborIndex = GetNeighborIndex(RoomIndex, Direction);
        FRoomNode* AdjacentNode = RoomMap.Find(NeighborIndex);
        const int32 DirIndex = static_cast<int32>(Direction);
        
        if (AdjacentNode && RoomNode->WallArray.IsValidIndex(DirIndex))
        {
            // Update Neighbor Room 
            const int OppositeDir = static_cast<int>(GetOppositeDirection(Direction));
            if (AdjacentNode->WallArray.IsValidIndex(OppositeDir))
            {
                AdjacentNode->WallArray[OppositeDir] = false;

                // If directly above another room
                if (OppositeDir == static_cast<int32>(ERoomDirection::North))
                {
                    AdjacentNode->WallArray[static_cast<int32>(ERoomDirection::North)] = false;
                    AdjacentNode->WallArray[static_cast<int32>(ERoomDirection::NorthWest)] = false;
                    AdjacentNode->WallArray[static_cast<int32>(ERoomDirection::NorthEast)] = false;
                }

                if (IsValid(AdjacentNode->RoomActor)) 
                {
                    AdjacentNode->RoomActor->SetWallArray(AdjacentNode->WallArray);
                }
            }
        }
    }
    
    // Upper directions
    for (ERoomDirection Direction : UpperDirs)
    {
        FHexIndex NeighborIndex = GetNeighborIndex(RoomIndex, Direction);
        const int32 DirIndex = static_cast<int32>(Direction);
        
        // If there is a room above, no walls can be tall
        if (!RoomMap.Contains(NeighborIndex) && RoomNode->WallArray.IsValidIndex(DirIndex) && !CheckAdjacency(RoomIndex, ERoomDirection::North))
        {
            RoomNode->WallArray[DirIndex] = true;
        }
    }
    if (IsValid(RoomNode->RoomActor)) 
    {
        RoomNode->RoomActor->SetWallArray(RoomNode->WallArray);
    }
}

void ARoomManager::CalculateDoorTypes(FHexIndex RoomIndex)
{
    FRoomNode* RoomNode = RoomMap.Find(RoomIndex);
    if (!RoomNode) return;

    RoomNode->DoorArray = RoomNode->RoomDefinition->DoorArray;
    TArray<EDoorTypes>& DoorTypes = RoomNode->DoorArray;
    
    for (int i = 0; i < NUM_ROOM_DIRECTIONS; i++)
    {
        const ERoomDirection Direction = static_cast<ERoomDirection>(i);
        FHexIndex NeighborIndex = GetNeighborIndex(RoomIndex, Direction);
        FRoomNode* AdjacentNode = RoomMap.Find(NeighborIndex);
        if (DoorTypes[i] == EDoorTypes::Door || DoorTypes[i] == EDoorTypes::Blocked)
        {
            if (CheckAdjacency(RoomIndex, Direction))
            {
                DoorTypes[i] = EDoorTypes::Door;
                if (AdjacentNode)
                {
                    const int OppositeDir = static_cast<int>(GetOppositeDirection(Direction));
                    AdjacentNode->DoorArray[OppositeDir] = EDoorTypes::Door;
                    if (IsValid(AdjacentNode->RoomActor)) 
                    {
                        AdjacentNode->RoomActor->SetDoorArray(AdjacentNode->DoorArray);
                    }
                }
            }
            else
            {
                DoorTypes[i] = EDoorTypes::Blocked;
            }
        }
    }

    if (IsValid(RoomNode->RoomActor)) 
    {
        RoomNode->RoomActor->SetDoorArray(RoomNode->DoorArray);
    }
}

bool ARoomManager::FindNextSpotInDirection(FHexIndex Origin, ERoomDirection Direction, FHexIndex& OutResult, bool& bHitExistingRoom)
{
    if (TryMoveDirection(Origin, Direction, OutResult))
    {
        bHitExistingRoom = RoomMap.Contains(OutResult);
        return true;
    }
    
    return false;
}

FVector ARoomManager::GetRelativeLocationFromHex(const FHexIndex Index) const
{
    // Pointy-topped Hex conversion
    static const float Sqrt3 = FMath::Sqrt(3.0f);
 
    const float x = HexSize * Sqrt3 * (Index.Q + Index.R / 2.0f);
    const float y = HexSize * 1.5f * Index.R;
    const float z = Index.FloorIndex * FloorHeight;

    return FVector(x, y, z);
}

bool ARoomManager::IsIndexValid(const FHexIndex& Index) const
{
    // Axial distance formula: (abs(q) + abs(q+r) + abs(r)) / 2
    const int32 Distance = (FMath::Abs(Index.Q) + FMath::Abs(Index.Q + Index.R) + FMath::Abs(Index.R)) / 2;
    return Distance <= MaxHexRadius;
}

bool ARoomManager::MoveSelector(const ERoomInputDirection InputDir)
{
    // Prevent going below Floor 0
    if (InputDir == ERoomInputDirection::Down && CurrentIndex.FloorIndex <= 0)
    {
        return false;
    }

    // Prevent going above the highest floor that currently has a room
    if (InputDir == ERoomInputDirection::Up)
    {
        const int32 MaxFloor = GetMaxOccupiedFloor();
        if (CurrentIndex.FloorIndex >= MaxFloor + 1) return false;
    }

    FHexIndex TargetIndex;
    bool bHitExistingRoom;

    if (FindNextSpotInDirection(CurrentIndex, ToRoomDirection(InputDir), TargetIndex, bHitExistingRoom))
    {
        CurrentIndex = TargetIndex;
        UE_LOG(LogTemp, Log, TEXT("[%f] move selector to %s"), GetWorld()->GetTimeSeconds(), *CurrentIndex.ToString());
        return true;
    }
    
    return false;
}

ERoomDirection ARoomManager::ToRoomDirection(const ERoomInputDirection InputDir) const
{
    switch (InputDir)
    {
        case ERoomInputDirection::South: return ERoomDirection::South;
        case ERoomInputDirection::Up:    return ERoomDirection::Up;
        case ERoomInputDirection::Down:  return ERoomDirection::Down;
        
        // Logic for mapping 4-way input to 6-way hexes
        // If in the center column move up diagonal, if not move down diagonal
        case ERoomInputDirection::West:
            return (CurrentIndex.Q == 0) ? ERoomDirection::NorthWest : ERoomDirection::SouthWest;
            
        case ERoomInputDirection::East:
            return (CurrentIndex.Q == 0) ? ERoomDirection::NorthEast : ERoomDirection::SouthEast;

        case ERoomInputDirection::North:
        default:
            return ERoomDirection::North;
    }
}

int32 ARoomManager::GetMaxOccupiedFloor() const
{
    int32 MaxFloor = 0;
    
    for (const auto& Pair : FloorRoomCounts)
    {
        // Pair.Key is FloorIndex, Pair.Value is RoomCount
        if (Pair.Value > 0)
        {
            MaxFloor = FMath::Max(MaxFloor, Pair.Key);
        }
    }
    
    return MaxFloor;
}

ERoomDirection ARoomManager::GetOppositeDirection(const ERoomDirection InputDir)
{
    const int EnumIndex = static_cast<int>(InputDir);
    if (InputDir == ERoomDirection::Down) return ERoomDirection::Up;
    if (InputDir == ERoomDirection::Up) return ERoomDirection::Down;
    return static_cast<ERoomDirection>((EnumIndex + 3) % 6);
}