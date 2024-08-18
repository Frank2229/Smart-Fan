# Smart-Fan
An automated fan that turns on and off on its own.
This device is an add-on to an existing fan by severing the hot wire and connecting each severed end to a relay.
The fan will turn on if the room temperature exceeds a defined floating-point variable (71.0 by default) or if the current time passes a user defined start of bedtime (sleep with fan on; 10pm by default).
After the current time passes end of bedtime (6am by default), the fan will turn off, and in the event of the fan turning on due to temperature, the fan will also turn off after the room temperature drops to a defined floating-point variable (69 by default).
