# editbin – C++ Port of the Java EditBinFile Program

A portable C++ command-line tool for reading, editing, and writing Amverseas
XBT binary profiles.  It is a faithful port of `EditBinFile.java`, accepting
the same switches and producing identical binary output.

Compiles and links against the installed **xbtbinfileutils** C++ library.
It needs a version with the type 4 EDF fields (second, salinity and EDF
text) and `BinEncoder::writeOutBin`, i.e. one that installs `EDFFormat.h`.

## Building

### With CMake

```sh
cmake -B build
cmake --build build
```

If xbtbinfileutils is installed to a non-standard prefix:

```sh
cmake -B build -DXBT_INCLUDE_DIR=/path/to/include -DXBT_LIBRARY=/path/to/libxbtbinfileutils.so
cmake --build build
```

### With Make

```sh
make
```

Or with custom paths:

```sh
make XBT_INCLUDE=/path/to/include XBT_LIB=/path/to/lib
```

### Direct compilation

```sh
g++ -std=c++17 -O2 -I/path/to/xbtbinfileutils/include \
    -o editbin EditBinFile.cpp \
    -L/path/to/lib -lxbtbinfileutils
```

## Usage

```
./editbin -i input.bin [-o output.bin] [field switches …]
```

If `-o` is omitted the decoded profile is printed to stdout as text.
Use `-i -` to read binary from stdin, and `-o -` to write binary to stdout:

```sh
cat input.bin | ./editbin -i - -callsign NEWCALL -o - > output.bin
```

Run `-help` for the full list of switches.

### Text output

The text output is the same for every message type.  It lists the
fields of the library's `XBTProfile::toString()` plus the type 4 fields
(`Second`, `Sea Resistance`, `Salinity` and `EDF text`), one
`name=value` per line.  A value the profile does not hold is printed as
`N/A`, e.g. the second and salinity of a type 1 to 3 profile.  The EDF
text gets one `EDF text=` line per line of text.

The points follow as a CSV table:

```
index,depth,resistance,temperature
0,0.669077,N/A,24.940000
1,1.338110,N/A,25.130000
```

- **depth** is in meters, from the fall rate equation of the probe type
  and the sample rate of the recorder type.  It is `N/A` when the probe
  type is unknown.
- **resistance** is in ohms.  It is `N/A` for types 1 to 3, which store
  temperatures only.
- **temperature** is in °C.  For type 4 it is converted from the
  resistance.

## Command-line switches

Every switch from the Java version is supported with identical names and
semantics:

| Switch            | Type    | Description                              |
|-------------------|---------|------------------------------------------|
| `-help`           | flag    | Show help                                |
| `-i`              | string  | Input file (or `-` for stdin)            |
| `-o`              | string  | Output file (or `-` for stdout)          |
| `-callsign`       | string  | Ship callsign / WMO ID                  |
| `-messagetype`    | int     | New message type (1–4)                   |
| `-latitude`       | double  | Latitude in decimal degrees              |
| `-longitude`      | double  | Longitude in decimal degrees             |
| `-transectname`   | string  | Transect name (e.g. AX07)               |
| `-transectnum`    | int     | Transect number                          |
| `-sequencenum`    | int     | Sequence number                          |
| `-year`           | int     | 4-digit year                             |
| `-month`          | int     | 2-digit month                            |
| `-day`            | int     | 2-digit day                              |
| `-hour`           | int     | 2-digit hour                             |
| `-minute`         | int     | 2-digit minute                           |
| `-shipname`       | string  | Ship name                                |
| `-imo`            | int     | IMO (Lloyds) number                      |
| `-seasversion`    | int     | Amverseas version (integer, e.g. 930)    |
| `-probeserial`    | int     | Probe serial number                      |
| `-probeyear/month/day` | int | Probe manufacture date                |
| `-datatype`       | int     | Data resolution type                     |
| `-dataquality`    | int     | Data quality flag                        |
| `-height`         | double  | Launch height (meters)                   |
| `-shipdir`        | double  | Ship direction (degrees)                 |
| `-shipspeed`      | double  | Ship speed (m/s)                         |
| `-probetype`      | int     | Instrument type code                     |
| `-recorder`       | int     | Recorder type code                       |
| `-windtype`       | int     | Wind instrument type                     |
| `-winddir`        | double  | Wind direction                           |
| `-windspeed`      | double  | Wind speed (m/s)                         |
| `-drybulbtemp`    | double  | Dry bulb temperature                     |
| `-currentmethod`  | int     | Current measurement method               |
| `-currentdir`     | int     | Current direction                        |
| `-currentspeed`   | double  | Current speed                            |
| `-launcher`       | int     | Launcher type code                       |
| `-recorderserial` | string  | Recorder serial number                   |
| `-recorderyear/month/day` | int | Recorder manufacture date          |
| `-agency`         | int     | Agency code                              |
| `-rider`          | string  | Rider name(s)                            |
| `-rideremail`     | string  | Rider email(s)                           |
| `-riderphone`     | string  | Rider phone(s)                           |
| `-institution`    | string  | Rider institution(s)                     |
| `-depth`          | int     | Total water depth                        |
| `-temps`          | string  | Temperature points (`@`-delimited)       |
| `-res`            | string  | Resistance points (`@`-delimited)        |
| `-second`         | int     | Launch second (type 4 only)              |
| `-salinity`       | double  | Salinity in PSU (type 4 only)            |
| `-edftext`        | string  | EDF header lines not held by other fields (type 4 only) |

`-second`, `-salinity` and `-edftext` are only stored in type 4 messages; the
library drops them when encoding types 1 to 3.

### Changing message type

Types 1 to 3 store temperatures and type 4 stores resistances.  When
`-messagetype` moves a profile into or out of type 4, the points are
converted with the library's Steinhart-Hart coefficients.  Temperatures
written to types 1 to 3 are limited to what they can store (-4.00 to
36.94 °C).  `-temps` given for a type 4 profile are converted to
resistances.  `-res` always makes the profile type 4, and as in the Java
version, resistances below 3199.61 Ω (about 35.5 °C) are raised to 3199.61.

## How the C++ program maps to the Java

| Java                     | C++                                | Notes                                    |
|--------------------------|------------------------------------|------------------------------------------|
| `EditBinFile.java`       | `EditBinFile.cpp`                  | CLI + edit logic, same switches/order    |
| Apache Commons CLI       | Internal `OptionParser` (~60 lines)| No external dependency                   |
| `BinDecoder`             | From xbtbinfileutils library       | Used via `decodeXBTProfile(profile)`     |
| `BinEncoder`             | From xbtbinfileutils library       | Used via constructor + `writeOutBinFile` |
| `XBTProfile`             | From xbtbinfileutils library       | Same getters/setters                     |
| `XBTProfile.toString()`  | `profileToString()` helper         | Adds type 4 fields, depths and N/A       |
| `MessageType`            | From xbtbinfileutils library       | Same constants                           |
| stdin reading            | `decoderFromBytes()` helper        | Fills BinDecoder's public `bits` in memory |
| stdout writing           | `writeEncoderToStdout()` helper    | Calls `BinEncoder::writeOutBin(std::cout)` |
