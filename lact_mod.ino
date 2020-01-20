// Ejemplo de prototipo de medidor de pulso

// la variable pulso contiene los datos brutos entrantes pudiendo  variar entre 0-1024

int pulso; // Contiene los datos de la lectura en bruto 

int limite = 550; // Determina que señal se va a interpretar como pulso

void setup() {

  // Inicilizamos el puerto serial
  Serial.begin(9600);
}

void loop() {

  pulso = analogRead(36); // Leemos de la entrada analogica 36 y guardamos el valor

  // Envíe el valor de pulso al Plotter serial. Comentar si queremos visualizar en “serial ploter”
  Serial.println(pulso);


  //Retardo de 35ms
  delay(35);
}
