// When a user requests data we find a free block. Split
// the free block giving the user a pointer the requested 
// data. Update the header of the remaining data. When the 
// user calls free we add it back to free list. If blocks 
// next to them are also free it merges into one larger free
// block
struct Header {
	size_t size;
	Header* next;
}
