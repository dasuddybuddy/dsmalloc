i#include <iostream>
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
		block->free = 0;
		block->magic = 0x12345678;
		return block;
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
	
	if (!global_base) {
		block->request_space(nullptr, size);
		if (!block) {
			return nullptr;
		}
		global_base = block;
	} else {
		block->find_free_block(last, size);
		if (!block) {
			block->request_space(last, size);
			if (!block) {
				return nullptr;
			}
		} else {
			block->free = 0;
			block->magic = 0x77777777;
		}
	}
}

void *free(void *ptr) {
	return;	
}

int main() {
	return 0;
}
