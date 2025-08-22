#pragma once

#include "Core.h"

// 상속 관계에 있는 클래스 간의
// 동적(실행중에) 형변환을 빠르게 하기 위해 사용하는 클래스.
// RunTime-Type-Information.
class ENGINE_API RTTI
{
public:
	// 타입 정보 반환 순수 가상 함수
	virtual const size_t& TypeIdInstance() const = 0;

	virtual bool Is(const size_t id) const
	{
		return false;
	}

	template<typename T>
	T* As()
	{
		// RTTI를 상속받는 클래스는 반드시 TypeIdClass가 있어야 함.
		if (Is(T::TypeIdClass()))
		{
			return (T*)this;
		}

		return nullptr;
	}

	template<typename T>
	const T* As() const
	{
		if (Is(T::TypeIdClass()))
		{
			return (T*)this;
		}

		return nullptr;
	}
};

// 자손, 부모를 인자로 받음
#define RTTI_DECLARATIONS(Type, ParentType)												\
public:																					\
	using super = ParentType;															\
	virtual const size_t& TypeIdInstance() const { return Type::TypeIdClass(); }		\
	static const size_t TypeIdClass()													\
	{																					\
		static int runTimeTypeId = 0;													\
		return reinterpret_cast<size_t>(&runTimeTypeId);								\
	}																					\
	virtual bool Is(const size_t id) const												\
	{																					\
		if (id == TypeIdClass())														\
		{																				\
			return true;																\
		}																				\
		else																			\
		{																				\
			return ParentType::Is(id);													\
		}																				\
	}