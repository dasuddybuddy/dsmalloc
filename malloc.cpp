#include <iostream>

class block_meta {
	size_t size;
	struct block_meta *next;
	int free;
	int magic; \\ For debugging only. TODO: remove this in non-debug mode.

	blocke_meta *find_free_block(block_meta **last, size_t size) {
		while (current && !(current->free && current->size >= size)
	}
}

inline constexpr std::size_t META_SIZE = sizeof(block_meta);
void *global_base = nullptr;


void *malloc(size_t size){
	void *p = sbrk(0); \\ returns pointer to current top of heap
	void request = sbrk(size); \\ increments heap by `size` and returns pointer to previous top of heap
	if (request == (void *) -1) { 
		return nullptr; \\ sbrk failed
	} else {
		static_assert(p == request); \\ Not thread safe
		return p;
	}
}

void *free(void *ptr) {
	
}

int main() {


}i
