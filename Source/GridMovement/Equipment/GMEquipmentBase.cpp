

#include "GMEquipmentBase.h"


AGMEquipmentBase::AGMEquipmentBase()
{
	WorldMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("WorldMesh"));
	InventoryMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("WorldMesh"));
}
