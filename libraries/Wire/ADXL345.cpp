#include "Arduino.h"
#include "Wire.h"
#include "ADXL345.h"

ADXL345::ADXL345(){
}

void ADXL345::start(byte bw_code){
  set_bit(ADXL345_POWER_CTL_ADDRESS, 3);
  set_bit(ADXL345_DATA_FORMAT_ADDRESS, 3);
  set_bw(bw_code);
}

boolean ADXL345::stop(){
  return clear_bit(ADXL345_POWER_CTL_ADDRESS, 3);
}

boolean ADXL345::read_accel(int *xyz){
  byte data_in[6];
  byte i;
  int val;
  bool status = true;

  if(read(ADXL345_DATA_ADDRESS, data_in, 6)){
    for(i = 0; i < 3; i++){
      xyz[i] = (((int)data_in[2 * i + 1]) << 8) | data_in[2 * i];
    }
  }
  else{
    status = false;
  }
  return status;
}

boolean ADXL345::selftest(){
  // set data rate to > 100 Hz (write 0x0A to range bits DATA_FORMAT address 0x31)
  // collect and average at least 10 samples (xst_off, yst_off, and zst_off)
  // enable self-test bit D7 of DATA_FORMAT reg 0x31
  // measure at least 4 samples discarding data
  // collect and average at least 10 samples xst_on, yst_on, zst_on
  // clear self-test bit D7 of DATA_FORMAT
  // compute xst = xst_on - xst_off
  //         yst = yst_on - yst_off
  //         zst = zst_on - zst_off
  // test xst, yst, zst
  return true;
}

boolean ADXL345::set_bw(byte bw_code){
  boolean status = true;

  if((bw_code < ADXL345_BW_3) || (bw_code > ADXL345_BW_1600)){
    status = false;
  }
  else{
    write_one(ADXL345_BW_RATE_ADDRESS, bw_code);
  }
  return status;
}

boolean ADXL345::set_range_bits(byte range_bits){
  /* range:
   * 0 --- +- 2G
   * 1 --- +- 4G
   * 2 --- +- 8G
   * 3 --- +- 16G
   */
  boolean status = true;
  byte bit;

  for(bit=0; status && bit < 2; bit++){
    if(range_bits & (1 << bit)){
    status = set_bit(ADXL345_DATA_FORMAT_ADDRESS, bit);
    }
    else{
      status = clear_bit(ADXL345_DATA_FORMAT_ADDRESS, 0);
    }
  }
  return status;
}

boolean ADXL345::get_range_bits(byte range_bits){
  /* range:
   * 0 --- +- 2G
   * 1 --- +- 4G
   * 2 --- +- 8G
   * 3 --- +- 16G
   */
  boolean status = read_one(ADXL345_DATA_FORMAT_ADDRESS, &range_bits);
  if(status){
    range_bits %= 4;
  }
  return status;
}

void ADXL345::set_offsets(double *offsets){}
boolean ADXL345::get_offsets(double *offsets){}
void ADXL345::set_scales(double *offsets){}
boolean ADXL345::get_scales(double *offsets){}

boolean ADXL345::read_one(byte address, byte *datum){
  byte* data[1];
  boolean status = true;

  if(read(address, datum, 1)){
  }
  else{
    status = false;
  }
  return status;
}
boolean ADXL345::read(byte address, byte* data, byte n_byte){
  boolean status = true;

  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.send(address);        
  Wire.endTransmission();              
  
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.requestFrom((int)ADXL345_ADDRESS, (int)n_byte);
  
  int i = 0;
  while(Wire.available() && i < n_byte){ 
    data[i] = Wire.receive();
    i++;
  }
  if(i == n_byte){
  }
  else{
    status = false;
  }
  delay(5); // not required with I2C comm
  return status;
}

void ADXL345::write_one(byte address, byte datum){
  write(address, &datum, 1);
}

void ADXL345::write(byte address, byte* data, byte n_byte){
  byte i;
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.send(address);
  for(i = 0; i < n_byte; i++){
    Wire.send(data[i]);
  }
  delay(5); // not required with I2C comm
  Wire.endTransmission();
}

boolean ADXL345::set_bit(byte address, byte bit){
  byte datum;
  boolean status = true;
  if(read_one(address, &datum)){
    datum |= (1 << bit); // set bit
    write_one(address, datum); 
  }
  else{
    status = false;
  }
  return status;
}

boolean ADXL345::clear_bit(byte address, byte bit){
  byte datum;
  boolean status = true;

  if(read_one(address, &datum)){
    datum &= ~(datum << bit); // clear bit
    write_one(address, datum);
  }
  else{
    status = false;
  }
  return status;
}

