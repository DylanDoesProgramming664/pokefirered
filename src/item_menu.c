#include "global.h"
#include "gflib.h"
#include "bag.h"
#include "battle_controllers.h"
#include "berry_pouch.h"
#include "decompress.h"
#include "field_player_avatar.h"
#include "graphics.h"
#include "help_system.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "link.h"
#include "list_menu.h"
#include "mail_data.h"
#include "menu.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon_storage_system.h"
#include "scanline_effect.h"
#include "strings.h"
#include "tm_case.h"
#include "constants/items.h"
#include "constants/songs.h"

#define FREE_IF_SET(ptr) ({ if (ptr) Free(ptr); })

struct BagMenuAlloc
{
    MainCallback exitCB;
    u8 field_04;
    u8 field_05_0:4;
    u8 itemMenuIcon:2;
    u8 field_05_6:2;
    u16 contextMenuSelectedItem;
    u8 pocketScrollArrowsTask;
    u8 pocketSwitchArrowsTask;
    u8 nItems[3];
    u8 maxShowed[3];
    u8 data[4];
};

EWRAM_DATA struct BagStruct gUnknown_203ACFC = {};
EWRAM_DATA struct BagMenuAlloc * gUnknown_203AD10 = NULL;
EWRAM_DATA void * gUnknown_203AD14 = NULL;
EWRAM_DATA struct ListMenuItem * gUnknown_203AD18 = NULL;
EWRAM_DATA u8 (*gUnknown_203AD1C)[19] = NULL;
EWRAM_DATA u8 gUnknown_203AD20[4] = {};
EWRAM_DATA const u8 *gUnknown_203AD24 = NULL;
EWRAM_DATA u8 gUnknown_203AD28 = 0;

void sub_8107F10(void);
bool8 sub_8107F3C(void);
void sub_810813C(void);
void sub_8108178(u8 taskId);
void sub_81081AC(void);
bool8 sub_81081D0(void);
bool8 sub_8108240(void);
u8 sub_8108388(u8 location);
bool8 sub_81083F4(void);
void sub_810842C(u8 pocket);
void sub_8108560(u8 *dest, u16 itemId);
void sub_81085A4(s32 itemIndex, bool8 onInit, struct ListMenu *list);
void sub_8108654(u8 windowId, s32 itemId, u8 y);
void bag_menu_print_cursor(u8 y, u8 colorIdx);
void sub_81087EC(void);
void sub_8108818(s32 itemIndex);
void sub_8108888(void);
void sub_81088D8(void);
void sub_810899C(void);
void sub_8108A68(void);
void sub_8108A84(void);
void sub_8108B04(void);
void sub_8108B8C(u8 taskId);
void sub_8108CFC(u8 taskId);
void sub_8108C10(void);
void sub_8108E54(void);
void sub_8108F0C(u8 taskId);
void sub_81090DC(u8 taskId);
void sub_8109140(u32 a0);
u8 sub_8109168(u8 taskId, u8 pocketId);
void sub_81091D0(u8 taskId, s16 direction, bool16 a2);
void sub_8109298(u8 taskId);
void sub_81093B8(u8 taskId, s16 itemIndex);
void sub_81094B0(u8 taskId);
void sub_81095AC(u8 taskId, u32 itemIndex);
void sub_810967C(u8 taskId, u32 itemIndex);
void sub_8109854(u8 a0);
void sub_8109BB8(u8 taskId);
void sub_8109BE4(u8 taskId);
void Task_ItemMenuAction_Use(u8 taskId);
void Task_ItemMenuAction_Toss(u8 taskId);
void sub_8109D38(u8 taskId);
void sub_8109DB0(u8 taskId);
void sub_8109DEC(u8 taskId);
void sub_8109EA8(u8 taskId);
void sub_8109F44(u8 taskId);
void Task_ItemMenuAction_ToggleSelect(u8 taskId);
void Task_ItemMenuAction_Give(u8 taskId);
void sub_810A170(u8 taskId);
void Task_ItemMenuAction_Cancel(u8 taskId);
void sub_810A18C(u8 taskId);
void sub_810A1D0(u8 taskId);
void sub_810A324(u8 taskId);
void sub_810A370(u8 taskId);
void sub_810A434(void);
void sub_810A448(void);
void sub_810A45C(void);
void sub_810A468(u8 taskId);
void sub_810A52C(void);
void sub_810A540(void);
void sub_810A554(void);
void sub_810A568(u8 taskId);
void sub_810A720(u8 taskId);
void sub_810A940(u8 taskId);
void sub_810AB40(u8 taskId);
bool8 sub_810ADAC(void);
void sub_810AF9C(u8 taskId);
void sub_810B1D4(u8 taskId);
void sub_810B378(u8 taskId);
void sub_810B4BC(u8 taskId);
void sub_810B5D4(u8 taskId);

const struct BgTemplate gUnknown_8452CF4[2] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }
};

const u8 *const sPocketNames[] = {
    gText_Items2,
    gText_KeyItems2,
    gText_PokeBalls2
};

const u16 gUnknown_8452D08[][18] = {
    [ITEMMENUACTION_USE] = INCBIN_U16("graphics/item_menu/bagmap_0.bin"),
    [ITEMMENUACTION_TOSS] = INCBIN_U16("graphics/item_menu/bagmap_1.bin"),
    [ITEMMENUACTION_REGISTER] = INCBIN_U16("graphics/item_menu/bagmap_2.bin"),
    [ITEMMENUACTION_GIVE] = INCBIN_U16("graphics/item_menu/bagmap_3.bin"),
    [ITEMMENUACTION_CANCEL] = INCBIN_U16("graphics/item_menu/bagmap_4.bin"),
    [ITEMMENUACTION_BATTLE_USE] = INCBIN_U16("graphics/item_menu/bagmap_5.bin"),
    [ITEMMENUACTION_CHECK] = INCBIN_U16("graphics/item_menu/bagmap_6.bin"),
    [ITEMMENUACTION_OPEN] = INCBIN_U16("graphics/item_menu/bagmap_7.bin"),
    [ITEMMENUACTION_OPEN_BERRIES] = INCBIN_U16("graphics/item_menu/bagmap_8.bin"),
    [ITEMMENUACTION_WALK] = INCBIN_U16("graphics/item_menu/bagmap_9.bin"),
    [ITEMMENUACTION_DESELECT] = INCBIN_U16("graphics/item_menu/bagmap_A.bin"),
    [ITEMMENUACTION_DUMMY] = INCBIN_U16("graphics/item_menu/bagmap_B.bin")
};

