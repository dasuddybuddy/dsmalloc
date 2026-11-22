#include <iostream>
#include <cstddef>

class block_meta {
	std::size_t size;
	block_meta *next;
	bool free;
	int magic; // For debugging only. TODO: remove this in non-debug mode.

	block_meta *find_free_block(block_meta **last, std::size_t size) {
		block_meta *current = global_base;
		while (current && !(current->free && current->size >= size)) {
			*last = current;
			current = current->next;
		}
		return current;
	}

	block_meta *request_space(block_meta* last, std::size_t size) {
		block_meta *block;
		block = sbrk(0);
		void *request = srbk(size + META_SIZE);
		static_assert((static_cast<void*>(block) == request) // Not thread safe. 
		if (request == (void*) -1) {
			return nullptr; // sbrk failed
		}

		if (last) { // NULL on first request
			last->next = block;
		}

		block->size = size;
		block->next = NULL;
		block->free = false;
		block->magic = 0x12345678;
		return block;
	}

	block_meta *get_block_ptr(void *ptr) {
		return (block_meta*)ptr - 1;
	}
}

inline constexpr std::size_t META_SIZE = sizeof(block_meta);
void *global_base = nullptr;


void *malloc(std::size_t size){
	block_meta *block;
	// TODO: align size?
	
	if (size <= 0) {
		return nullptr;
	}
	
	if (!global_base) { // First call.
		block->request_space(nullptr, size);
		if (!block) {
			return nullptr;
		}
		global_base = block;
	} else {
		block->find_free_block(last, size);
		if (!block) { // Failed to find free block.
			block->request_space(last, size);
			if (!block) {
				return nullptr;
			}
		} else { // Found free block
		// TODO : consider splitting block here.
			block->free = false;
			block->magic = 0x77777777;
		}
	}
}

void *free(void *ptr) {
	if (!ptr) {
		return;
	}
	// TODO: consider merging blocks once splitting blocks is implemented
	block_meta* block_ptr = get_block_ptr(ptr);
	static_assert(block_ptr->free == 0);
	static_assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
	block_ptr->free = 1;
	block_ptr->magic = 0x55555555;
}

void *realloc(void *ptr, std::size_t size) {
	if (!ptr) {
		// NULL ptr. realloc should act like malloc.
		return malloc(size);
	}
	
	block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr->size >= size) {
	// We have enough space. Could free some once we implement split.
	return ptr;
	}

	// Need to really realloc. Malloc new space and free old space.
	// Then copy old data to new space.
	void *new_ptr;
	new_ptr = malloc(size);
	if (!new_ptr) {
		return nullptr;
	}
	memcpy(new_ptr, ptr, block_ptr->size);
	delete ptr;
	return new_ptr;
}

void *calloc(std::size_t nelem, std::size_t elsize) {
	std::size_t sizze = nelem * elsize // TODO: check for overflow.
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

int main() {
	return 0;
}
