# edf2bin

Converts an export data file (EDF) to an Amverseas binary XBT profile using the
xbtbinfileutils library.

Every EDF layout is read:

| Title line | Written by |
|---|---|
| `// This is a MK21 EXPORT DATA FILE  (EDF)` | WinMK21 |
| `// MK21 EXPORT DATA FILE  (EDF)` | MK21 software |
| `// AOML EXPORT DATA FILE  (EDF)` | xbtbinfileutils (`bin2ascii` type 1) |

## Usage

```bash
edf2bin [options] [input.edf | -]
```

The EDF is read from standard input when no file or `-` is given. The binary is
written to standard output unless `-o` is used.

| Option | |
|---|---|
| `-T, --type NUMBER` | message type to save, 1 to 4 (default 3) |
| `-o, --output FILE` | write the binary to FILE (`-` is standard output) |
| `-q, --quiet` | do not print warnings |
| `-h, --help` | show the help |

EDF files do not hold the ship and environment metadata of an Amverseas
profile, these options add it. A value that does not fit its field stops the
conversion, an option the chosen message type has no field for prints a
warning.

| Option | |
|---|---|
| `-c, --callsign TEXT` | ship call sign, up to 9 characters |
| `-s, --ship-name TEXT` | ship name, up to 30 characters (AOML files hold it in the memo) |
| `-i, --imo NUMBER` | ship IMO (Lloyds) number |
| `-l, --soop-line TEXT` | SOOP line, e.g. AX07 |
| `-t, --transect NUMBER` | transect number |
| `--sequence NUMBER` | sequence number, replaces the one of the EDF |
| `--seas-version NUMBER` | Amverseas version |
| `--data-quality NUMBER` | 0 not suspect (default), 1 slightly, 2 highly suspect, 3 unfit |
| `--probe CODE` | WMO probe code, replaces the one found from the probe type |
| `--recorder CODE` | WMO recorder code, replaces the one found from the MK21 device |
| `--recorder-serial TEXT` | recorder serial number, up to 8 characters |
| `--recorder-date YYYY-MM-DD` | date the recorder was manufactured |
| `--probe-date YYYY-MM-DD` | date the probe was manufactured |
| `--launcher CODE` | WMO launcher code |
| `--launch-height METERS` | height of the launch above the sea |
| `--ship-direction DEGREES` | ship heading |
| `--ship-speed M/S` | ship speed in meters per second |
| `--wind-instrument CODE` | WMO wind instrument code |
| `--wind-direction DEGREES` | wind direction |
| `--wind-speed M/S` | wind speed in meters per second |
| `--air-temperature CELSIUS` | dry bulb temperature, stored in kelvin |
| `--current-method CODE` | WMO sea surface current measurement method code |
| `--current-direction DEGREES` | sea surface current direction |
| `--current-speed M/S` | sea surface current speed in meters per second |
| `--water-depth METERS` | total water depth |
| `--agency CODE` | WMO code of the agency that owns the platform |
| `--rider-name TEXT` | rider name |
| `--rider-email TEXT` | rider email |
| `--rider-institution TEXT` | rider institution |
| `--rider-phone TEXT` | rider phone number |

```bash
edf2bin PODS16001.edf -o PODS16001.bin
edf2bin --type 4 PODS16001.edf -o PODS16001.bin
edf2bin -c WTEC -s "Oleander" --ship-speed 9.5 PODS16001.edf -o PODS16001.bin
cat TD_00001.EDF | edf2bin -c WTEC --rider-name "Jane Doe" > TD_00001.bin
```

## Message types

- **Type 4** keeps the resistance values. The EDF values that have a field in
  the binary are stored there (date, position, probe, recorder, serial
  number), type 4 adds the launch second, the salinity and one text block with
  the header lines the fields can not hold (units, MK21 device, memo, ...).
  `bin2ascii file.bin 1` writes it back as an AOML EDF, with the layout of the
  MK21 software, the position at the 0.00001 degree resolution of the binary
  and the name of the binary as the raw data filename.
  Files without resistances store their temperatures instead, no resistances
  are made up for them (see the xbtbinfileutils README for the encoding).
- **Types 1 to 3** store the temperatures, the probe, recorder, position, date
  and the fields of that type. Temperatures are stored in 12 bits, so values
  outside of -4.00 to 36.94 °C (e.g. the 37.07 °C the MK21 writes when the wire
  breaks) are limited to that range. The seconds, salinity and EDF text are
  not stored.

## Building

```bash
mkdir build && cd build
cmake ..
make
sudo make install
```

To build against a library that is not installed:

```bash
cmake -DXBTBINFILEUTILS_INCLUDE_DIR=../../xbtbinfileutils/include \
      -DXBTBINFILEUTILS_LIBRARY_DIR=../../xbtbinfileutils/build ..
```
