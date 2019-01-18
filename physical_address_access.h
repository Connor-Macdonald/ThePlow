#ifndef physical_address_map
#define physical_address_map

int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

#endif

