import processing.serial.*;
Serial myPort;
char command;
void setup()
{
  myPort = new Serial(this,"/dev/cu.RNBT-8C48-RNI-SPP",115200);
}

void draw()
{
}

void keyReleased() {
  myPort.write(key);
}
