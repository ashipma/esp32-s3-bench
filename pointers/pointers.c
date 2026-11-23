#include <stdio.h>    // printf
#include <stdint.h>   // uint8_t, uint16_t, uint32_t, uintptr_t
#include <inttypes.h> // PRIxPTR, PRIuPTR

int main(void) {
    uint32_t *p32 = (uint32_t *)0x1000;
    //p32 = 0x1000, sizeof(p32) = 8, sizeof(*p32) = 4
    printf("p32 = %p, sizeof(p32) = %lu, sizeof(*p32) = %lu\n", (void *)p32, sizeof(p32), sizeof(*p32));
    
    //p32 + 1 = 0x1004
    printf("p32 + 1 = %p\n", (void *)(p32 + 1));
    
    uintptr_t n = 0x1000;
    // n + 1 = 0x1001
    printf("n + 1 = 0x%" PRIxPTR "\n", n + 1);
    
    // as decimal: 4096
    printf("as decimal: %" PRIuPTR "\n", n);
    
    
    printf("uint8_t -----\n");
    
    uint8_t *p8 = (uint8_t *)0x1000;
    uint16_t *p16 = (uint16_t *)0x1000;
    uintptr_t n1 = 0x1000;
    
    // p8 + 1 = 0x1001, sizeof(p8) = 8, sizeof(*p8) = 1
    printf("p8 + 1 = %p, sizeof(p8) = %lu, sizeof(*p8) = %lu\n", (void *)(p8 + 1), sizeof(p8), sizeof(*p8));
    
    // p16 + 1 = 0x1002, sizeof(p16) = 8, sizeof(*p16) = 2
    printf("p16 + 1 = %p, sizeof(p16) = %lu, sizeof(*p16) = %lu\n", (void *)(p16 + 1), sizeof(p16), sizeof(*p16));
    
    // n1 + 1 = 0x1001, sizeof(n1) = 8
    printf("n1 + 1 = 0x%" PRIxPTR ", sizeof(n1) = %lu\n", (n1 + 1), sizeof(n1));
    
    
    printf("arrays -----\n");
    uint32_t a[] = {1, 2, 3, 4, 5};
    
    // &a = 0x..., &a[3] = (&a) + 3 * sizeof(a[0]) (which is (&a) + 12), a[3] = 4, a + 3 = 4
    printf("&a = %p, &a[3] = %p, a[3] = %u, a + 3 = %u\n", &a, &a[3], a[3], *(a + 3));
    
    // &a[3] - &a[0] = (%ld) 3, = (%td) 3
    printf("&a[3] - &a[0] = (%%ld) %ld, = (%%td) %td\n", (&a[3] - &a[0]), (&a[3] - &a[0]));
    
    // (uintptr_t)&a[3] - (uintptr_t)&a[0] = 12
    printf("(uintptr_t)&a[3] - (uintptr_t)&a[0] = %" PRIuPTR "\n", (uintptr_t)&a[3] - (uintptr_t)&a[0]);
    
    // sizeof(a) = 20, sizeof(a[0]) = 4, sizeof(a) / sizeof(a[0]) = 5
    printf("sizeof(a) = %lu, sizeof(a[0]) = %lu, sizeof(a) / sizeof(a[0]) = %lu\n", sizeof(a), sizeof(a[0]), sizeof(a) / sizeof(a[0]));
    
    
    printf("sizeof -----\n");
    
    // sizeof(uint32_t) = 4, sizeof(uint32_t *) = 8, sizeof(uintptr_t) = 8
    printf("sizeof(uint32_t) = %zu, sizeof(uint32_t *) = %zu, sizeof(uintptr_t) = %zu\n", sizeof(uint32_t), sizeof(uint32_t *), sizeof(uintptr_t));
    
    return 0;
}
