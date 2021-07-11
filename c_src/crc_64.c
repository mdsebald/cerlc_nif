//
// Handle 64-bit CRC generation and calculation
//

#include "cerlc_nif.h"
#include "crc_64.h"

ERL_NIF_TERM handle_crc_64(ErlNifEnv* env, const ERL_NIF_TERM argv[])
{
    uint64_t polynomial;
    uint64_t init_value;
    uint64_t final_xor_value;
    uint64_t *crc_table;
    char reflected_str[6]; // "true" or "false"
    unsigned reflected = FALSE;


    debug("handle_crc_64");

    if(!enif_get_uint64(env, argv[POLYNOMIAL], &polynomial)) {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint64(env, argv[INIT_VALUE], &init_value)) {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint64(env, argv[FINAL_XOR_VALUE], &final_xor_value)) {
        return enif_make_badarg(env);
    }

    if(!enif_get_atom(env, argv[REFLECTED], reflected_str, 6, ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    if (!strncmp("true", reflected_str, 4)) {
        reflected = TRUE;
    }

    struct CerlcNifPriv *priv = enif_priv_data(env);
    struct CerlcNifConfig *cerlc_nif_config = enif_alloc_resource(priv->cerlc_nif_config_type, sizeof(struct CerlcNifConfig));
    
    cerlc_nif_config->bits = 64;
    cerlc_nif_config->reflected = reflected;
    cerlc_nif_config->final_xor_value_64 = final_xor_value;

    crc_table = (uint64_t *) enif_alloc(sizeof(uint64_t) * 256);
    if (reflected) {
        cerlc_nif_config->init_value_64 = reflect_64(init_value);
        gen_crc_table_64_r(crc_table, reflect_64(polynomial));
    }
    else {
        cerlc_nif_config->init_value_64 = init_value;
        gen_crc_table_64(crc_table, polynomial);
    }

    cerlc_nif_config->crc_table = crc_table;

    ERL_NIF_TERM res_term = enif_make_resource(env, cerlc_nif_config);

    // Erlang side owns the resource now. Safe for NIF side to release it.
    enif_release_resource(cerlc_nif_config);

    return(res_term);
}

void gen_crc_table_64(uint64_t *crc_table, uint64_t polynomial)
{
    uint64_t curr_byte;

    debug("gen_crc_table_64");

    for (uint64_t index = 0; index < 256; index++) {
        curr_byte = index << 56;
        for (unsigned curr_bit = 0; curr_bit < 8; curr_bit++) {
            if (curr_byte & 0x8000000000000000) {
                curr_byte = (curr_byte << 1) ^ polynomial;
            }
            else {
                curr_byte <<= 1;
            }
        }
        crc_table[index] = curr_byte;
    }
    char debug_str[500];
    sprintf(debug_str, "%lx, %lx, %lx, %lx", crc_table[0], crc_table[1], crc_table[2], crc_table[3]);
    debug(debug_str);
}

void gen_crc_table_64_r(uint64_t *crc_table, uint64_t polynomial)
{
    uint64_t curr_byte;

    debug("gen_crc_table_64_r");

    for (unsigned index = 0; index < 256; index++) {
        curr_byte = index;
        for (unsigned curr_bit = 0; curr_bit < 8; curr_bit++) {
            if (curr_byte & 0x01) {
                curr_byte = (curr_byte >> 1) ^ polynomial;
            }
            else {
                curr_byte >>= 1;
            }
        }
        crc_table[index] = curr_byte;
    }
    char debug_str[500];
    sprintf(debug_str, "%lx, %lx, %lx, %lx", crc_table[0], crc_table[1], crc_table[2], crc_table[3]);
    debug(debug_str);
}

uint64_t calc_crc_64(uint64_t *crc_table, uint64_t init_val, uint64_t final_xor_val, uint8_t *bytes, unsigned len)
{
    uint64_t crc;
    uint8_t pos;
    
    debug("calc_crc_64");

    crc = init_val;
    
    for (unsigned index = 0; index < len; ++index) {
        pos = bytes[index] ^ (crc >> 56);
  		crc = crc_table[pos] ^ (crc << 8);
    }

    return (crc ^ final_xor_val);
}

uint64_t calc_crc_64_r(uint64_t *crc_table, uint64_t init_val, uint64_t final_xor_val, uint8_t *bytes, unsigned len)
{
    uint64_t crc;
    uint8_t pos;
    
    debug("calc_crc_64_r");

    crc = init_val;
    
    for (unsigned index = 0; index < len; ++index) {
        pos = (crc ^ bytes[index]) & 0xFF;
        crc = crc_table[pos] ^ (crc >> 8);
    }
    return (crc ^ final_xor_val);
}

uint64_t reflect_64(uint64_t data)
{
	uint64_t  reflection = 0;

	for (unsigned bit = 0; bit < 64; ++bit) {
		if (data & ((uint64_t)1 << bit)) {
			reflection |= ((uint64_t)1 << ((64-1) - bit));
		}
	}
	return (reflection);
}
