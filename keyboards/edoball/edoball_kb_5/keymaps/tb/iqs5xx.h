#ifndef AZOTEQ_IQS5XX_ADDRESS
#    define AZOTEQ_IQS5XX_ADDRESS (0x74 << 1)
#endif
#ifndef AZOTEQ_IQS5XX_TIMEOUT_MS
#    define AZOTEQ_IQS5XX_TIMEOUT_MS 10
#endif
#define AZOTEQ_IQS5XX_REG_GLOBAL_ATI_C 0x056B
#define AZOTEQ_IQS5XX_REG_ATI_TARGET 0x056D
#define AZOTEQ_IQS5XX_REG_PROX_THRESHOLD 0x0594
#define AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_SET 0x0596
#define AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_CLEAR 0x0597
#define AZOTEQ_IQS5XX_REG_NUMBER_OF_FINGERS 0x0011

uint16_t get_ati_target2(void);
uint16_t set_ati_target2(uint16_t value);
uint16_t get_ati_target(void);
uint16_t set_ati_target(uint16_t ati_target);
uint8_t get_global_ati_c(void) ;
uint8_t set_global_ati_c(uint8_t global_ati_c);
uint8_t get_prox_th(void);
uint8_t set_prox_th(uint8_t prox_th);
uint8_t get_global_touch_multiplier_set(void);
uint8_t set_global_touch_multiplier_set(uint8_t mul);
uint8_t get_global_touch_multiplier_clear(void);
uint8_t set_global_touch_multiplier_clear(uint8_t mul);
void print_ati_target(void);
void print_iqs5xx_config(void);
uint16_t dump_reg(uint16_t start_addr, uint16_t end_addr);
uint8_t get_reg8(uint16_t reg_addr);