const struct MenuAction gUnknown_8452EB8[] = {
    [ITEMMENUACTION_USE] = {gOtherText_Use, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_TOSS] = {gOtherText_Toss, {.void_u8 = Task_ItemMenuAction_Toss}},
    [ITEMMENUACTION_REGISTER] = {gOtherText_Register, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_GIVE] = {gOtherText_Give, {.void_u8 = Task_ItemMenuAction_Give}},
    [ITEMMENUACTION_CANCEL] = {gFameCheckerText_Cancel, {.void_u8 = Task_ItemMenuAction_Cancel}},
    [ITEMMENUACTION_BATTLE_USE] = {gOtherText_Use, {.void_u8 = sub_810A324}},
    [ITEMMENUACTION_CHECK] = {gOtherText_Check, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN] = {gOtherText_Open, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN_BERRIES] = {gOtherText_Open, {.void_u8 = sub_810A324}},
    [ITEMMENUACTION_WALK] = {gOtherText_Walk, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_DESELECT] = {gOtherText_Deselect, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_DUMMY] = {gString_Dummy, {.void_u8 = NULL}}
};

const u8 gUnknown_8452F18[][4] = {
    {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL
    }, {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_REGISTER,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    }, {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    }
};

const u8 gUnknown_8452F24[] = {
    ITEMMENUACTION_CHECK,
    ITEMMENUACTION_GIVE,
    ITEMMENUACTION_TOSS,
    ITEMMENUACTION_CANCEL
};

const u8 gUnknown_8452F28[][2] = {
    {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_CANCEL
    }, {
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    }, {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_CANCEL
    }
};

const u8 gUnknown_8452F2E[] = {
    ITEMMENUACTION_OPEN,
    ITEMMENUACTION_CANCEL
};

const u8 gUnknown_8452F30[] = {
    ITEMMENUACTION_BATTLE_USE,
    ITEMMENUACTION_CANCEL
};

const u8 gUnknown_8452F32[] = {
    ITEMMENUACTION_CANCEL,
    ITEMMENUACTION_DUMMY
};

const TaskFunc gUnknown_8452F34[] = {
    sub_8109BB8,
    sub_810A370,
    sub_810A568,
    sub_810AB40,
    sub_810A468,
    sub_8109BB8,
    NULL
};

const struct YesNoFuncTable gUnknown_8452F50 = {
    sub_8109EA8,
    sub_8109DB0
};

const struct YesNoFuncTable gUnknown_8452F58 = {
    sub_810A940,
    sub_810A720
};

const u8 gUnknown_8452F60[] = _("{COLOR_HIGHLIGHT_SHADOW RED TRANSPARENT GREEN}");

const u8 gUnknown_8452F66[] = _("{COLOR_HIGHLIGHT_SHADOW LIGHT_GRAY TRANSPARENT BLACK}");

const struct ScrollArrowsTemplate gUnknown_8452F6C = {
    .firstArrowType = SCROLL_ARROW_LEFT,
    .firstX = 8,
    .firstY = 72,
    .secondArrowType = SCROLL_ARROW_RIGHT,
    .secondX = 72,
    .secondY = 72,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 2,
    .tileTag = 111,
    .palTag = 111,
    .palNum = 0,
};

const u8 gUnknown_8452F7C[] = INCBIN_U8("graphics/interface/select_button.4bpp");

void GoToBagMenu(u8 location, u8 a1, MainCallback a2)
{
    u8 i;

    sub_81081AC();
    gUnknown_203AD10 = Alloc(sizeof(struct BagMenuAlloc));
    if (gUnknown_203AD10 == NULL)
        SetMainCallback2(a2);
    else
    {
        if (location != 11)
            gUnknown_203ACFC.location = location;
        if (a2 != NULL)
            gUnknown_203ACFC.bagCallback = a2;
        gUnknown_203AD10->exitCB = NULL;
        gUnknown_203AD10->field_04 = 0xFF;
        gUnknown_203AD10->itemMenuIcon = 0;
        gUnknown_203AD10->field_05_6 = 0;
        gUnknown_203AD10->pocketScrollArrowsTask = 0xFF;
        gUnknown_203AD10->pocketSwitchArrowsTask = 0xFF;
        if (location == 3)
            gUnknown_203AD10->field_05_0 = 1;
        else if (location == 6)
            gUnknown_203AD10->field_05_0 = 2;
        else
            gUnknown_203AD10->field_05_0 = 0;
        for (i = 0; i < 4; i++)
        {
            gUnknown_203AD10->data[i] = 0;
        }
        if (a1 == 0 || a1 == 1 || a1 == 2)
            gUnknown_203ACFC.pocket = a1;
        gTextFlags.autoScroll = FALSE;
        gSpecialVar_ItemId = ITEM_NONE;
        SetMainCallback2(sub_8107F10);
    }
}

void CB2_BagMenuFromStartMenu(void)
{
    GoToBagMenu(0, 3, CB2_ReturnToFieldWithOpenMenu);
}

void sub_8107ECC(void)
{
    GoToBagMenu(5, 3, SetCB2ToReshowScreenAfterMenu2);
}

