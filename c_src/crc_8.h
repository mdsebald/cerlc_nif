#ifndef CRC_8_H
#define CRC_8_H

#include <erl_nif.h>
#include <stdint.h>

ERL_NIF_TERM handle_crc_8(ErlNifEnv* env, const ERL_NIF_TERM argv[]);
void gen_crc_table_8(uint8_t *crc_table, uint8_t polynomial);
void gen_crc_table_8_r(uint8_t *crc_table, uint8_t polynomial);
uint8_t calc_crc_8(uint8_t *crc_table, uint8_t init_val, uint8_t final_xor_val, uint8_t *bytes, unsigned len);
uint8_t reflect_8(uint8_t value);

#endif