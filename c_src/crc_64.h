#ifndef CRC_64_H
#define CRC_64_H

#include <erl_nif.h>
#include <stdint.h>

ERL_NIF_TERM handle_crc_64(ErlNifEnv* env, const ERL_NIF_TERM argv[]);
void gen_crc_table_64(uint64_t *crc_table, uint64_t polynomial);
void gen_crc_table_64_r(uint64_t *crc_table, uint64_t polynomial);
uint64_t calc_crc_64(uint64_t *crc_table, uint64_t init_val, uint64_t final_xor_val, uint8_t *bytes, unsigned len);
uint64_t calc_crc_64_r(uint64_t *crc_table, uint64_t init_val, uint64_t final_xor_val, uint8_t *bytes, unsigned len);
uint64_t reflect_64(uint64_t value);

#endif