void sub_8107EE0(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

void sub_8107EFC(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void sub_8107F10(void)
{
    while (1)
    {
        if ((u8)sub_80BF72C() == TRUE)
            break;
        if (sub_8107F3C() == TRUE)
            break;
        if (MenuHelpers_LinkSomething() == TRUE)
            break;
    }
}

bool8 sub_8107F3C(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = TRUE;
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        ResetItemMenuIconState();
        gMain.state++;
        break;
    case 6:
        if (!MenuHelpers_LinkSomething())
        {
            ResetTasks();
        }
        gMain.state++;
        break;
    case 7:
        if (sub_81081D0())
        {
            gUnknown_203AD10->data[0] = 0;
            gMain.state++;
        }
        else
        {
            sub_810813C();
            return TRUE;
        }
        break;
    case 8:
        if (sub_8108240() == TRUE)
            gMain.state++;
        break;
    case 9:
        sub_810B858();
        gMain.state++;
        break;
    case 10:
        sub_8108E54();
        sub_8108A68();
        sub_8108A84();
        gMain.state++;
        break;
    case 11:
        if (!sub_81083F4())
        {
            sub_810813C();
            return TRUE;
        }
        gMain.state++;
        break;
    case 12:
        sub_810842C(gUnknown_203ACFC.pocket);
        gMain.state++;
        break;
    case 13:
        if (gUnknown_203ACFC.location != 3)
            sub_81087EC();
        else
            sub_810B994();
        gMain.state++;
        break;
    case 14:
        taskId = sub_8108388(gUnknown_203ACFC.location);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[8] = 0;
        gMain.state++;
        break;
    case 15:
        sub_80984FC(gUnknown_203ACFC.pocket);
        gMain.state++;
        break;
    case 16:
        sub_8108888();
        sub_81088D8();
        gMain.state++;
        break;
    case 17:
        sub_80985E4();
        gMain.state++;
        break;
    case 18:
        sub_8108C10();
        gMain.state++;
        break;
    case 19:
        if (gUnknown_203ACFC.location == 3)
            HelpSystem_SetSomeVariable2(29);
        else
            HelpSystem_SetSomeVariable2(9);
        gPaletteFade.bufferTransferDisabled = FALSE;
        gMain.state++;
        break;
    default:
        SetVBlankCallback(sub_8107EFC);
        SetMainCallback2(sub_8107EE0);
        return TRUE;
    }
    return FALSE;
}

void sub_810813C(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_BLACK);
    CreateTask(sub_8108178, 0);
    SetVBlankCallback(sub_8107EFC);
    SetMainCallback2(sub_8107EE0);
}

void sub_8108178(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(gUnknown_203ACFC.bagCallback);
        sub_8108B04();
        DestroyTask(taskId);
    }
}

void sub_81081AC(void)
{
    gUnknown_203AD10 = NULL;
    gUnknown_203AD14 = NULL;
    gUnknown_203AD18 = NULL;
    gUnknown_203AD1C = NULL;
}

bool8 sub_81081D0(void)
{
    void **buff;
    ResetAllBgsCoordinatesAndBgCntRegs();
    buff = &gUnknown_203AD14;
    *buff = Alloc(0x800);
    if (*buff == NULL)
        return FALSE;
    memset(*buff, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, gUnknown_8452CF4, NELEMS(gUnknown_8452CF4));
    SetBgTilemapBuffer(1, *buff);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    return TRUE;
}

bool8 sub_8108240(void)
{
    switch (gUnknown_203AD10->data[0])
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gUnknown_8E830CC, 0, 0, 0);
        gUnknown_203AD10->data[0]++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            if (gUnknown_203ACFC.location != 3)
                LZDecompressWram(gUnknown_8E832C0, gUnknown_203AD14);
            else
                LZDecompressWram(gUnknown_8E83444, gUnknown_203AD14);
            gUnknown_203AD10->data[0]++;
        }
        break;
    case 2:
        LoadCompressedPalette(gUnknown_8E835B4, 0x00, 0x60);
        if (!sub_810ADAC() && gSaveBlock2Ptr->playerGender != MALE)
            LoadCompressedPalette(gUnknown_8E83604, 0x00, 0x20);
        gUnknown_203AD10->data[0]++;
        break;
    case 3:
        if (sub_810ADAC() == TRUE || gSaveBlock2Ptr->playerGender == MALE)
            LoadCompressedSpriteSheet(&gUnknown_83D41E4);
        else
            LoadCompressedSpriteSheet(&gUnknown_83D41EC);
        gUnknown_203AD10->data[0]++;
        break;
    case 4:
        LoadCompressedSpritePalette(&gUnknown_83D41F4);
        gUnknown_203AD10->data[0]++;
        break;
    case 5:
        LoadCompressedSpriteSheet(&gBagSwapSpriteSheet);
        gUnknown_203AD10->data[0]++;
        break;
    default:
        LoadCompressedSpritePalette(&gBagSwapSpritePalette);
        gUnknown_203AD10->data[0] = 0;
        return TRUE;
    }

    return FALSE;
}

u8 sub_8108388(u8 location)
{
    switch (location)
    {
    case 6:
        return CreateTask(sub_810AF9C, 0);
    case 9:
        return CreateTask(sub_810B1D4, 0);
    case 10:
        return CreateTask(sub_810B5D4, 0);
    case 7:
        return CreateTask(sub_810B4BC, 0);
    case 8:
        return CreateTask(sub_810B378, 0);
    default:
        return CreateTask(sub_8108F0C, 0);
    }
}

bool8 sub_81083F4(void)
{
    gUnknown_203AD18 = Alloc(43 * sizeof(struct ListMenuItem));
    if (gUnknown_203AD18 == NULL)
        return FALSE;
    gUnknown_203AD1C = Alloc(43 * 19);
    if (gUnknown_203AD1C == NULL)
        return FALSE;
    return TRUE;
}

void sub_810842C(u8 pocket)
{
    u16 i;
    struct BagPocket * bagPocket = &gBagPockets[pocket];
    for (i = 0; i < gUnknown_203AD10->nItems[pocket]; i++)
    {
        sub_8108560(gUnknown_203AD1C[i], bagPocket->itemSlots[i].itemId);
        gUnknown_203AD18[i].label = gUnknown_203AD1C[i];
        gUnknown_203AD18[i].index = i;
    }
    StringCopy(gUnknown_203AD1C[i], gUnknown_8452F60);
    StringAppend(gUnknown_203AD1C[i], gFameCheckerText_Cancel);
    gUnknown_203AD18[i].label = gUnknown_203AD1C[i];
    gUnknown_203AD18[i].index = i;
    gMultiuseListMenuTemplate.items = gUnknown_203AD18;
    gMultiuseListMenuTemplate.totalItems = gUnknown_203AD10->nItems[pocket] + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 1;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = gUnknown_203AD10->maxShowed[pocket];
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = sub_81085A4;
    gMultiuseListMenuTemplate.itemPrintFunc = sub_8108654;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

void sub_8108560(u8 *dest, u16 itemId)
{
    if (itemId == ITEM_TM_CASE || itemId == ITEM_BERRY_POUCH)
        StringCopy(dest, gUnknown_8452F66);
    else
        StringCopy(dest, gUnknown_8452F60);
    StringAppend(dest, ItemId_GetName(itemId));
}

void sub_81085A4(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (onInit != TRUE)
    {
        PlaySE(SE_BAG1);
        sub_8098580();
    }
    if (gUnknown_203AD10->field_04 == 0xFF)
    {
        DestroyItemMenuIcon(gUnknown_203AD10->itemMenuIcon ^ 1);
        if (gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket] != itemIndex)
            CreateItemMenuIcon(BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, itemIndex), gUnknown_203AD10->itemMenuIcon);
        else
            CreateItemMenuIcon(ITEM_N_A, gUnknown_203AD10->itemMenuIcon);
        gUnknown_203AD10->itemMenuIcon ^= 1;
        if (gUnknown_203AD10->field_05_6 == 0)
            sub_8108818(itemIndex);
    }
}

