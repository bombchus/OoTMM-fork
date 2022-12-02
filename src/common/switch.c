#include <combo.h>

typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

#if defined(GAME_OOT)
# define FOREIGN_OFF    0x80000
# define FOREIGN_CART   (0x10001000 | MM_BASE)
# define FOREIGN_SIZE   0x19500
#else
# define FOREIGN_OFF    0x400
# define FOREIGN_CART   0x10001000
# define FOREIGN_SIZE   0x6430
#endif
#define FOREIGN_DRAM   (0x80000000 | FOREIGN_OFF)

static void waitSubsystems(void)
{
    u32 tmp;

    for (;;)
    {
        tmp = IO_READ(SP_STATUS_REG);
        if (tmp & 1)
            break;
    }

    for (;;)
    {
        tmp = IO_READ(SP_DMA_BUSY_REG);
        if (!(tmp & 1))
            break;
    }
    IO_WRITE(SP_STATUS_REG, 0xaaaaae);

    for (;;)
    {
        tmp = IO_READ(DPC_STATUS_REG);
        if (!(tmp & 0x170))
            break;
    }
    IO_WRITE(MI_BASE_REG, 0x800);

    for (;;)
    {
        tmp = IO_READ(AI_STATUS_REG);
        if (!(tmp & 0x40000000))
            break;
    }
    IO_WRITE(AI_STATUS_REG, 0);

    for (;;)
    {
        tmp = IO_READ(PI_STATUS_REG);
        if (!(tmp & 3))
            break;
    }
    IO_WRITE(PI_STATUS_REG, 2);

    for (;;)
    {
        tmp = IO_READ(SI_STATUS_REG);
        if (!(tmp & 3))
            break;
    }
    IO_WRITE(SI_STATUS_REG, 0);

    tmp = IO_READ(VI_CONTROL_REG);
    IO_WRITE(VI_CONTROL_REG, tmp & ~0x3);
    tmp = IO_READ(VI_CURRENT_REG);
    IO_WRITE(VI_CURRENT_REG, tmp);
}

NORETURN void comboGameSwitch2(void);

NORETURN void comboGameSwitch(void)
{
#if defined(GAME_OOT)
    gComboCtx.saveIndex = gSaveContext.fileIndex;
#endif

    comboWriteSave();
    comboDisableInterrupts();
    waitSubsystems();
    comboGameSwitch2();
}

NORETURN void comboGameSwitch3(void)
{
    comboInvalDCache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboDma_NoCacheInval((void*)FOREIGN_OFF, FOREIGN_CART, FOREIGN_SIZE);
    comboInvalDCache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboInvalICache((void*)FOREIGN_DRAM, FOREIGN_SIZE);

    comboExportContext();
    ((EntryPoint)FOREIGN_DRAM)();
}
