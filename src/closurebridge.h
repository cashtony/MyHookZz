#ifndef closurebridge_h
#define closurebridge_h

#include <stdint.h>

#define PRIAVE

#ifndef REGISTER_STATE_STRUCT
#define REGISTER_STATE_STRUCT
#if defined(__arm64__) || defined(__aarch64__)
typedef union _FPReg {
    __int128_t q;
    struct {
        double d1;
        double d2;
    } d;
    struct {
        float f1;
        float f2;
        float f3;
        float f4;
    } f;
} FPReg;

typedef struct _RegState {
    uint64_t sp;

    union {
        uint64_t x[29];
        struct {
            uint64_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21,
                x22, x23, x24, x25, x26, x27, x28;
        } regs;
    } general;

    uint64_t fp;
    uint64_t lr;

    union {
        FPReg q[8];
        struct {
            FPReg q0, q1, q2, q3, q4, q5, q6, q7;
        } regs;
    } floating;
} RegState;
#elif defined(__arm__)
typedef struct _RegState {
    uint32_t sp;

    union {
        uint32_t r[13];
        struct {
            uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;
        } regs;
    } general;

    uint32_t lr;
} RegState;
#elif defined(__i386__)
typedef struct _RegState {
} RegState;
#elif defined(__x86_64__)
typedef struct _RegState {
} RegState;
#endif
#endif

typedef struct _ClosureBridgeData {
    void *user_code;
    void *user_data;
    void *redirect_trampoline;
} ClosureBridgeData;

typedef struct _ClosureBridgeTrampolineTable {
    void *entry;
    void *trampoline_page;
    uint16_t used_count;
    uint16_t free_count;

    struct _ClosureBridgeTrampolineTable *prev;
    struct _ClosureBridgeTrampolineTable *next;
} ClosureBridgeTrampolineTable;

typedef struct _DynamicClosureTrampoline {
    PRIAVE void *trampolineTo;

    void *user_code;
    void *user_data;
    void *redirect_trampoline;
} DynamicClosureTrampoline;

typedef struct _DynamicClosureTrampolineTable {
    void *entry;
    void *trampoline_page;
    void *data_page;
    uint16_t used_count;
    uint16_t free_count;

    struct _DynamicClosureTrampolineTable *prev;
    struct _DynamicClosureTrampolineTable *next;
} DynamicClosureTrampolineTable;

typedef void (*USER_CODE_CALL)(RegState *rs, ClosureBridgeData *cbd);

typedef void (*DYNAMIC_USER_CODE_CALL)(RegState *rs, DynamicClosureTrampoline *cbd);

ClosureBridgeData *ClosureBridgeAllocate(void *user_data, void *user_code);

DynamicClosureTrampoline *DynamicClosureTrampolineAllocate(void *user_data, void *user_code);

void closure_bridge_trampoline_template();

void closure_bridge_template();

void dynamic_closure_bridge_template();

void dynamic_closure_trampoline_table_page();

#endif