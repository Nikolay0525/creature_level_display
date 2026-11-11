// creature_level_display: Kyubu port of ToufouMaster's CreatureLevelDisplay. Shows "LV <level> <name>" for the creature
// you look at.
//
// The hook site and the way to replace the string come from level_rework (its install_name_level), which already
// does this on this build with a verified symbol:
//   cube::GameController::lookAtNameSite (0x49B016), in GameController::update, where the call that writes the kind
//   ("Dog") of the name tag is about to run. On the stack: [ESP] the stream, [ESP+4] the kind's std::wstring, and the
//   creature is `*[ESP+0x38]`. The hook passes a string of its own in place of the game's. The game frees its
//   temporaries by their stack address and never through this argument, and update runs on the main thread, so one
//   static string is enough (no allocation, no cache).
// The original mod hooked 0x49AE36 with a stub and made the same replacement; its offsets ([esp+0x24], [esp+0x58]
// after pushing 8 registers) are the same +4 / +0x38.
//
// level_rework has this feature too ("Dog (12)", option show_creature_level). Do not run both: they would stack.
#include "../../common.hpp"
#include "cubeworld/cubeworld.h"

#include <cstdio>

namespace {

kyubu::Mod* g_mod = nullptr;
std_wstring g_name_string{};
wchar_t g_name_text[96];
int g_logged = 0;

void KYUBU_CALL on_name_tag(KyubuCpuContext* ctx, void*) {
    const std_wstring** argument = reinterpret_cast<const std_wstring**>(ctx->esp + 4);
    const std_wstring* kind = *argument;
    const cube_Creature* const* holder = *reinterpret_cast<const cube_Creature* const**>(ctx->esp + 0x38);
    if (!kind || !holder || !*holder) return;

    constexpr uint32_t kRoom = sizeof g_name_text / sizeof g_name_text[0];
    char prefix[16];
    const int level = static_cast<int>((*holder)->data.level);
    const int added = std::snprintf(prefix, sizeof prefix, "LV %d ", level);
    const uint32_t length = kind->size;
    if (added <= 0 || length == 0 || length + static_cast<uint32_t>(added) >= kRoom) return;

    for (int i = 0; i < added; ++i) g_name_text[i] = static_cast<wchar_t>(prefix[i]);
    // VC110's std::wstring keeps up to 7 characters in the object itself (capacity 7) and the longer ones behind a pointer.
    std::memcpy(g_name_text + added, kind->capacity < 8 ? kind->bx.buf : kind->bx.ptr, length * sizeof(wchar_t));
    g_name_text[added + length] = L'\0';

    g_name_string.bx.ptr = g_name_text;
    g_name_string.size = length + static_cast<uint32_t>(added);
    g_name_string.capacity = kRoom - 1;
    *argument = &g_name_string;

    if (g_logged < 5) {
        ++g_logged;
        char shown[kRoom];
        for (uint32_t i = 0; i <= g_name_string.size; ++i) shown[i] = g_name_text[i] < 0x80 ? static_cast<char>(g_name_text[i]) : '?';
        g_mod->info("creature_level_display: name tag \"%s\"", shown);
    }
}

}  // namespace

struct CreatureLevelDisplay {
    explicit CreatureLevelDisplay(kyubu::Mod& m) {
        g_mod = &m;
        const uintptr_t site = m.symbol(CW_SYM_cube_GameController_lookAtNameSite);
        if (!site) { m.error("creature_level_display: the name tag site is unavailable on this build: names stay as they are"); return; }
        KyubuId id = 0;
        const KyubuResult r = m.api()->hook_mid(m.handle(), site, &on_name_tag, nullptr, &id);
        if (r != KY_OK) { m.error("creature_level_display: could not hook the name tag (result %d)", static_cast<int>(r)); return; }
        m.info("creature_level_display: creature names show their level (hook at 0x%08X)", static_cast<unsigned>(site));
    }
};

KYUBU_MOD("creature_level_display", KY_SIDE_CLIENT, CreatureLevelDisplay)
