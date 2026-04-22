#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00

void setup() {

  Serial.begin(9600);
  execute_CMD(0x3F, 0, 0);  // init
  delay(1000);
  setVolume(10);
  delay(1000);
  execute_CMD(0x08, 0, 1);
  delay(100);

}

void loop() {
  // nothing needed — module loops tracks automatically
}

void execute_CMD(byte CMD, byte Par1, byte Par2) {
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);

  byte Command_line[10] = {
    Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
  };

  for (byte k = 0; k < 10; k++) {
    Serial.write(Command_line[k]);
  }
}
void setVolume(int volume) {
  execute_CMD(0x06, 0, volume);
}