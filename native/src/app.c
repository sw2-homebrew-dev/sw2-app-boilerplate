/*
 * Hello World - sw2-app-boilerplate native starter.
 *
 * Shows the parts most apps need: the frame loop, controller and touch input,
 * the 2D UI, a packaged asset (assets/greeting.txt) and the persistent save
 * block (manifest "save_bytes"; stored on the PC as saves/<id>.sav).
 *
 * Entry point: hbx_app_main.  Return to hand control back to the launcher.
 */
#include "hbx_sdk.h"
#include "hbx_app.h"
#include "hbx_ui.h"

#define SAVE_VERSION 1u

struct save {
    struct hbx_save_header header;
    uint32_t launches;
    uint32_t best_presses;
};

static const uint32_t INK = HBX_RGB(27, 36, 48);
static const uint32_t MUTED = HBX_RGB(91, 102, 117);
static const uint32_t ACCENT = HBX_RGB(216, 67, 47);

/* No libc: format an unsigned number into a small buffer. */
static const char *u32_text(uint32_t value, char out[12]) {
    char *p = out + 11;
    *p = 0;
    do { *--p = (char)('0' + value % 10u); value /= 10u; } while (value);
    return p;
}

static struct save *open_save(const struct hbx_ctx *ctx) {
    uint32_t size = 0;
    struct save *save = (struct save *)hbx_save_block(ctx, &size);
    if (!save || size < sizeof *save) return 0;
    if (save->header.magic != HBX_SAVE_MAGIC || save->header.version != SAVE_VERSION) {
        hbx_memset(save, 0, sizeof *save);
        save->header.magic = HBX_SAVE_MAGIC;
        save->header.version = SAVE_VERSION;
        save->header.bytes = sizeof *save;
    }
    return save;
}

int hbx_app_main(const struct hbx_ctx *ctx) {
    struct hbx_input_state input;
    struct save *save;
    uint64_t deadline = 0;
    uint32_t presses = 0, greeting_size = 0, i;
    float x = 960.0f, y = 640.0f;
    char number[12], greeting[64];
    const char *asset;

    if (hbx_ctx_check(ctx) != HBX_OK) return HBX_ERR_ABI;
    hbx_memset(&input, 0, sizeof input);

    /* Assets are not NUL-terminated: copy into a bounded buffer. */
    hbx_memset(greeting, 0, sizeof greeting);
    asset = (const char *)hbx_asset(ctx, "greeting.txt", &greeting_size);
    for (i = 0; asset && i < greeting_size && i + 1 < sizeof greeting; i++) greeting[i] = asset[i];

    save = open_save(ctx);
    if (save) {
        save->launches++;
        save->header.dirty = 1;
    }

    while (!hbx_stop_requested(ctx)) {
        hbx_input_poll(ctx, &input);
        if (input.pressed & (HBX_B | HBX_PLUS)) break;
        if (input.pressed & HBX_A) presses++;
        x += (float)input.stick_lx * 0.12f;
        y -= (float)input.stick_ly * 0.12f;
        if (input.touch_count) {
            x = (float)input.touch[0].x * 1.5f;
            y = (float)input.touch[0].y * 1.5f;
        }
        x = x < 40.0f ? 40.0f : x > 1880.0f ? 1880.0f : x;
        y = y < 460.0f ? 460.0f : y > 1040.0f ? 1040.0f : y;

        hbx_ui_begin(ctx, HBX_RGB(246, 247, 249));
        hbx_ui_text(ctx, 120.0f, 110.0f, HBX_FONT_TITLE, "Hello World", INK);
        hbx_ui_text(ctx, 124.0f, 230.0f, HBX_FONT_H2, greeting[0] ? greeting : "(no greeting asset)", MUTED);

        hbx_ui_text(ctx, 124.0f, 300.0f, HBX_FONT_BODY, "A presses this session:", MUTED);
        hbx_ui_text(ctx, 420.0f, 300.0f, HBX_FONT_BODY, u32_text(presses, number), INK);
        if (save) {
            hbx_ui_text(ctx, 124.0f, 336.0f, HBX_FONT_BODY, "Launches (save block):", MUTED);
            hbx_ui_text(ctx, 420.0f, 336.0f, HBX_FONT_BODY, u32_text(save->launches, number), INK);
            hbx_ui_text(ctx, 124.0f, 372.0f, HBX_FONT_BODY, "Best A presses:", MUTED);
            hbx_ui_text(ctx, 420.0f, 372.0f, HBX_FONT_BODY, u32_text(save->best_presses, number), INK);
        }
        hbx_ui_circle(ctx, x, y, 36.0f, 36.0f, ACCENT, 0); /* thickness = radius: filled */
        hbx_ui_text(ctx, 124.0f, 1000.0f, HBX_FONT_SMALL,
                    "Left stick or touch moves the dot. A counts. B returns to the launcher.", MUTED);
        hbx_ui_present(ctx);
        hbx_frame_end(ctx);
        hbx_pace(ctx, &deadline, 60);
    }

    if (save && presses > save->best_presses) {
        save->best_presses = presses;
        save->header.dirty = 1;
    }
    return HBX_OK;
}
