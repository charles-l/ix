#include "system.h"

unsigned int page_dir[1024] __attribute__((aligned(4096)));
unsigned int page_tbl[1024] __attribute__((aligned(4096)));

void page_init(void)
{
    // TODO: dynamically get and free pages
    for(int i = 0; i < 1024; i++) page_dir[i] = 0x02;
    for(int i = 0; i < 1024; i++) page_tbl[i] = (i * 0x1000) | 3;
    page_dir[0] = ((unsigned int) page_tbl) | 3;

    load_page_dir(page_dir);
    enable_paging();
}
