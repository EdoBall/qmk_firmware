#include QMK_KEYBOARD_H
#include "iqs5xx.h"
#include "print.h"
#include "debug.h"

/* for IQS5XX
Switch Science IQS525:
Global ATI C 3;
Global ATI Target 500;

TPS43:
Global ATI C 1;
Global ATI Target 700;
Prox Th 23;Global Touch Multiplier - Set 16;Global Touch Multiplier - clear 12;t:0;df:7,6,250;
*/

static char debug_str[256];


uint8_t get_reg8(uint16_t reg_addr) {
    uint8_t reg_data[1];
    i2c_status_t status;

    status = i2c_read_register16(AZOTEQ_IQS5XX_ADDRESS, reg_addr, reg_data, 1, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        return(reg_data[0]);
    } else {
        dprint("I2C error");
    }
    return(0);
}

uint8_t set_reg8(uint16_t reg_addr, uint8_t reg_data){
    uint8_t current_reg_data;
    i2c_status_t status;

    current_reg_data = get_reg8(reg_addr);
    status = i2c_write_register16(AZOTEQ_IQS5XX_ADDRESS, reg_addr, &reg_data, 1, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        sprintf(debug_str, "%x:%d > %d;", reg_addr, current_reg_data, reg_data);
        dprint(debug_str);
        return(reg_data);
    } else {
        dprint("I2C error");
    }
    return(0);
}

// 2Bytes operation functions
uint16_t swap16(uint16_t org) {
    return(org << 8 | org >> 8);
}

uint16_t get_reg16(uint16_t reg_addr) {
    // return azoteq iqs5xx's register value at reg_addr (16bit)
    // Error occured: return 0
    uint8_t read_data[2];
    uint16_t ret;
    i2c_status_t status;
    
    status = i2c_read_register16(AZOTEQ_IQS5XX_ADDRESS, reg_addr, read_data, 2, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        ret = read_data[0] << 8 | read_data[1];
        return(ret);
    } else {
        dprint("I2C error");
    }
    return(0);
}

uint16_t set_reg16(uint16_t reg_addr, uint16_t value){
    uint8_t reg_data[2];
    uint16_t current_value;
    i2c_status_t status;

    current_value = get_reg16(reg_addr);
    reg_data[0] = value >> 8;
    reg_data[1] = (uint8_t) value;
    status = i2c_write_register16(AZOTEQ_IQS5XX_ADDRESS, reg_addr, reg_data, 2, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        sprintf(debug_str, "%x:%d > %d;", reg_addr, current_value, value);
        dprint(debug_str);
        return(value);
    } else {
        dprint("I2C error");
    }
    return(0);
}

uint16_t get_ati_target2(void) {
    return(get_reg16(AZOTEQ_IQS5XX_REG_ATI_TARGET));
}


uint16_t set_ati_target2(uint16_t value) {
    return(set_reg16(AZOTEQ_IQS5XX_REG_ATI_TARGET, value));
}

uint16_t get_ati_target(void) {
    // return azoteq iqs5xx ati target value. (usualy 100 - 1000)
    // Error occured: return 0
    uint8_t read_data[2];
    uint16_t ati_target;
    i2c_status_t status;
    
    status = i2c_read_register16(AZOTEQ_IQS5XX_ADDRESS, AZOTEQ_IQS5XX_REG_ATI_TARGET, read_data, 2, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        ati_target = read_data[0] << 8 | read_data[1];
        return(ati_target);
    } else {
        dprint("I2C error");
    }
    return(0);
}

uint16_t set_ati_target(uint16_t ati_target){
    uint8_t reg_data[2];
    uint16_t current_ati_target;
    i2c_status_t status;

    current_ati_target = get_ati_target();
    reg_data[0] = ati_target >> 8;
    reg_data[1] = (uint8_t) ati_target;
    status = i2c_write_register16(AZOTEQ_IQS5XX_ADDRESS, AZOTEQ_IQS5XX_REG_ATI_TARGET, reg_data, 2, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        sprintf(debug_str, "ATI Target %d > %d;", current_ati_target, ati_target);
        dprint(debug_str);
        return(ati_target);
    } else {
        dprint("I2C error");
    }
    return(0);
}


uint8_t get_global_ati_c(void) {
    // return azoteq iqs5xx global ati c
    // Error occured: return 0

    return(get_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_ATI_C));
}

uint8_t set_global_ati_c(uint8_t global_ati_c){
    return(set_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_ATI_C, global_ati_c));
}

uint8_t get_prox_th(void) {
    // return azoteq iqs5xx prox threshold.
    // Error occured: return 0
    return(get_reg8(AZOTEQ_IQS5XX_REG_PROX_THRESHOLD));
}

