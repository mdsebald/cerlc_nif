//
// NIF implementation of cerlc lib
// cerlc is a configurable CRC calculator, implemented in Erlang
//

#include "cerlc_nif.h"
#include "crc_8.h"
#include "crc_16.h"
#include "crc_32.h"
#include "crc_64.h"

static void cerlc_nif_dtor(ErlNifEnv *env, void *obj)
{
    debug("cerlc_dtor");

    struct CerlcNifConfig *res = (struct CerlcNifConfig *) obj;

    enif_free(res->crc_table);
}

static int cerlc_nif_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM info)
{
#ifdef DEBUG
#ifdef LOG_PATH
    log_location = fopen(LOG_PATH, "w");
#endif
#endif
    debug("cerlc_load");

    struct CerlcNifPriv *priv = enif_alloc(sizeof(struct CerlcNifPriv));
    if (!priv) {
        error("Can't allocate cerlc priv");
        return 1;
    }

    priv->cerlc_nif_config_type = enif_open_resource_type(env, NULL, "cerlc_nif_config_type", cerlc_nif_dtor, ERL_NIF_RT_CREATE, NULL);
    if (priv->cerlc_nif_config_type == NULL) {
        error("open cerlc NIF resource type failed");
        return 1;
    }

    priv->atom_ok = enif_make_atom(env, "ok");
    priv->atom_error = enif_make_atom(env, "error");

    *priv_data = priv;
    return 0;
}

static void cerlc_nif_unload(ErlNifEnv *env, void *priv_data)
{
    debug("cerlc_unload");
    struct CerlcNifPriv *priv = enif_priv_data(env);

    enif_free(priv);
}

static ERL_NIF_TERM init_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    // init_nif() has 5 arguments:
    // Bits, Polynomial, InitValue, FinalXorValue, Reflected

    unsigned bits;

    if (!enif_get_uint(env, argv[BITS], &bits)) {
        return enif_make_badarg(env);
    }

    switch (bits) {
        case 8: return handle_crc_8(env, argv);
        case 16: return handle_crc_16(env, argv);
        case 32: return handle_crc_32(env, argv);
        case 64: return handle_crc_64(env, argv);

        // Not a valid bits value
        default:
            return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM calc_crc_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{

    struct CerlcNifConfig *crc_config;
    ErlNifBinary bin;

    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin)) {
        return enif_make_badarg(env);
    }

    struct CerlcNifPriv *priv = enif_priv_data(env);

    if (!enif_get_resource(env, argv[1], priv->cerlc_nif_config_type, (void **)&crc_config)) {
        return enif_make_badarg(env);
    }

    switch(crc_config->bits) {
        case 8: 
            return enif_make_uint(env, calc_crc_8(crc_config->crc_table, crc_config->init_value, crc_config->final_xor_value, bin.data, bin.size));

        case 16:
            if (crc_config->reflected) {
                return enif_make_uint(env, calc_crc_16_r(crc_config->crc_table, crc_config->init_value, crc_config->final_xor_value, bin.data, bin.size));
            } else {
                return enif_make_uint(env, calc_crc_16(crc_config->crc_table, crc_config->init_value, crc_config->final_xor_value, bin.data, bin.size));
            }

        case 32:
            if (crc_config->reflected) {
                return enif_make_uint(env, calc_crc_32_r(crc_config->crc_table, crc_config->init_value, crc_config->final_xor_value, bin.data, bin.size));
            } else {
                return enif_make_uint(env, calc_crc_32(crc_config->crc_table, crc_config->init_value, crc_config->final_xor_value, bin.data, bin.size));
            }

        case 64:
            if (crc_config->reflected) {
                return enif_make_uint64(env, calc_crc_64_r(crc_config->crc_table, crc_config->init_value_64, crc_config->final_xor_value_64, bin.data, bin.size));
            } else {
                return enif_make_uint64(env, calc_crc_64(crc_config->crc_table, crc_config->init_value_64, crc_config->final_xor_value_64, bin.data, bin.size));
            }
 
        // Not a valid bits value
        default:
            return enif_make_badarg(env);
    }
}

static ErlNifFunc nif_funcs[] = {
    {"init", 5, init_nif},
    {"calc_crc", 2, calc_crc_nif}
};

ERL_NIF_INIT(cerlc_nif, nif_funcs, cerlc_nif_load, NULL, NULL, cerlc_nif_unload)
