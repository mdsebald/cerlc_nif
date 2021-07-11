#ifndef CRC_32_H
#define CRC_32_H

#include <erl_nif.h>
#include <stdint.h>

ERL_NIF_TERM handle_crc_32(ErlNifEnv* env, const ERL_NIF_TERM argv[]);
void gen_crc_table_32(uint32_t *crc_table, uint32_t polynomial);
void gen_crc_table_32_r(uint32_t *crc_table, uint32_t polynomial);
uint32_t calc_crc_32(uint32_t *crc_table, uint32_t init_val, uint32_t final_xor_val, uint8_t *bytes, unsigned len);
uint32_t calc_crc_32_r(uint32_t *crc_table, uint32_t init_val, uint32_t final_xor_val, uint8_t *bytes, unsigned len);
uint32_t reflect_32(uint32_t value);

#endif