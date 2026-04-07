#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00

void setup() {
  Serial.begin(9600);
  delay(1000);
  execute_CMD(0x3F, 0, 0);  // init
  delay(500);
  setVolume(5);
  delay(500);
  playTrack(1, 10000);
  playTrack(2, 45000);
  playTrack(1, 30000);
}

void loop() {
  // nothing needed — module loops tracks automatically
}

void setVolume(int volume) {
  execute_CMD(0x06, 0, volume);
  delay(500);
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

void playTrack(int track, unsigned long duration) {
  execute_CMD(0x03, 0, track);
  delay(duration);  // wait for track to finish
}