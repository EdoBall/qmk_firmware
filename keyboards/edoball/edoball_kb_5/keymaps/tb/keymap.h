#define TIME_OUT 1000
#define TH_ON 10
#define TH_OFF 8

// track pad off
#undef AZOTEQ_IQS5XX_TPS43

// layers:
#define _QWERTY 0
#define _LOWER  1
#define _RAISE  2
#define _LOWERJ 3
#define _CURSOR 4
#define _TENKEY 5
#define _MOUSE  6
#define _TCURSOR 7
#define _FN     14
#define _ADJUST 15

// Keyboard Control Mode:
// 0: normal(usus), 1: us key on jp os, 2: emacs, 3: vi, 4: wasd, 5: Touch wall invert mode, 
#define M_USUS 0
#define M_USJP 1
#define M_EMACS 2
#define M_VI 3
#define M_WASD 4
#define M_TENKEY 5
#define M_PCTL_INV 6
#define M_CURSOR_ON 7

enum custom_keycodes {
    CC_TW1 = SAFE_RANGE,  // touch wall 1
    CC_PCB1, // touch key PCB 1 (on the main PCB)
    CC_PR1,  // touch key 2 (palm rest)
    CC_PCTL, // Pseudo-Control
    CC_FN,
    CC_LOWER,
    CC_RAISE,
    CC_ADJUST,
    CC_INFO,
    CC_TBUP,
    CC_TBDWN,
    CC_USUS,
    CC_USJP,
    CC_EMACS,
    CC_CTL_INV, // ctrl key invert
    CC_TENKEY,
    CC_SPLW,
    CC_ATTUP,
    CC_ATTDN,
    CC_GACUP,
    CC_GACDN,
    CC_PRXUP,
    CC_PRXDN,
    CC_TMSUP,
    CC_TMSDN,
    CC_TMCUP,
    CC_TMCDN,
    CC_DUMP,
    CC_SCRL,
    CC_T1,
    CC_T2,
    CC_T3,
    CC_T4,
    CC_DUMMY
};
