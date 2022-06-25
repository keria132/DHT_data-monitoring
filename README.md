## Adruino (DHT, TM1637) data monitoring
My oldest project for arduino, which purpose is to get temperature and humidity
data from dht sensor and then display it on TM1637.
It also has timer which counts working hours (I don't really know why i wrote it)
All you need to do is connect sensor, display (and optionally button and led's)
to these arduino pins:
- TM1637 display connected to the (2 - DIO, 3-CLK)
- DHT pin (6)
- Button to switch between timer and sensor data (9)
- Indicator led's (4-RED blinks when data is corrupted, 5-GREEN blinks when everything's fine)