void sub_8108654(u8 windowId, s32 itemId, u8 y)
{
    u16 bagItemId;
    u16 bagItemQuantity;
    if (gUnknown_203AD10->field_04 != 0xFF)
    {
        if (gUnknown_203AD10->field_04 == (u8)itemId)
            bag_menu_print_cursor(y, 2);
        else
            bag_menu_print_cursor(y, 0xFF);
    }
    if (itemId != -2 && gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket] != itemId)
    {
        bagItemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, itemId);
        bagItemQuantity = BagGetQuantityByPocketPosition(gUnknown_203ACFC.pocket + 1, itemId);
        if (gUnknown_203ACFC.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(bagItemId))
        {
            ConvertIntToDecimalStringN(gStringVar1, bagItemQuantity, STR_CONV_MODE_RIGHT_ALIGN, 3);
            StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
            sub_810B8F0(windowId, 0, gStringVar4, 0x6e, y, 0, 0, 0xFF, 1);
        }
        else if (gSaveBlock1Ptr->registeredItem != ITEM_NONE && gSaveBlock1Ptr->registeredItem == bagItemId)
        {
            BlitBitmapToWindow(windowId, gUnknown_8452F7C, 0x70, y, 0x18, 0x10);
        }
    }
}

void bag_menu_print_cursor_(u8 taskId, u8 colorIdx)
{
    bag_menu_print_cursor(ListMenuGetYCoordForPrintingArrowCursor(taskId), colorIdx);
}

void bag_menu_print_cursor(u8 y, u8 colorIdx)
{
    if (colorIdx == 0xFF)
    {
        FillWindowPixelRect(0, PIXEL_FILL(0), 1, y, GetMenuCursorDimensionByFont(2, 0), GetMenuCursorDimensionByFont(2, 1));
    }
    else
    {
        sub_810B8F0(0, 2, gFameCheckerText_ListMenuCursor, 1, y, 0, 0, 0, colorIdx);
    }
}

void sub_81087EC(void)
{
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    sub_810B958(sPocketNames[gUnknown_203ACFC.pocket], gUnknown_203ACFC.pocket);
}

void sub_8108818(s32 itemIndex)
{
    const u8 *description;
    if (itemIndex != gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket])
        description = ItemId_GetDescription(BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, itemIndex));
    else
        description = gText_CloseBag;
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    sub_810B8F0(1, 2, description, 0, 3, 2, 0, 0, 0);
}

void sub_8108888(void)
{
    gUnknown_203AD10->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        160,
        8,
        104,
        gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket] - gUnknown_203AD10->maxShowed[gUnknown_203ACFC.pocket] + 1,
        110,
        110,
        &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket]
    );
}

void sub_81088D8(void)
{
    if (gUnknown_203AD10->field_05_0 != 1)
    {
        gUnknown_203AD10->pocketSwitchArrowsTask = AddScrollIndicatorArrowPair(&gUnknown_8452F6C, &gUnknown_203ACFC.pocket);
    }
}

void sub_8108908(void)
{
    gUnknown_203AD10->contextMenuSelectedItem = 1;
    gUnknown_203AD10->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        152,
        72,
        104,
        2,
        110,
        110,
        &gUnknown_203AD10->contextMenuSelectedItem
    );
}

void sub_8108940(void)
{
    gUnknown_203AD10->contextMenuSelectedItem = 1;
    gUnknown_203AD10->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        212,
        120,
        152,
        2,
        110,
        110,
        &gUnknown_203AD10->contextMenuSelectedItem
    );
}

void sub_8108978(void)
{
    if (gUnknown_203AD10->pocketScrollArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(gUnknown_203AD10->pocketScrollArrowsTask);
        gUnknown_203AD10->pocketScrollArrowsTask = 0xFF;
    }
    sub_810899C();
}

void sub_810899C(void)
{
    if (gUnknown_203AD10->pocketSwitchArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(gUnknown_203AD10->pocketSwitchArrowsTask);
        gUnknown_203AD10->pocketSwitchArrowsTask = 0xFF;
    }
}

void sub_81089BC(void)
{
    u8 i;
    gUnknown_203ACFC.pocket = POCKET_ITEMS - 1;
    gUnknown_203ACFC.unk5 = 0;
    for (i = 0; i < 3; i++)
    {
        gUnknown_203ACFC.itemsAbove[i] = 0;
        gUnknown_203ACFC.cursorPos[i] = 0;
    }
}

void sub_81089F4(u8 pocketId)
{
    if (gUnknown_203ACFC.cursorPos[pocketId] != 0 && gUnknown_203ACFC.cursorPos[pocketId] + gUnknown_203AD10->maxShowed[pocketId] > gUnknown_203AD10->nItems[pocketId] + 1)
    {
        gUnknown_203ACFC.cursorPos[pocketId] = (gUnknown_203AD10->nItems[pocketId] + 1) - gUnknown_203AD10->maxShowed[pocketId];
    }
    if (gUnknown_203ACFC.cursorPos[pocketId] + gUnknown_203ACFC.itemsAbove[pocketId] >= gUnknown_203AD10->nItems[pocketId] + 1)
    {
        if (gUnknown_203AD10->nItems[pocketId] + 1 < 2)
            gUnknown_203ACFC.itemsAbove[pocketId] = 0;
        else
            gUnknown_203ACFC.itemsAbove[pocketId] = gUnknown_203AD10->nItems[pocketId];
    }
}

