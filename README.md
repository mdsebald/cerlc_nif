# cerlc_nif

## A Custom Configurable Cyclic Redundancy Check (CRC), Function Generator
 
[![Erlang CI](https://github.com/mdsebald/cerlc_nif/workflows/Erlang%20CI/badge.svg)](https://github.com/mdsebald/cerlc_nif/actions)

### Implemented in native Erlang and NIFs

#### Select from over 50 predefined CRC algorithms.  
    See src/cerlc_nif.erl for complete list

#### OR Generate custom CRC algorithms by manually specifying the parameters. 
    bit width, polynomial, initial value, final XOR value, and data reflected or not 

### Build
```
    $ rebar3 compile
```

### Test
```
    $ rebar3 eunit
```

### Use

#### In an Erlang application
```erlang

% Add to list of dependencies in rebar.config
{deps, [
    {cerlc_nif, "0.1.0"}
]}.


% Example: Using a predefined CRC algorithm
CrcDefn = cerlc_nif:init(crc16_aug_ccitt),

% Data is an iolist (binary, list of bytes, or some combination of the two)
Crc = cerlc_nif:calc_crc(Data, CrcDefn)


% Example: Creating a custom CRC algorithm
% Custom CRC parameters: {Bits, Polynomial, InitValue, FinalXorValue, Reflected}
CustomDefn = cerlc_nif:init(8, 16#4F, 0, 16#FF, false),

Crc = cerlc_nif:calc_crc(Data, CustomDefn)


% Example: Using a macro to evaluate init() function at compile time
-define(CRC_DEFN, cerlc_nif:init(crc16_aug_ccitt)).

Crc = cerlc_nif:calc_crc(Data, ?CRC_DEFN),


```

#### In an Elixir application
```elixir

# Add to list of dependencies in mix.exs
def deps do
  [
    {:cerlc_nif, "~> 0.1.0"}
  ]
end


# Example: Using a predefined CRC algorithm
crc8_defn = :cerlc_nif.init(:crc8)

# Data is an iolist (binary, list of bytes, or some combination of the two)
crc = :cerlc_nif.calc_crc(data, crc8_defn)


# Example: Creating a custom CRC algorithm
# Custom CRC parameters: {bits, polynomial, init_value, final_xor_value, reflected}
custom_defn = cerlc_nif:init(16, 0x1234, 0xFFFF, 0xFFFF, true)

crc = :cerlc_nif.calc_crc(data, custom_defn)


# Example: Using a module attribute to evaluate init() function at compile time
@crc32_defn :cerlc_nif.init(:crc32_c)

crc = :cerlc_nif.calc_crc(data, @crc32_defn)


```

### Performance

cerlc_nif is slower than Erlang's built in crc32() function but faster than crc calculators built using pure Erlang or Elixir

cerlc_nif is useful for quickly generating CRC's with little code and no other dependencies.

### Sources of CRC Definitions
[Javascript CRC Calculator](http://www.sunshine2k.de/coding/javascript/crc/crc_js.html)

[crccalc](https://crccalc.com/)
