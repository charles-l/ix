#include "system.h"

extern uint32 end;
uint32 placement_addr;
#define INDEX_FROM_BIT(a)  (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

uint32 kmalloc_full(uint32 sz, int align, uint32 *phys)
{
    if(align == 1 && (placement_addr & 0xFFFFF000))
    {
        placement_addr &= 0xFFFFF000;
        placement_addr += 0x1000;
    }
    if(phys)
    {
        *phys = placement_addr;
    }
    uint32 tmp = placement_addr;
    placement_addr += sz;
    return tmp;
}

uint32 kmalloc   (uint32 sz)               {kmalloc_full(sz, 0, NULL);};
uint32 kmalloc_a (uint32 sz)               {kmalloc_full(sz, 1, NULL);};
uint32 kmalloc_p (uint32 sz, uint32 *phys) {kmalloc_full(sz, 0, phys);};
uint32 kmalloc_ap(uint32 sz, uint32 *phys)  {kmalloc_full(sz, 1, phys);};

typedef struct page
{
    uint32 present  : 1;
    uint32 rw       : 1;
    uint32 user     : 1;
    uint32 accessed : 1;
    uint32 dirty    : 1;
    uint32 unused   : 7;
    uint32 frame    : 20;
} page_t;

typedef struct page_tbl
{
    page_t pages[1024];
} page_tbl_t;

typedef struct page_dir
{
    page_tbl_t *tables[1024];
    uint32 tables_phys[1024];
    uint32 physical_addr;
} page_dir_t;

uint32 *frames;
uint32 nframes;

static void set_frame(uint32 frame_addr)
{
    uint32 frame = frame_addr/0x1000;
    uint32 idx = INDEX_FROM_BIT(frame);
    uint32 off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32 frame_addr)
{
    uint32 frame = frame_addr/0x1000;
    uint32 idx = INDEX_FROM_BIT(frame);
    uint32 off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static uint32 test_frame(uint32 frame_addr)
{
    uint32 frame = frame_addr/0x1000;
    uint32 idx = INDEX_FROM_BIT(frame);
    uint32 off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

static uint32 first_frame()
{
    uint32 i, j;
    for(i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if(frames[i] != 0xFFFFFFFF)
        {
            for(j = 0; j < 32; j++)
            {
                uint32 t = 0x1 << j;
                if( !(frames[i] & t))
                {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if(page->frame != 0)
    {
        return;
    } else {
        uint32 idx = first_frame();
        if(idx == (uint32) -1)
        {
            // no free frames
        }
        set_frame(idx * 0x1000);
        page->present = 1;
        page->rw      = (is_writeable) ? 1 : 0;
        page->user    = (is_kernel)    ? 1 : 0;
        page->frame   = idx;
    }
}

void free_frame(page_t *page)
{
    uint32 frame;
    if( !(frame = page->frame))
    {
        return;
    } else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

page_t *get_page(uint32 addr, int mk, page_dir_t *dir)
{
    addr /= 0x1000;
    uint32 tbl_idx = addr / 1024;
    if(dir->tables[tbl_idx])
    {
        return &dir->tables[tbl_idx]->pages[addr % 1024];
    } else if (mk) {
        uint32 tmp;
        dir->tables[tbl_idx] = (page_tbl_t *) kmalloc_ap(sizeof(page_tbl_t), &tmp);
        memset(dir->tables[tbl_idx], 0, 0x1000);
        dir->tables_phys[tbl_idx] = tmp | 0x7;
        return &dir->tables[tbl_idx]->pages[addr % 1024];
    } else {
        return 0;
    }
}

void page_init()
{
    placement_addr = &end;
    uint32 mem_end_page = 0x1000000;
    nframes = mem_end_page / 0x1000;
    frames  = (uint32 *)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    page_dir_t *kernel_dir = (page_dir_t *) kmalloc_a(sizeof(page_dir_t));
    memset(kernel_dir, 0, sizeof(page_dir_t));

    int i = 0;
    while(i < placement_addr)
    {
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i += 0x1000;
    }
    switch_page_dir(kernel_dir);
}

void switch_page_dir(page_dir_t *dir)
{
    load_page_dir(dir->tables_phys);
    enable_paging();
}
