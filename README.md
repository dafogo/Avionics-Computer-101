# **Avionics-Computer-101**

## Pin Distribution

|       Name        |   Pin     |
|       ---         |   :---:   |
|   pyro_read       |   D13     |
|   pyro_read2      |   D12     |
|   pyro_trigger    |   D4      |
|   pyro_trigger2   |   D34     |
|   arm_button      |   D16     |
|   reset_button    |   D17     |
|   orangeLED       |   D14     |
|   greenLED        |   D27     |



## Register Data

|    Variable    |  Values Range |  DataType |
|       ---      |     :---:     |   :---:   |
|    - Tiempo    |               |           |
|    - Estado    |   1 - 6       |   uint8   |
|    - Altura    |   0 - 1500    |   float   |
|    - Pitch     |               |   float   |
|    - Roll      |               |   float   |
|    - Yaw       |               |   float   |
|    - xAccel    |   -200 - 200  |   float   |
|    - yAccel    |   -200 - 200  |   float   |
|    - zAccel    |   -200 - 200  |   float   |


EEPROM Capacity = 512000 bits = 64000 bytes - 10 (Control Variables)

Quantity of Registers that can be allocated = 2206 = aprox. 2000 = 9 mins at a rate of 4 per second.
