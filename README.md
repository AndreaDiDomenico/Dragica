# Dragica
Auto irrigation with Arduino nano and SIM900 module 

The code uses the the pin d7 and d8 of arduino for rx and tx, connected to the SIM900 module. The pin d9 is connected to "key" pin of the module SIM900, used to reset it, after reset it sends a negative pulse with a duration of 1 second.

You can set the number that can be used to enable/disable a pump or a valve. On a call, if the module recognize the number of the caller, it turn on the pumpe/valve, hung up the call and recall twice the caller. If the module receives a call before a time out, it turns off the valve/pump. If the timeout expires without a call, the module turns off the valve/pump and it waits for a new call of the set number.