void sub_8108A68(void)
{
    u8 i;
    for (i = 0; i < 3; i++)
    {
        sub_81089F4(i);
    }
}

void sub_8108A84(void)
{
    u8 i;
    u8 j;

    for (i = 0; i < 3; i++)
    {
        if (gUnknown_203ACFC.itemsAbove[i] > 3)
        {
            for (j = 0; j <= gUnknown_203ACFC.itemsAbove[i] - 3; gUnknown_203ACFC.itemsAbove[i]--, gUnknown_203ACFC.cursorPos[i]++, j++)
            {
                if (gUnknown_203ACFC.cursorPos[i] + gUnknown_203AD10->maxShowed[i] == gUnknown_203AD10->nItems[i] + 1)
                    break;
            }
        }
    }
}

void sub_8108B04(void)
{
    FREE_IF_SET(gUnknown_203AD10);
    FREE_IF_SET(gUnknown_203AD14);
    FREE_IF_SET(gUnknown_203AD18);
    FREE_IF_SET(gUnknown_203AD1C);
    FreeAllWindowBuffers();
}

void ItemMenu_StartFadeToExitCallback(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = sub_8108B8C;
}

void sub_8108B8C(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active && FuncIsActiveTask(sub_8108CFC) != TRUE)
    {
        DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        if (gUnknown_203AD10->exitCB != NULL)
            SetMainCallback2(gUnknown_203AD10->exitCB);
        else
            SetMainCallback2(gUnknown_203ACFC.bagCallback);
        sub_8108978();
        sub_8108B04();
        DestroyTask(taskId);
    }
}

void sub_8108C10(void)
{
    u16 paldata = RGB_BLACK;
    u8 taskId;

    LoadPalette(&paldata, 0x00, 0x02);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0x3F);
    BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    if (gUnknown_203ACFC.unk5 == 1)
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
    }
    else
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
        taskId = CreateTask(sub_8108CFC, 0);
        gTasks[taskId].data[0] = 192;
        gTasks[taskId].data[1] = -16;
        gUnknown_203ACFC.unk5 = 1;
    }
}

void sub_8108CB4(void)
{

    u8 taskId = CreateTask(sub_8108CFC, 0);
    gTasks[taskId].data[0] = -16;
    gTasks[taskId].data[1] =  16;
    gUnknown_203ACFC.unk5 = 0;
}

void CB2_SetUpReshowBattleScreenAfterMenu(void)
{
    gUnknown_203ACFC.unk5 = 0;
}

void sub_8108CFC(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[0] += data[1];
    if (data[0] > 160)
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
    else
        SetGpuReg(REG_OFFSET_WIN0V, data[0]);
    if ((data[1] == 16 && data[0] == 160) || (data[1] == -16 && data[0] == 0))
        DestroyTask(taskId);
}

void MoveItemSlotInList(struct ItemSlot * itemSlots_, u32 from, u32 to_)
{
    // dumb assignments needed to match
    struct ItemSlot *itemSlots = itemSlots_;
    u32 to = to_;

    if (from != to)
    {
        s16 i, count;
        struct ItemSlot firstSlot = itemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                itemSlots[i] = itemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                itemSlots[i] = itemSlots[i - 1];
        }
        itemSlots[to] = firstSlot;
    }
}

void sub_8108DC8(u8 pocketId)
{
    u16 i;
    struct BagPocket * pocket = &gBagPockets[pocketId];
    BagPocketCompaction(pocket->itemSlots, pocket->capacity);
    gUnknown_203AD10->nItems[pocketId] = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        gUnknown_203AD10->nItems[pocketId]++;
    }
    if (gUnknown_203AD10->nItems[pocketId] + 1 > 6)
        gUnknown_203AD10->maxShowed[pocketId] = 6;
    else
        gUnknown_203AD10->maxShowed[pocketId] = gUnknown_203AD10->nItems[pocketId] + 1;
}

void sub_8108E54(void)
{
    u8 i;
    for (i = 0; i < 3; i++)
        sub_8108DC8(i);
}

void DisplayItemMessageInBag(u8 taskId, u8 fontId, const u8 * string, TaskFunc followUpFunc)
{
    s16 *data = gTasks[taskId].data;
    data[10] = sub_810BA70(5);
    FillWindowPixelBuffer(data[10], PIXEL_FILL(1));
    DisplayMessageAndContinueTask(taskId, data[10], 0x06D, 0x0D, fontId, GetTextSpeedSetting(), string, followUpFunc);
    ScheduleBgCopyTilemapToVram(0);
}

void ItemMenu_SetExitCallback(MainCallback cb)
{
    gUnknown_203AD10->exitCB = cb;
}

u8 sub_8108EEC(u8 a0)
{
    return gUnknown_203ACFC.cursorPos[a0] + gUnknown_203ACFC.itemsAbove[a0];
}

void sub_8108F0C(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 cursorPos;
    u16 itemsAbove;
    s32 input;

    if (gPaletteFade.active)
        return;
    if (FuncIsActiveTask(sub_8108CFC) == TRUE)
        return;
    if ((u8)sub_80BF72C() == TRUE)
        return;
    switch (sub_8109168(taskId, gUnknown_203ACFC.pocket))
    {
    case 1:
        sub_81091D0(taskId, -1, FALSE);
        return;
    case 2:
        sub_81091D0(taskId,  1, FALSE);
        return;
    default:
        if (JOY_NEW(SELECT_BUTTON) && gUnknown_203ACFC.location == 0)
        {
            ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
            if (cursorPos + itemsAbove != gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket])
            {
                PlaySE(SE_SELECT);
                sub_81093B8(taskId, cursorPos + itemsAbove);
                return;
            }
        }
        break;
    }
    input = ListMenu_ProcessInput(data[0]);
    ListMenuGetScrollAndRow(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        return;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        gSpecialVar_ItemId = ITEM_NONE;
        sub_8108CB4();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        break;
    default:
        PlaySE(SE_SELECT);
        if (input == gUnknown_203AD10->nItems[gUnknown_203ACFC.pocket])
        {
            gSpecialVar_ItemId = ITEM_NONE;
            sub_8108CB4();
            gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        }
        else
        {
            sub_8108978();
            bag_menu_print_cursor_(data[0], 2);
            data[1] = input;
            data[2] = BagGetQuantityByPocketPosition(gUnknown_203ACFC.pocket + 1, input);
            gSpecialVar_ItemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, input);
            gTasks[taskId].func = sub_81090DC;
        }
        break;
    }
}

