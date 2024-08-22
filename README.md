# Smart-Fan
An automated fan that turns on and off on its own.
This device is an add-on to an existing fan by severing the hot wire and connecting each severed end to a relay.
The fan will turn on if the room temperature exceeds a defined floating-point variable (71.0 by default) or if the current time passes a user defined start of bedtime (sleep with fan on; 10pm by default).
After the current time passes end of bedtime (6am by default), the fan will turn off, and in the event of the fan turning on due to temperature, the fan will also turn off after the room temperature drops to a defined floating-point variable (69.0 by default).

Components used:
- Inland ESP32-WROOM-32D Module                                                                                                    https://www.microcenter.com/product/613822/ESP32-WROOM-32D_Module?storeID=145
- 0.96 Inch OLED I2C IIC Display Module 12864 128x64 Pixel SSD1306 Mini Self-Luminous OLED Screen Board                            https://www.amazon.com/gp/product/B09C5K91H7/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&th=1
- HiLetgo 2pcs 5V One Channel Relay Module Relay Switch with OPTO Isolation High Low Level Trigger                                 https://www.amazon.com/dp/B00LW15A4W?ref=ppx_yo2ov_dt_b_fed_asin_title
- HiLetgo 2pcs DHT22/AM2302 Digital Temperature And Humidity Sensor Module Temperature Humidity Monitor Sensor                     https://www.amazon.com/dp/B0795F19W6?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
- Adafruit Industries Perma-Proto Half-sized Breadboard PCB                                                                        https://www.microcenter.com/product/504168/adafruit-industries-perma-proto-half-sized-breadboard-pcb-3-pack
- Honeywell Turbo Force 2 in 1 Tower Oscillating Fan Black                                                                         https://www.target.com/p/honeywell-turbo-force-2-in-1-tower-oscillating-fan-black/-/A-75574814#lnk=sametab
