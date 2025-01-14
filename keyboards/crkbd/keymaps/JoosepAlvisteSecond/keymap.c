#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TAB_L G(S(KC_LBRC))
#define TAB_R G(S(KC_RBRC))
#define SPACE_L A(G(KC_LEFT))
#define SPACE_R A(G(KC_RGHT))
#define LA_NUM MO(NUM)
#define LA_NAV MO(NAV)

enum layers {
    DEF,
    NAV,
    NUM,
    SYM,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN,  // Switch to next window         (cmd-tab)
    SW_LANG, // Switch to next input language (ctl-spc)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_split_3x6_3(
        XXXXXXX, KC_Q, KC_W, KC_F, KC_P, KC_G,    KC_J, KC_L, KC_U,    KC_Y,   KC_QUOT, XXXXXXX,
        XXXXXXX, KC_A, KC_R, KC_S, KC_T, KC_D,    KC_H, KC_N, KC_E,    KC_I,   KC_O,    XXXXXXX,
        XXXXXXX, KC_Z, KC_X, KC_C, KC_V, KC_B,    KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, XXXXXXX,
                     KC_ESC, LA_NAV,  KC_LSFT,    LT(SYM, KC_SPC),  LA_NUM, KC_BSPC
    ),

    [NUM] = LAYOUT_split_3x6_3(
        _______, KC_LBRC,  KC_7, KC_8, KC_9, KC_RBRC,   _______, _______,  _______, _______, _______, _______,
        _______, KC_SCLN,  KC_4, KC_5, KC_6, KC_EQL,    _______, OS_SHFT,  OS_CMD,  OS_ALT,  OS_CTRL, _______,
        _______, KC_GRAVE, KC_1, KC_2, KC_3, KC_BSLS,   _______, _______,  _______, _______, _______, _______,
                                 KC_9, KC_0, KC_MINS,   _______, _______, _______
    ),

    [NAV] = LAYOUT_split_3x6_3(
        _______, KC_TAB,  SW_WIN,  TAB_L,   TAB_R,   KC_VOLU,   RESET,   _______, _______, _______,  KC_DEL,  _______,
        _______, OS_CTRL, OS_ALT,  OS_CMD,  OS_SHFT, KC_VOLD,   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_CAPS, _______,
        _______, _______, SPACE_R, KC_MPRV, KC_MNXT, KC_MPLY,   HOME,    KC_PGDN, KC_PGUP, END,      SW_LANG, _______,
                                   _______, _______, _______,   _______, _______, _______
    ),

    [SYM] = LAYOUT_split_3x6_3(
        _______, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,   _______, _______,  _______, _______, _______, _______,
        _______, KC_COLN, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS,   _______, OS_SHFT,  OS_CMD,  OS_ALT,  OS_CTRL, _______,
        _______, KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE,   _______, _______,  _______, _______, _______, _______,
                                   KC_LPRN, KC_RPRN, KC_UNDS,   _______, _______, _______
    ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_NUM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_NUM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
        keycode, record
    );
    update_swapper_three(
        &sw_lang_active, KC_LCMD, KC_LALT, KC_SPC, SW_LANG,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    return true;
}

enum combos {
  LU_ENT,
  FP_TAB,
  MCOMMA_CLN,
  XC_MINS,
  COMMDOT_UNDS,
};

const uint16_t PROGMEM lu_combo[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM fp_combo[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM mcomma_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM xc_combo[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM commdot_combo[] = {KC_COMM, KC_DOT, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [LU_ENT] = COMBO(lu_combo, KC_ENT),
  [FP_TAB] = COMBO(fp_combo, KC_TAB),
  [MCOMMA_CLN] = COMBO(mcomma_combo, KC_COLN),
  [XC_MINS] = COMBO(xc_combo, KC_MINS),
  [COMMDOT_UNDS] = COMBO(commdot_combo, KC_UNDS),
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer:"), false);
    // switch (layer_state) {
    //     case L_BASE:
    //         oled_write_ln_P(PSTR("Default"), false);
    //         break;
    //     case L_MEDIA:
    //         oled_write_ln_P(PSTR("Media"), false);
    //         break;
    //     case L_NAV:
    //         oled_write_ln_P(PSTR("Nav"), false);
    //         break;
    //     case 8:
    //         oled_write_ln_P(PSTR("Mouse"), false);
    //         break;
    //     case 16:
    //         oled_write_ln_P(PSTR("Symbols"), false);
    //         break;
    //     case 32:
    //         oled_write_ln_P(PSTR("Numbers"), false);
    //         break;
    //     case 64:
    //         oled_write_ln_P(PSTR("Function"), false);
    //         break;
    // }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x20, 0x60, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0,
        0x80, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x20, 0x60, 0xc0, 0xc0,
        0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xf8, 0xf0, 0xc0, 0xc0, 0xc0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0,
        0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x88, 0x24, 0xa4, 0xe6, 0xe6, 0xce, 0xdc, 0xfd, 0xfd,
        0xfd, 0xff, 0xfe, 0xda, 0x25, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x03, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0f, 0x07, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x81, 0x49, 0x65, 0x64, 0xc4, 0xce, 0xee, 0xef, 0xe7, 0xf7, 0xff,
        0x7f, 0x3f, 0xbf, 0x9f, 0xc0, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01,
        0x00, 0x03, 0x03, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(crkbd_logo, sizeof(crkbd_logo));
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   if (record->event.pressed) {
//     set_keylog(keycode, record);
//   }
//   return true;
// }
#endif // OLED_DRIVER_ENABLE
