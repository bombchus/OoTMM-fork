#include <combo.h>
#include <combo/custom.h>

void Shader_Opa0_Xlu1(void*, s16);
void Shader_Opa0_Xlu12(void*, s16);
void Shader_Opa0(void*, s16);
void Shader_Opa01(void*, s16);
void Shader_Opa1023(void*, s16);
void Shader_Opa10_Xlu2(void*, s16);
void Shader_Opa10_Xlu234(void*, s16);
void Shader_Opa10_Xlu32(void*, s16);
void Shader_Opa10234567(void*, s16);
void Shader_Xlu01(void*, s16);
void Shader_BlueFire(void*, s16);
void Shader_BombchuMask(void*, s16);
void Shader_Compass(void*, s16);
void Shader_DekuNut(void*, s16);
void Shader_Fairy(void*, s16);
void Shader_Fish(void*, s16);
void Shader_GiantKnife(void*, s16);
void Shader_GS(void*, s16);
void Shader_Heart(void*, s16);
void Shader_Medallion(void*, s16);
void Shader_MirrorShield(void*, s16);
void Shader_Poe(void*, s16);
void Shader_Potion(void*, s16);
void Shader_Rupee(void*, s16);
void Shader_Scale(void*, s16);
void Shader_SoldOut(void*, s16);
void Shader_Spell(void*, s16);
void Shader_MoonTear(void*, s16);

/* Custom Shaders */
void Shader_CustomNote(GameState* gs, u16 shaderId)
{
    const Shader* shader;
    u32 c;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    shader = &kShaders[shaderId];
    c = shader->lists[0];
    r = (c >> 24) & 0xff;
    g = (c >> 16) & 0xff;
    b = (c >> 8) & 0xff;
    a = (c >> 0) & 0xff;

    OPEN_DISPS(gs->gfx);
    InitListPolyXlu(gs->gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, shader->lists[1]);
    CLOSE_DISPS();
}


#if defined(GAME_OOT)

void ConvertMatrix(const float* in, u16* out);

static const float kMatrixRot[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, -1.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
};

static void* pushMatrix(GfxContext* gfx, const float* mat)
{
    void* end = gfx->polyOpa.end;
    end = (char*)end - 0x40;
    gfx->polyOpa.end = end;

    ConvertMatrix(mat, end);

    return end;
}

void Shader_SpiritualStones(GameState* gs, u16 shaderId)
{
    u16 mat[0x20];

    const Shader* shader;

    float fc = 0;

    shader = &kShaders[shaderId];
    OPEN_DISPS(gs->gfx);

    /* Matrix setup */
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_XLU_DISP++, pushMatrix(gs->gfx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, pushMatrix(gs->gfx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    gSPSegment(POLY_XLU_DISP++, 9, GetSegment(gs->gfx, 0, fc * 2, fc * -6, 0x20, 0x20, 1, fc, fc * -2, 0x20, 0x20));
    InitListPolyXlu(gs->gfx);
    gSPDisplayList(POLY_XLU_DISP++, shader->lists[0]);

    gSPSegment(POLY_OPA_DISP++, 8, GetSegment(gs->gfx, 0, fc * 2, fc * -6, 0x20, 0x20, 1, fc, fc * -2, 0x20, 0x20));
    InitListPolyOpa(gs->gfx);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[1]);

    CLOSE_DISPS();
}

#else

void Shader_SpiritualStones(GameState* gs, u16 shaderId)
{

}

#endif

const Shader kShaders[256] = {
#if defined(GAME_OOT)
# include "data/oot/shaders.inc"
# include "data/mm/shaders.inc"
#endif

#if defined(GAME_MM)
# include "data/mm/shaders.inc"
# include "data/oot/shaders.inc"
#endif
};