uint8_t set_prox_th(uint8_t prox_th){
    return(set_reg8(AZOTEQ_IQS5XX_REG_PROX_THRESHOLD, prox_th));
}

uint8_t get_global_touch_multiplier_set(void){
    return(get_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_SET));
}

uint8_t set_global_touch_multiplier_set(uint8_t mul){
    return(set_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_SET, mul));
}

uint8_t get_global_touch_multiplier_clear(void){
    return(get_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_CLEAR));
}

uint8_t set_global_touch_multiplier_clear(uint8_t mul){
    return(set_reg8(AZOTEQ_IQS5XX_REG_GLOBAL_TOUCH_MULTIPLIER_CLEAR, mul));
}

void print_ati_target(void) {
    uint8_t reg_data[2];
    uint8_t global_ati_c;
    uint16_t ati_target;
    i2c_status_t status;

    status = i2c_read_register16(AZOTEQ_IQS5XX_ADDRESS, AZOTEQ_IQS5XX_REG_GLOBAL_ATI_C, reg_data, 1, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        global_ati_c = reg_data[0];
        sprintf(debug_str, "Global ATI C %d;", global_ati_c);
        SEND_STRING(debug_str);
        dprint(debug_str);
    } else {
        dprint("I2C error");
    }
    
    status = i2c_read_register16(AZOTEQ_IQS5XX_ADDRESS, AZOTEQ_IQS5XX_REG_ATI_TARGET, reg_data, 2, AZOTEQ_IQS5XX_TIMEOUT_MS);
    if (status == I2C_STATUS_SUCCESS) {
        ati_target = reg_data[0] << 8 | reg_data[1];
        sprintf(debug_str, "ATI Target %d;", ati_target);
        SEND_STRING(debug_str);
        dprint(debug_str);
    } else {
        dprint("I2C error");
    }
    return;
}    

void print_iqs5xx_config(void) {
    uint16_t ati_target;
    uint8_t global_ati_c;
    uint8_t prox_th;
    uint8_t global_touch_multiplier_set;
    uint8_t global_touch_multiplier_clear;
    
    global_ati_c = get_global_ati_c();
    sprintf(debug_str, "Global ATI C %d;", global_ati_c);
    SEND_STRING(debug_str);
    wait_ms(10);
    dprint(debug_str);
    if (global_ati_c == 0) {
        dprint("I2C error (maybe...)");
    }

    ati_target = get_ati_target2();
    sprintf(debug_str, "Global ATI Target %d;", ati_target);
    SEND_STRING(debug_str);
    wait_ms(10);
    dprint(debug_str);
    if (ati_target == 0) {
        dprint("I2C error (maybe...)");
    }

    prox_th = get_prox_th();
    sprintf(debug_str, "Prox Th %d;", prox_th);
    SEND_STRING(debug_str);
    wait_ms(10);
    dprint(debug_str);
    if (global_ati_c == 0) {
        dprint("I2C error (maybe...)");
    }
    global_touch_multiplier_set = get_global_touch_multiplier_set();
    sprintf(debug_str, "Global Touch Multiplier - Set %d;", global_touch_multiplier_set);
    SEND_STRING(debug_str);
    wait_ms(10);            
    dprint(debug_str);
    if (global_ati_c == 0) {
        dprint("I2C error (maybe...)");
    }

    global_touch_multiplier_clear = get_global_touch_multiplier_clear();
    sprintf(debug_str, "Global Touch Multiplier - clear %d;", global_touch_multiplier_clear);
    SEND_STRING(debug_str);
    wait_ms(10);
    dprint(debug_str);
    if (global_ati_c == 0) {
        dprint("I2C error (maybe...)");
    }

    sprintf(debug_str, "t:%d;df:%d,%d,%d;", get_reg8(0x0011), get_reg8(0x0637), get_reg8(0x0638), get_reg16(0x0639));
    SEND_STRING(debug_str);
    wait_ms(10);
    dprint(debug_str);
    return;
}    

uint16_t dump_reg(uint16_t start_addr, uint16_t end_addr) {
    uint8_t reg_value;
    uint16_t addr;
    //    i2c_status_t status;
    //    sprintf(debug_str, "\naddr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    //    SEND_STRING(debug_str);
    //    wait_ms(10);
    for (addr = start_addr; addr <= end_addr; addr++) {
        if ((addr == start_addr) || ((addr & 0x000f) == 0)) {
            sprintf(debug_str, "\n%04X: ", addr);
            SEND_STRING(debug_str);
            wait_ms(10);
        }
        reg_value = get_reg8(addr);
        sprintf(debug_str, "%02X ", reg_value);
        SEND_STRING(debug_str);
        wait_ms(10);
    }
    return(addr);
}
