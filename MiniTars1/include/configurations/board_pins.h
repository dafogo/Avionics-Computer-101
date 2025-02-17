#define PYRO_READ 	    13  // D13
#define PYRO_READ2 	    12  // D12
#define PYRO_TRIGGER 	4   // D4 
#define PYRO_TRIGGER2 	34  // D34  ! se puede cambiar por 32
#define ARM_BUTTON 	    16  // D16
#define RESET_BUTTON 	17  // D17
#define ORANGE_LED 	    14  // D14
#define GREEN_LED 	    27  // D27

void pins_setup(){
    pinMode(ARM_BUTTON, INPUT); // Initialization Arming Button
    pinMode(RESET_BUTTON, INPUT); // Initialization Reset Button

    pinMode(PYRO_READ, INPUT); // Initialization Pyro Read
    pinMode(PYRO_TRIGGER, OUTPUT); // Initialization Pyro Trigger
    pinMode(PYRO_READ2, INPUT); // Initialization Pyro2 Read
    pinMode(PYRO_TRIGGER2, OUTPUT); // Initialization Pyro2 Trigger

    // digitalWrite(PYRO_TRIGGER, LOW); // checar si es lo mejor ponerlos en el setup
    // digitalWrite(PYRO_TRIGGER2, LOW);
}