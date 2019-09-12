# xbtbinfileutils
A c++ library that contains classes to work wih XBT bin files generated by Amverseas
The libray can encode and decode message types 1, 2, 3 and 4.
A type 4 message is essentially a type 3 message that stores resistace values instead of temperature values.
Storing actual raw data instead of the temperature values is useful for converting data between the different fall rate equations and applying resistance offsets when calibrating hardware configurations.
Type 4 messages are intended to be used during the calibration stage of a setup.



Disclaimer
==========
This repository is a scientific product and is not official communication of the National Oceanic and
Atmospheric Administration, or the United States Department of Commerce. All NOAA GitHub project code is
provided on an ‘as is’ basis and the user assumes responsibility for its use. Any claims against the Department of
Commerce or Department of Commerce bureaus stemming from the use of this GitHub project will be governed
by all applicable Federal law. Any reference to specific commercial products, processes, or services by service
mark, trademark, manufacturer, or otherwise, does not constitute or imply their endorsement, recommendation or
favoring by the Department of Commerce. The Department of Commerce seal and logo, or the seal and logo of a
DOC bureau, shall not be used in any manner to imply endorsement of any commercial product or activity by
DOC or the United States Government.

-----------------------------------
**COMPILING AND USING THE LIBRARY**
------------------------------------

To compile under Ubuntu 18.04
install make and g++
```bash
sudo apt-get update
sudo apt-get install make g++
```

For a Release version

```bash
cd Release
make
```

For a Debug version

```bash
cd Debug
make
```

The generated library will be in the directory you ran the make command from.
```
libxbtbinfileutils.so
```




