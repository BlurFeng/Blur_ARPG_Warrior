// Blur Feng All Rights Reserved.


#include "Components/PawnExtensionComponentBase.h"

APawn* UPawnExtensionComponentBase::GetOwningPawn() const
{
	return GetOwningPawn<APawn>();
}

AController* UPawnExtensionComponentBase::GetOwningController() const
{
	return GetOwningController<AController>();
}
