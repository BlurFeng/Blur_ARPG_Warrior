// Blur Feng All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_StartUpBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"


void UDataAsset_StartUpBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	//初始化能力
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}

void UDataAsset_StartUpBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive,
	UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty()) return;

	//在启动时，将配置的GameplayAbility给到AvatarActor的AbilitySystemComponent。
	for(const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability) continue;

		// Gameplay Ability Specification 用于描述Ability能力的详细信息。
		FGameplayAbilitySpec AbilitySpec(Ability); //通过GameplayAbility创建Specification。
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor(); //能力来源对象，可以是角色，或者静态物体。
		AbilitySpec.Level = ApplyLevel; //能力等级。

		//只有经过GiveAbility()的能力才能使用。
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
