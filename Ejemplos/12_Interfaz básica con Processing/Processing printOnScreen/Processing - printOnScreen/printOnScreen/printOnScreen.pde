import processing.serial.*;

Serial GPiCPort;                            //Declara el nombre del puerto serial
int endChar = 13;                           //Equivalente a \r
String incomingString = "None";

void setup() 
{
  size(200, 200);                           //Define el tamaño de la ventana
  GPiCPort = new Serial(this, "COM4", 9600);// Se configura el puerto Serial
  GPiCPort.bufferUntil(1);
  fill(255);                                // Define el color de relleno (blanco)
}

void draw()
{
  background(0);
  text(incomingString, 5, 80);
}

void serialEvent(Serial GPiCPort)
{
  incomingString = GPiCPort.readString(); 
}
