import processing.serial.*;
Serial myPort;
char command;
void setup()
{
  myPort = new Serial(this,"",115200);
}

void draw()
{
}

void keyReleased() {
  myPort.write(key);
}
