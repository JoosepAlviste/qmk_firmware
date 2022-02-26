#define MASTER_LEFT

// OLED
#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

// Combos
#define COMBO_COUNT 5
#define COMBO_TERM 40

// Mod taps
#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 300
// Prevent normal rollover on alphas from accidentally triggering mods.
#define IGNORE_MOD_TAP_INTERRUPT
// Apply the modifier on keys that are tapped during a short hold of a modtap
// Try disabling if activating mods too often
#define PERMISSIVE_HOLD
// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD
