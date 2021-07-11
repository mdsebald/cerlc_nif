%
% @doc A NIF implementatio of a configurable cyclic redundancy check (CRC) generator
% 

-module(cerlc_nif).

-export([init/1, init/5, calc_crc/2]).

-on_load(load_nif/0).

% Attempt to load the 'C' NIF implementation of this library
load_nif() -> erlang:load_nif("./priv/cerlc_nif", 0).

% @doc
% Generate a record containing the CRC calculation function
% and associated configuration values,
% for the given CRC algorithm definition
%

-spec init(integer(), integer(), integer(), integer(), boolean()) -> any().
init(_Bits, _Polynomial, _InitValue, _FinalXorValue, _Reflected) ->
  erlang:nif_error(nif_not_loaded).

% Convert CRC name to config parameters
-spec init(atom()) -> any().
init(crc8) -> init(8, 16#07, 16#00, 16#00, false);
init(crc8_sae_j1850) -> init(8, 16#1D, 16#FF, 16#FF, false);
init(crc8_sae_j1850_zero) -> init(8, 16#1D, 16#00, 16#00, false);
init(crc8_8h2f) -> init(8, 16#2F, 16#FF, 16#FF, false);
init(crc8_cdma2000) -> init(8, 16#9B, 16#FF, 16#00, false);
init(crc8_darc) -> init(8, 16#39, 16#00, 16#00, true);
init(crc8_dvb_s2) -> init(8, 16#D5, 16#00, 16#00, false);
init(crc8_ebu) -> init(8, 16#1D, 16#FF, 16#00, true);
init(crc8_icode) -> init(8, 16#1D, 16#FD, 16#00, false);
init(crc8_itu) -> init(8, 16#07, 16#00, 16#55, false);
init(crc8_maxim) -> init(8, 16#31, 16#00, 16#00, true);
init(crc8_sensirion) -> init(8, 16#31, 16#FF, 16#00, false);
init(crc8_rohc) -> init(8, 16#07, 16#FF, 16#00, true);
init(crc8_wcdma) -> init(8, 16#9B, 16#00, 16#00, true);
init(crc16_ccitt_zero) -> init(16, 16#1021, 16#0000, 16#0000, false);
init(crc16_arc) -> init(16, 16#8005, 16#0000, 16#0000, true);
init(crc16_aug_ccitt) -> init(16, 16#1021, 16#1D0F, 16#0000, false);
init(crc16_buypass) -> init(16, 16#8005, 16#0000, 16#0000, false);
init(crc16_ccitt_false) -> init(16, 16#1021, 16#FFFF, 16#0000, false);
init(crc16_cdma2000) -> init(16, 16#C867, 16#FFFF, 16#0000, false);
init(crc16_dds_110) -> init(16, 16#8005, 16#800D, 16#0000, false);
init(crc16_dect_r) -> init(16, 16#0589, 16#0000, 16#0001, false);
init(crc16_dect_x) -> init(16, 16#0589, 16#0000, 16#0000, false);
init(crc16_dnp) -> init(16, 16#3D65, 16#0000, 16#FFFF, true);
init(crc16_en_13757) -> init(16, 16#3D65, 16#0000, 16#FFFF, false);
init(crc16_genibus) -> init(16, 16#1021, 16#FFFF, 16#FFFF, false);
init(crc16_maxim) -> init(16, 16#8005, 16#0000, 16#FFFF, true);
init(crc16_mcrf4xx) -> init(16, 16#1021, 16#FFFF, 16#0000, true);
init(crc16_riello) -> init(16, 16#1021, 16#B2AA, 16#0000, true);
init(crc16_t10_dif) -> init(16, 16#8BB7, 16#0000, 16#0000, false);
init(crc16_teledisk) -> init(16, 16#A097, 16#0000, 16#0000, false);
init(crc16_tms37157) -> init(16, 16#1021, 16#89EC, 16#0000, true);
init(crc16_usb) -> init(16, 16#8005, 16#FFFF, 16#FFFF, true);
init(crc16_a) -> init(16, 16#1021, 16#C6C6, 16#0000, true);
init(crc16_kermit) -> init(16, 16#1021, 16#0000, 16#0000, true);
init(crc16_modbus) -> init(16, 16#8005, 16#FFFF, 16#0000, true);
init(crc16_x_25) -> init(16, 16#1021, 16#FFFF, 16#FFFF, true);
init(crc16_xmodem) -> init(16, 16#1021, 16#0000, 16#0000, false);
init(crc32) -> init(32, 16#04C11DB7, 16#FFFFFFFF, 16#FFFFFFFF, true);
init(crc32_bzip2) -> init(32, 16#04C11DB7, 16#FFFFFFFF, 16#FFFFFFFF, false);
init(crc32_c) -> init(32, 16#1EDC6F41, 16#FFFFFFFF, 16#FFFFFFFF, true);
init(crc32_d) -> init(32, 16#A833982B, 16#FFFFFFFF, 16#FFFFFFFF, true);
init(crc32_mpeg2) -> init(32, 16#04C11DB7, 16#FFFFFFFF, 16#00000000, false);
init(crc32_posix) -> init(32, 16#04C11DB7, 16#00000000, 16#FFFFFFFF, false);
init(crc32_q) -> init(32, 16#814141AB, 16#00000000, 16#00000000, false);
init(crc32_jamcrc) -> init(32, 16#04C11DB7, 16#FFFFFFFF, 16#00000000, true);
init(crc32_xfer) -> init(32, 16#000000AF, 16#00000000, 16#00000000, false);
init(crc64_ecma_182) -> init(64, 16#42F0E1EBA9EA3693, 16#0, 16#0, false);

init(crc64_go_iso) ->
  init(64, 16#000000000000001B, 16#FFFFFFFFFFFFFFFF, 16#FFFFFFFFFFFFFFFF, true);

init(crc64_we) ->
  init(64, 16#42F0E1EBA9EA3693, 16#FFFFFFFFFFFFFFFF, 16#FFFFFFFFFFFFFFFF, false);

init(crc64_xz) ->
  init(64, 16#42F0E1EBA9EA3693, 16#FFFFFFFFFFFFFFFF, 16#FFFFFFFFFFFFFFFF, true).

-spec calc_crc(iolist(), any()) -> integer().
calc_crc(_Bytes, _CrcConfig) ->
  erlang:nif_error(nif_not_loaded).
