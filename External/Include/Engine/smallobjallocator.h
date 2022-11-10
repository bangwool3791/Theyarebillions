#pragma once

#include "chunkgroup.h"

inline ChunkGroup* pool;
/*  이것보다 더 좋은 싱글턴은 많이 있다.
	상황에 맞는 좋은 싱글턴을 사용하길 바란다. */
template <size_t OBJ_SIZE, unsigned char MAX_OBJ_NUM>
ChunkGroup* ChunkGroupInstance()
{
	if (!pool)
		pool = new ChunkGroup(OBJ_SIZE, MAX_OBJ_NUM);
	return pool;
}
/* 상속으로만 사용한다면, 템플릿으로 박아 둔다. */
template <typename T, size_t MAX_OBJ_NUM>
class SmallObjAllocator
{
public:
	static void* operator new(size_t s)
	{
		return ChunkGroupInstance<sizeof(T), MAX_OBJ_NUM>()->alloc();
	}

	static void operator delete(void* pdelloc_target, std::size_t obj_size)
	{
		if (NULL == pdelloc_target)
			return;

		ChunkGroupInstance<sizeof(T), MAX_OBJ_NUM>()->delloc(pdelloc_target);
	}
};
