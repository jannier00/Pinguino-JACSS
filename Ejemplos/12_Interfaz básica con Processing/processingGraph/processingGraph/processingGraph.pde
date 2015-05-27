import processing.serial.*;

Serial GPiCPort;                          //Declara el nombre del puerto serial
int endChar = 13;                         //Equivalente a \r
String incomingString = "None";           //Variable para cuardar la cagena recibida

void setup() 
{
  size(300, 200);                         // Define el tamaño de la ventana
  
  //El numero de puerto COM debe coincidir con el de la G-PiC Lite!
  GPiCPort = new Serial(this, "COM4", 9600);// Se configura el puerto Serial
  
  GPiCPort.bufferUntil(endChar);          //Guarda caracteres recibidos por USB
                                          //y genera un serialEvent() cuando recibe
                                          //un caracter específico.
}

void draw()                         //Equivalente al while(true)    
{
  background(0);                    // Define el color de fondo (negro)
  fill(255);                        // Define el color de relleno (blanco)
  
  text("G-PiC Lite! Processing Demo", 70, 20);
  
  //Imprime el texto con el valor de la temperatura
  text("Temperature: " + incomingString + " °C", 5, 80);
   
  // Dibuja la grafica
  float temperature = float(incomingString);  //Convierte la cadena de caracteres
                                              //en un numero float
  temperature *= -1;
  text("_100°C", 220, 50);
  text("_80 °C", 220, 70);
  text("_60 °C", 220, 90);
  text("_40 °C", 220, 110);
  text("_20 °C", 220, 130);
  text("_0  °C", 220, 150);
  
  fill(255, 0, 0);                  // Define el color de relleno (rojo)                  
  rect(195, 150, 15, temperature);  // Dibuja el grafica de barra
}

//Codigo que se ejecuta cuando recibe un \r por USB
void serialEvent(Serial GPiCPort)   
{
  incomingString = GPiCPort.readString();  //Lee los caracteres recibidos 
}
