#define MAX_KEY_OVERRIDES 35

void init_overrides(void);
int8_t add_overrides(const key_override_t **add);
int8_t set_overrides(const key_override_t **ko1, const key_override_t **ko2, const key_override_t **ko3);
int8_t cursor_overrides_on(void);
int8_t cursor_overrides_off(void);

extern const key_override_t *key_overrides[MAX_KEY_OVERRIDES];

// Perform as a US keyboard on JP OS systems
extern const key_override_t *us_key_on_jp_os_overrides[];

extern const key_override_t *emacs_overrides[];
extern const key_override_t *vi_overrides[];
extern const key_override_t *game_overrides[];
extern const key_override_t *tenkey_cursor_overrides[];
extern const key_override_t *dummy_overrides[];
