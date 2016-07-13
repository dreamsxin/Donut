#ifndef SYSTEM_ALLOCATOR_H
#define SYSTEM_ALLOCATOR_H

// Library includes
#include "allocator.h"

// External includes
#include <unordered_set>

namespace Donut
{
	class TSystemAllocator : public TAllocator
	{
	public:
		TSystemAllocator();
		~TSystemAllocator();

		// Allocate a memory chunk give a particular alignement
		virtual memory_block allocate(size_t size, char alignment = 4);

		// Frees a memory spot given
		virtual void deallocate(const memory_block& _blc);

		// Function that aswers to the question, has this bloc
		// been allocated by this allocator ?
		virtual bool owns(const memory_block& _blc);

	protected:
		std::unordered_set<void*> m_allocatedPointers;
	};
}

#endif // SYSTEM_ALLOCATOR_H