#pragma once

namespace Debug
{
	//Debug用类，提供了打印等常用的Debug方法。
	//通过 #include "WarriorDebugHelper.h" 引入进行使用。
	
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		//Notes：在传入参数中，const确保传入参数不可修改，&用于引用传递，避免拷贝产生性能问题。
		
		//确保游戏引擎类已经准备好
		if(GEngine)
		{
			//打印到屏幕
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg);

			//Notes：*Msg 解引用
			//*Msg中的*用于解引用，将复杂类型FString转换为TCHAR*原生字符串形式（C风格字符串），供UE_LOG使用。
			//使用TEXT("%s")格式化字符串，%s占位符的实际内容是之后的*Msg。
			//打印到Log文件中
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1,  const FColor& Color = FColor::MakeRandomColor())
	{
		if(GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}
