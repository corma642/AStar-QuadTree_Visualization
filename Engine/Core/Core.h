#pragma once

// DLL 외부로 템플릿을 내보낼 때 발생하는 경고 비활성화
#pragma warning(disable: 4251)

// 지역 변수의 주소를 반환할 때 발생하는 경고 비활성화
#pragma warning(disable: 4172)

// 메모리 누수 디버깅
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#if BuildEngineDLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


// 메모리 정리 함수
template<typename T>
void SafeDelete(T*& target) // 포인터의 래퍼런스 받기
{
	if (target)
	{
		delete target;
		target = nullptr;
	}
}

// 배열 메모리 정리 함수
template<typename T>
void SafeDeleteArray(T*& target) // 포인터의 래퍼런스 받기
{
	if (target)
	{
		delete[] target;
		target = nullptr;
	}
}