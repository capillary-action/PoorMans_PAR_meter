# PoorMans_PAR_meter

***STILL A WORK IN PROGRESS***

The code is an example program for using three different light sensors to measure light levels and calculate photosynthetically active radiation (PAR) values. The sensors used are:

   - VEML7700: Measures lux levels in visible light spectrum.
   - TSL2591: Measures lux levels in visible and infrared spectrum.
   - AS7341: Measures spectral power at different wavelengths.

The program first initializes the sensors and checks for their detection. Then, it enters an infinite loop that performs the following actions:

   - Reads the spectral power at different wavelengths from the AS7341 sensor and normalizes it.
   - Reads the lux level in visible light spectrum from the VEML7700 sensor and prints it.
   - Reads the lux level in visible and infrared spectrum from the TSL2591 sensor and prints it.
   - Calculates the PAR values using the normalized spectral power and lux levels measured by the VEML7700 and TSL2591 sensors.
   - Prints the calculated PAR values.