void sub_81090DC(u8 taskId)
{
    sub_8109140(1);
    gUnknown_8452F34[gUnknown_203ACFC.location](taskId);
}

void sub_810910C(u8 taskId)
{
    sub_8109140(0);
    sub_8108888();
    sub_81088D8();
    gTasks[taskId].func = sub_8108F0C;
}

void sub_8109140(u32 a0)
{
    SetBgTilemapPalette(1, 0, 14, 30, 6, a0 + 1);
    ScheduleBgCopyTilemapToVram(1);
}

u8 sub_8109168(u8 taskId, u8 pocketId)
{
    u8 lrState;
    if (gUnknown_203AD10->field_05_0 != 0)
        return 0;
    lrState = GetLRKeysState();
    if (JOY_NEW(DPAD_LEFT) || lrState == 1)
    {
        if (pocketId == POCKET_ITEMS - 1)
            return 0;
        PlaySE(SE_BAG2);
        return 1;
    }
    if (JOY_NEW(DPAD_RIGHT) || lrState == 2)
    {
        if (pocketId >= POCKET_POKE_BALLS - 1)
            return 0;
        PlaySE(SE_BAG2);
        return 2;
    }
    return 0;
}

void sub_81091D0(u8 taskId, s16 direction, bool16 a2)
{
    s16 *data = gTasks[taskId].data;
    data[13] = 0;
    data[12] = 0;
    data[11] = direction;
    if (!a2)
    {
        ClearWindowTilemap(0);
        ClearWindowTilemap(1);
        ClearWindowTilemap(2);
        DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        ScheduleBgCopyTilemapToVram(0);
        DestroyItemMenuIcon(gUnknown_203AD10->itemMenuIcon ^ 1);
        sub_8108978();
    }
    FillBgTilemapBufferRect_Palette0(1, 0x02D, 11, 1, 18, 12);
    ScheduleBgCopyTilemapToVram(1);
    sub_8098528(gUnknown_203ACFC.pocket + direction);
    SetTaskFuncWithFollowupFunc(taskId, sub_8109298, gTasks[taskId].func);
}

void sub_8109298(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!MenuHelpers_LinkSomething() && !sub_810ADAC())
    {
        switch (sub_8109168(taskId, gUnknown_203ACFC.pocket + data[11]))
        {
        case 1:
            gUnknown_203ACFC.pocket += data[11];
            SwitchTaskToFollowupFunc(taskId);
            sub_81091D0(taskId, -1, TRUE);
            return;
        case 2:
            gUnknown_203ACFC.pocket += data[11];
            SwitchTaskToFollowupFunc(taskId);
            sub_81091D0(taskId,  1, TRUE);
            return;
        }
    }
    switch (data[13])
    {
    case 0:
        if (data[12] != 0x7FFF)
        {
            data[12]++;
            sub_8109854(data[12]);
            if (data[12] == 12)
                data[12] = 0x7FFF;
        }
        if (data[12] == 0x7FFF)
            data[13]++;
        break;
    case 1:
        gUnknown_203ACFC.pocket += data[11];
        sub_81087EC();
        sub_810842C(gUnknown_203ACFC.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        PutWindowTilemap(1);
        PutWindowTilemap(2);
        ScheduleBgCopyTilemapToVram(0);
        sub_8108888();
        sub_81088D8();
        SwitchTaskToFollowupFunc(taskId);
        break;
    }
}

void sub_81093B8(u8 taskId, s16 itemIndex)
{
    u16 itemsAbove;
    u16 cursorPos;
    s16 *data = gTasks[taskId].data;
    ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
    ListMenuSetUnkIndicatorsStructField(data[0], 0x10, 1);
    data[1] = itemIndex;
    gUnknown_203AD10->field_04 = itemIndex;
    StringCopy(gStringVar1, ItemId_GetName(BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1])));
    StringExpandPlaceholders(gStringVar4, gOtherText_WhereShouldTheStrVar1BePlaced);
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    sub_810B8F0(1, 2, gStringVar4, 0, 3, 2, 0, 0, 0);
    sub_80986A8(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
    sub_8098660(0);
    sub_810899C();
    bag_menu_print_cursor_(data[0], 2);
    gTasks[taskId].func = sub_81094B0;
}

