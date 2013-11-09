#ifndef ADXL345_h
#define ADXL345_h

#include "Arduino.h"
#include "Wire.h"

#define ADXL345_ADDRESS 0x53

#define ADXL345_BW_RATE_ADDRESS     0x2C
#define ADXL345_POWER_CTL_ADDRESS   0x2D
#define ADXL345_DATA_FORMAT_ADDRESS 0x31
#define ADXL345_DATA_ADDRESS        0x32


#define ADXL345_BW_1600 0xF // 1111
#define ADXL345_BW_800  0xE // 1110
#define ADXL345_BW_400  0xD // 1101  
#define ADXL345_BW_200  0xC // 1100
#define ADXL345_BW_100  0xB // 1011  
#define ADXL345_BW_50   0xA // 1010 
#define ADXL345_BW_25   0x9 // 1001 
#define ADXL345_BW_12   0x8 // 1000 
#define ADXL345_BW_6    0x7 // 0111
#define ADXL345_BW_3    0x6 // 0110

// call Wire.begin() before using

class ADXL345{
 public:
  /* general accel methods */
  ADXL345();
  void start(byte bw_code);               // begin measurements
  boolean read_accel(int *xyz);  // return 3D accel in Gs
  boolean stop();                // end measurments
  boolean selftest();
  
  /* methods particular to ADXL345 */
  boolean set_bw(byte bw_code); 
  boolean set_range_bits(byte range_bits);    // 2, 4, 8, 16 Gs
  boolean get_range_bits(byte range_bits);    // 2, 4, 8, 16 Gs
  void set_offsets(double *offsets); // x- y- and z-offsets
  boolean get_offsets(double *offsets); // x- y- and z-offsets
  void set_scales(double *scales);   // x- y- and z-scales (Gs / count)
  boolean get_scales(double *scales);   // x- y- and z-scales (Gs / count)
  
 private:
  boolean read_one(byte address, byte* datum);
  boolean read(byte address, byte* data, byte n_byte);
  void write_one(byte address, byte datum);
  void write(byte address, byte *data, byte n_byte);
  boolean set_bit(byte address, byte bit);
  boolean clear_bit(byte address, byte bit);
};
extern ADXL345 Accel;

#endif
