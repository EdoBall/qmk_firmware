# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

# added for edoball_kb_2
SPLIT_KEYBOARD = yes

# for right hand(slave): no
# for left hand(master):  yes
#OLED_ENABLE = yes
OLED_ENABLE = yes
#POINTING_DEVICE_ENABLE = yes
# currently no track ball.
POINTING_DEVICE_ENABLE = no
#POINTING_DEVICE_DRIVER = custom