void sub_81094B0(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 input;
    u16 itemsAbove;
    u16 cursorPos;
    if ((u8)sub_80BF72C() == TRUE)
        return;
    input = ListMenu_ProcessInput(data[0]);
    ListMenuGetScrollAndRow(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    sub_80986A8(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
    if (JOY_NEW(SELECT_BUTTON))
    {
        PlaySE(SE_SELECT);
        gUnknown_203AD10->field_04 = 0xFF;
        ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
        sub_81095AC(taskId, cursorPos + itemsAbove);
        return;
    }
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        return;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        gUnknown_203AD10->field_04 = 0xFF;
        ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
        sub_810967C(taskId, cursorPos + itemsAbove);
        break;
    default:
        PlaySE(SE_SELECT);
        gUnknown_203AD10->field_04 = 0xFF;
        sub_81095AC(taskId, input);
        break;
    }
}

void sub_81095AC(u8 taskId, u32 itemIndex)
{
    s16 *data = gTasks[taskId].data;
    if (data[1] == itemIndex || data[1] == itemIndex - 1)
    {
        sub_810967C(taskId, itemIndex);
    }
    else
    {
        MoveItemSlotInList(gBagPockets[gUnknown_203ACFC.pocket].itemSlots, data[1], itemIndex);
        DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        if (data[1] < itemIndex)
            gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]--;
        sub_810842C(gUnknown_203ACFC.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        sub_8098660(1);
        sub_81088D8();
        gTasks[taskId].func = sub_8108F0C;
    }
}

void sub_810967C(u8 taskId, u32 itemIndex)
{
    s16 *data = gTasks[taskId].data;
    DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    if (data[1] < itemIndex)
        gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]--;
    sub_810842C(gUnknown_203ACFC.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    sub_8098660(1);
    sub_81088D8();
    gTasks[taskId].func = sub_8108F0C;
}

void sub_810971C(u16 cursorPos, const u8 *str)
{
    u8 r4;
    u8 r5 = sub_810B9DC(6, 2);
    CopyItemName(BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, cursorPos), gStringVar1);
    StringExpandPlaceholders(gStringVar4, str);
    sub_810B8F0(r5, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    r4 = sub_810B9DC(0, 0);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    sub_810B8F0(r4, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
    sub_8108940();
}

void sub_81097E4(s16 value, u8 ndigits)
{
    u8 r6 = sub_810BAD8(0);
    FillWindowPixelBuffer(r6, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_LEADING_ZEROS, ndigits);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    sub_810B8F0(r6, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
}

void sub_8109854(u8 a0)
{
    CopyToBgTilemapBufferRect(1, gUnknown_8452D08[12 - a0], 11, 13 - a0, 18, 1);
    ScheduleBgCopyTilemapToVram(1);
}

void sub_8109890(u8 taskId)
{
    u8 r6;
    u8 r4;
    switch (gUnknown_203ACFC.location)
    {
    case 5:
    case 7:
        if (gSpecialVar_ItemId == ITEM_BERRY_POUCH)
        {
            gUnknown_203AD20[0] = ITEMMENUACTION_OPEN_BERRIES;
            gUnknown_203AD20[1] = ITEMMENUACTION_CANCEL;
            gUnknown_203AD24 = gUnknown_203AD20;
            gUnknown_203AD28 = 2;
        }
        else if (ItemId_GetBattleUsage(gSpecialVar_ItemId))
        {
            gUnknown_203AD24 = gUnknown_8452F30;
            gUnknown_203AD28 = 2;
        }
        else
        {
            gUnknown_203AD24 = gUnknown_8452F32;
            gUnknown_203AD28 = 1;
        }
        break;
    case 6:
    case 8:
        gUnknown_203AD24 = gUnknown_8452F30;
        gUnknown_203AD28 = 2;
        break;
    default:
        if (MenuHelpers_LinkSomething() == TRUE || InUnionRoom() == TRUE)
        {
            if (gSpecialVar_ItemId == ITEM_TM_CASE || gSpecialVar_ItemId == ITEM_BERRY_POUCH)
            {
                gUnknown_203AD24 = gUnknown_8452F2E;
                gUnknown_203AD28 = 2;
            }
            else
            {
                if (gUnknown_203ACFC.pocket == POCKET_KEY_ITEMS - 1)
                    gUnknown_203AD28 = 1;
                else
                    gUnknown_203AD28 = 2;
                gUnknown_203AD24 = gUnknown_8452F28[gUnknown_203ACFC.pocket];
            }
        }
        else
        {
            switch (gUnknown_203ACFC.pocket)
            {
            case POCKET_ITEMS - 1:
                gUnknown_203AD28 = 4;
                if (ItemIsMail(gSpecialVar_ItemId) == TRUE)
                    gUnknown_203AD24 = gUnknown_8452F24;
                else
                    gUnknown_203AD24 = gUnknown_8452F18[gUnknown_203ACFC.pocket];
                break;
            case POCKET_KEY_ITEMS - 1:
                gUnknown_203AD24 = gUnknown_203AD20;
                gUnknown_203AD28 = 3;
                gUnknown_203AD20[2] = ITEMMENUACTION_CANCEL;
                if (gSaveBlock1Ptr->registeredItem == gSpecialVar_ItemId)
                    gUnknown_203AD20[1] = ITEMMENUACTION_DESELECT;
                else
                    gUnknown_203AD20[1] = ITEMMENUACTION_REGISTER;
                if (gSpecialVar_ItemId == ITEM_TM_CASE || gSpecialVar_ItemId == ITEM_BERRY_POUCH)
                    gUnknown_203AD20[0] = ITEMMENUACTION_OPEN;
                else if (gSpecialVar_ItemId == ITEM_BICYCLE && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_MACH_BIKE))
                    gUnknown_203AD20[0] = ITEMMENUACTION_WALK;
                else
                    gUnknown_203AD20[0] = ITEMMENUACTION_USE;
                break;
            case POCKET_POKE_BALLS - 1:
                gUnknown_203AD24 = gUnknown_8452F18[gUnknown_203ACFC.pocket];
                gUnknown_203AD28 = 3;
                break;
            }
        }
    }
    r6 = sub_810B9DC(10, gUnknown_203AD28 - 1);
    AddItemMenuActionTextPrinters(
        r6,
        2,
        GetMenuCursorDimensionByFont(2, 0),
        2,
        GetFontAttribute(2, FONTATTR_LETTER_SPACING),
        GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2,
        gUnknown_203AD28,
        gUnknown_8452EB8,
        gUnknown_203AD24
    );
    Menu_InitCursor(r6, 2, 0, 2, GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, gUnknown_203AD28, 0);
    r4 = sub_810B9DC(6, 0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gOtherText_StrVar1);
    sub_810B8F0(r4, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
}

void sub_8109BB8(u8 taskId)
{
    sub_8109890(taskId);
    gTasks[taskId].func = sub_8109BE4;
}

void sub_8109BE4(u8 taskId)
{
    s8 input;
    if ((u8)sub_80BF72C() != TRUE)
    {
        input = Menu_ProcessInputNoWrapAround();
        switch (input)
        {
        case -1:
            PlaySE(SE_SELECT);
            gUnknown_8452EB8[ITEMMENUACTION_CANCEL].func.void_u8(taskId);
            break;
        case -2:
            break;
        default:
            PlaySE(SE_SELECT);
            gUnknown_8452EB8[gUnknown_203AD24[input]].func.void_u8(taskId);
            break;
        }
    }
}

void Task_ItemMenuAction_Use(u8 taskId)
{
    if (ItemId_GetFieldFunc(gSpecialVar_ItemId) != NULL)
    {
        sub_810BA3C(10);
        sub_810BA3C(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        if (CalculatePlayerPartyCount() == 0 && ItemId_GetType(gSpecialVar_ItemId) == 1)
            sub_810A170(taskId);
        else
            ItemId_GetFieldFunc(gSpecialVar_ItemId)(taskId);
    }
}

void Task_ItemMenuAction_Toss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ClearWindowTilemap(sub_810BAD8(10));
    ClearWindowTilemap(sub_810BAD8(6));
    sub_810BA3C(10);
    sub_810BA3C(6);
    PutWindowTilemap(0);
    data[8] = 1;
    if (data[2] == 1)
    {
        sub_8109D38(taskId);
    }
    else
    {
        sub_810971C(data[1], gText_TossOutHowManyStrVar1s);
        gTasks[taskId].func = sub_8109DEC;
    }
}

void sub_8109D38(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrowAwayStrVar2OfThisItemQM);
    sub_810B8F0(sub_810B9DC(6, 1), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    sub_810BAE8(taskId, &gUnknown_8452F50);
}

void sub_8109DB0(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    sub_810BA3C(6);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    sub_810910C(taskId);
}

void sub_8109DEC(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
    {
        sub_81097E4(data[8], 3);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(sub_810BAD8(6));
        sub_810BA3C(6);
        sub_810BA3C(0);
        ScheduleBgCopyTilemapToVram(0);
        sub_8108978();
        sub_8109D38(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        sub_810BA3C(6);
        sub_810BA3C(0);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        bag_menu_print_cursor_(data[0], 1);
        sub_8108978();
        sub_810910C(taskId);
    }
}

void sub_8109EA8(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    sub_810BA3C(6);
    CopyItemName(BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1]), gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrewAwayStrVar2StrVar1s);
    sub_810B8F0(sub_810B9DC(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    gTasks[taskId].func = sub_8109F44;
}

void sub_8109F44(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveBagItem(gSpecialVar_ItemId, data[8]);
        sub_810BA3C(6);
        DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        sub_8108DC8(gUnknown_203ACFC.pocket);
        sub_81089F4(gUnknown_203ACFC.pocket);
        sub_810842C(gUnknown_203ACFC.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        bag_menu_print_cursor_(data[0], 1);
        sub_810910C(taskId);
    }
}

void Task_ItemMenuAction_ToggleSelect(u8 taskId)
{
    u16 itemId;
    s16 *data = gTasks[taskId].data;
    itemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1]);
    if (gSaveBlock1Ptr->registeredItem == itemId)
        gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    else
        gSaveBlock1Ptr->registeredItem = itemId;

    DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    sub_810842C(gUnknown_203ACFC.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    CopyWindowToVram(0, 1);
    Task_ItemMenuAction_Cancel(taskId);
}

void Task_ItemMenuAction_Give(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1]);
    sub_810BA3C(10);
    sub_810BA3C(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    CopyWindowToVram(0, 1);
    if (!CanWriteMailHere(itemId))
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, sub_810A1D0);
    else if (!itemid_is_unique(itemId))
    {
        if (CalculatePlayerPartyCount() == 0)
        {
            sub_810A170(taskId);
        }
        else
        {
            gUnknown_203AD10->exitCB = CB2_ChooseMonToGiveItem;
            gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        }
    }
    else
        sub_810A18C(taskId);
}

void sub_810A170(u8 taskId)
{
    DisplayItemMessageInBag(taskId, 2, gText_ThereIsNoPokemon, sub_810A1D0);
}

void sub_810A18C(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_ItemCantBeHeld);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, sub_810A1D0);
}

