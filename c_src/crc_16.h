#ifndef CRC_16_H
#define CRC_16_H

#include <erl_nif.h>
#include <stdint.h>

ERL_NIF_TERM handle_crc_16(ErlNifEnv* env, const ERL_NIF_TERM argv[]);
void gen_crc_table_16(uint16_t *crc_table, uint16_t polynomial);
void gen_crc_table_16_r(uint16_t *crc_table, uint16_t polynomial);
uint16_t calc_crc_16(uint16_t *crc_table, uint16_t init_val, uint16_t final_xor_val, uint8_t *bytes, unsigned len);
uint16_t calc_crc_16_r(uint16_t *crc_table, uint16_t init_val, uint16_t final_xor_val, uint8_t *bytes, unsigned len);
uint16_t reflect_16(uint16_t value);

#endif