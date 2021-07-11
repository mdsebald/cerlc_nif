//
// Handle 16-bit CRC generation and calculation
//

#include "cerlc_nif.h"
#include "crc_16.h"

ERL_NIF_TERM handle_crc_16(ErlNifEnv* env, const ERL_NIF_TERM argv[])
{
    unsigned polynomial;
    unsigned init_value;
    unsigned final_xor_value;
    uint16_t *crc_table;
    char reflected_str[6]; // "true" or "false"
    unsigned reflected = FALSE;

    if(!enif_get_uint(env, argv[POLYNOMIAL], &polynomial)) {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint(env, argv[INIT_VALUE], &init_value)) {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint(env, argv[FINAL_XOR_VALUE], &final_xor_value)) {
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
    
    cerlc_nif_config->bits = 16;
    cerlc_nif_config->reflected = reflected;
    cerlc_nif_config->final_xor_value = final_xor_value;

    crc_table = (uint16_t *) enif_alloc(sizeof(uint16_t) * 256);
    if (reflected) {
        cerlc_nif_config->init_value = reflect_16(init_value);
        gen_crc_table_16_r(crc_table, reflect_16(polynomial));
    }
    else {
        cerlc_nif_config->init_value = init_value;
        gen_crc_table_16(crc_table, polynomial);
    }

    cerlc_nif_config->crc_table = crc_table;

    ERL_NIF_TERM res_term = enif_make_resource(env, cerlc_nif_config);

    // Erlang side owns the resource now. Safe for NIF side to release it.
    enif_release_resource(cerlc_nif_config);

    return(res_term);
}

void gen_crc_table_16(uint16_t *crc_table, uint16_t polynomial)
{
    uint16_t curr_byte;

    for (unsigned index = 0; index < 256; index++) {
        curr_byte = index << 8;
        for (unsigned curr_bit = 0; curr_bit < 8; curr_bit++) {
            if (curr_byte & 0x8000) {
                curr_byte = (curr_byte << 1) ^ polynomial;
            }
            else {
                curr_byte <<= 1;
            }
        }
        crc_table[index] = curr_byte;
    }
}

void gen_crc_table_16_r(uint16_t *crc_table, uint16_t polynomial)
{
    uint16_t curr_byte;

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
}

uint16_t calc_crc_16(uint16_t *crc_table, uint16_t init_val, uint16_t final_xor_val, uint8_t *bytes, unsigned len)
{
    uint16_t crc;
    uint8_t pos;
    
    crc = init_val;
    
    for (unsigned index = 0; index < len; ++index) {
        pos = bytes[index] ^ (crc >> 8);
  		crc = crc_table[pos] ^ (crc << 8);
    }

    return (crc ^ final_xor_val);
}

uint16_t calc_crc_16_r(uint16_t *crc_table, uint16_t init_val, uint16_t final_xor_val, uint8_t *bytes, unsigned len)
{
    uint16_t crc;
    uint8_t pos;
    
    crc = init_val;
    
    for (unsigned index = 0; index < len; ++index) {
        pos = (crc ^ bytes[index]) & 0xFF;
        crc =  crc_table[pos] ^ (crc >> 8);
    }
    return (crc ^ final_xor_val);
}

uint16_t reflect_16(uint16_t data)
{
	uint16_t  reflection = 0;

	for (unsigned bit = 0; bit < 16; ++bit) {
		if (data & (1 << bit)) {
			reflection |= (1 << ((16-1) - bit));
		}
	}
	return (reflection);
}
