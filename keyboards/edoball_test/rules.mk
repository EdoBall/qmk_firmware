# MCU name
#MCU = atmega32u4
MCU = RP2040

# Bootloader selection
#BOOTLOADER = atmel-dfu
BOOTLOADER = rp2040

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes        # Console for debug
COMMAND_ENABLE = yes        # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

# added for edoball_kb_2
SPLIT_KEYBOARD = no

# for right hand: no
# for left hand:  yes
OLED_ENABLE = no
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = adns5050

WS2812_DRIVER = vendor
SERIAL_DRIVER = vendor

# BOARD = GENERIC_RP_RP2040
