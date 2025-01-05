// Blur Feng All Rights Reserved.

#pragma once

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EWarriorValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EWarriorSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EWarriorCountDownActionInput : uint8
{
	//开始。
	Start,
	//取消。
	Cancel
};

UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
	//更新。
	Update,
	//完成。
	Completed,
	//取消。
	Cancelled
};