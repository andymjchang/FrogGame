#include "RoomDefinition.h"

#include "RoomTypes.h"

URoomDefinition::URoomDefinition()
{
	DoorArray.Init(EDoorTypes::None, 6);
}
