#ifndef CERLC_NIF_H
#define CERLC_NIF_H

#include <erl_nif.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//define DEBUG

#ifdef DEBUG
#define log_location stderr
//#define LOG_PATH "/tmp/circuits_i2c.log"
#define debug(...) do { enif_fprintf(log_location, __VA_ARGS__); enif_fprintf(log_location, "\r\n"); fflush(log_location); } while(0)
#define error(...) do { debug(__VA_ARGS__); } while (0)
#define start_timing() ErlNifTime __start = enif_monotonic_time(ERL_NIF_USEC)
#define elapsed_microseconds() (enif_monotonic_time(ERL_NIF_USEC) - __start)
#else
#define debug(...)
#define error(...) do { enif_fprintf(stderr, __VA_ARGS__); enif_fprintf(stderr, "\n"); } while(0)
#define start_timing()
#define elapsed_microseconds() 0
#endif

// Indices to init() CRC configuration argv[] values
#define BITS 0
#define POLYNOMIAL 1
#define INIT_VALUE 2
#define FINAL_XOR_VALUE 3
#define REFLECTED 4

#define FALSE 0
#define TRUE (!FALSE)

// cerlc NIF Private data
struct CerlcNifPriv {
    ErlNifResourceType *cerlc_nif_config_type;
    ERL_NIF_TERM atom_ok;
    ERL_NIF_TERM atom_error;
};

struct CerlcNifConfig {
  unsigned bits;
  unsigned reflected;
  unsigned init_value;
  unsigned final_xor_value;
  uint64_t init_value_64;
  uint64_t final_xor_value_64;
  void *crc_table;
};

#endif