#define PYRO_READ 	    D13
#define PYRO_READ2 	    D12
#define PYRO_TRIGGER 	D4
#define PYRO_TRIGGER2 	D34
#define ARM_BUTTON 	    D16
#define RESET_BUTTON 	D17
#define ORANGE_LED 	    D14
#define GREEN_LED 	    D27

void pins_setup(){
    pinMode(ARM_BUTTON, INPUT); // Initialization Arming Button
    pinMode(RESET_BUTTON, INPUT); // Initialization Reset Button

    pinMode(PYRO_READ, INPUT); // Initialization Pyro Read
    pinMode(PYRO_TRIGGER, OUTPUT); // Initialization Pyro Trigger
    pinMode(PYRO_READ2, INPUT); // Initialization Pyro2 Read
    pinMode(PYRO_TRIGGER2, OUTPUT); // Initialization Pyro2 Trigger

    digitalWrite(PYRO_TRIGGER, LOW);
    digitalWrite(PYRO_TRIGGER2, LOW);
}