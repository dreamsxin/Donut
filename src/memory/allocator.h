#ifndef ALLOCATOR_H
#define ALLOCATOR_H

// External includes
#if __posix__
#include <unistd.h>
#elif WIN32
#endif

namespace Donut
{
	// Memory allocation
	struct memory_block
	{
		void* ptr;
		size_t size;
	};

	// Generic allocator that implements the basic function of an allocator
	class TAllocator
	{
	public:
		TAllocator();
		virtual ~TAllocator();

		// Allocate a memory chunk give a particular alignement
		virtual memory_block allocate(size_t size, char alignment = 4) = 0;

		// Frees a memory spot given
		virtual void deallocate(void* _ptr) = 0;

		// Function that aswers to the question, has this bloc
		// been allocated by this allocator ?
		virtual bool owns(void* _ptr) = 0;
	};
}

#endif // ALLOCATOR_H