void sub_810A1D0(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        sub_810A1F8(taskId);
    }
}

void sub_810A1F8(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    sub_810BA9C(5);
    DestroyListMenuTask(data[0], &gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], &gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    sub_8108DC8(gUnknown_203ACFC.pocket);
    sub_81089F4(gUnknown_203ACFC.pocket);
    sub_810842C(gUnknown_203ACFC.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gUnknown_203ACFC.cursorPos[gUnknown_203ACFC.pocket], gUnknown_203ACFC.itemsAbove[gUnknown_203ACFC.pocket]);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    sub_810910C(taskId);
}

void sub_810A288(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemsAbove;
    u16 cursorPos;
    ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
    sub_8108818(cursorPos + itemsAbove);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    sub_810910C(taskId);
}

void Task_ItemMenuAction_Cancel(u8 taskId)
{
    sub_810BA3C(10);
    sub_810BA3C(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(gTasks[taskId].data[0], 1);
    sub_810910C(taskId);
}

void sub_810A324(u8 taskId)
{
    if (ItemId_GetBattleFunc(gSpecialVar_ItemId) != NULL)
    {
        sub_810BA3C(10);
        sub_810BA3C(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        CopyWindowToVram(0, 1);
        ItemId_GetBattleFunc(gSpecialVar_ItemId)(taskId);
    }
}

void sub_810A370(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1]);
    if (!CanWriteMailHere(itemId))
    {
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, sub_810A1D0);
    }
    else if (itemId == ITEM_TM_CASE)
    {
        ItemMenu_SetExitCallback(sub_810A434);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (itemId == ITEM_BERRY_POUCH)
    {
        ItemMenu_SetExitCallback(sub_810A448);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (gUnknown_203ACFC.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(itemId))
    {
        sub_8108CB4();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
    {
        sub_810A18C(taskId);
    }
}

void sub_810A434(void)
{
    InitTMCase(1, sub_810A45C, FALSE);
}

void sub_810A448(void)
{
    InitBerryPouch(1, sub_810A45C, FALSE);
}

void sub_810A45C(void)
{
    CB2_SelectBagItemToGive();
}

void sub_810A468(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gUnknown_203ACFC.pocket + 1, data[1]);
    if (ItemIsMail(itemId) == TRUE)
    {
        DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, sub_810A1D0);
    }
    else if (itemId == ITEM_TM_CASE)
    {
        ItemMenu_SetExitCallback(sub_810A52C);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (itemId == ITEM_BERRY_POUCH)
    {
        ItemMenu_SetExitCallback(sub_810A540);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (gUnknown_203ACFC.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(itemId))
    {
        sub_8108CB4();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
    {
        sub_810A18C(taskId);
    }
}

void sub_810A52C(void)
{
    InitTMCase(3, sub_810A554, FALSE);
}

void sub_810A540(void)
{
    InitBerryPouch(3, sub_810A554, FALSE);
}

void sub_810A554(void)
{
    GoToBagMenu(4, 3, sub_808CE60);
}
