# Eddystone Beacon Hardware: the TrackR device 

This document shows how to open and connect wires to the TrackR so as to allow it to be reprogrammed with the Eddystone firmware.

## TrackR Hardware Components
The TrackR device is shown [here](https://github.com/foldedtoad/trackr/blob/master/images/trackr_whole_device.jpg).

The TrackR case can be opened by sliding an X-acto knife blade between the metal case and the plastic core section.  The case is held together by double-sided tape and with a moderate amount of care the two metal sandwich leaves can be pryed up.
Go slow and the tape will give: be sure not to bend the metal in the process.  The hinge is especially suseptable to metal fatigue. Clean off the tape sticky residue with alcohol.

When opened, the parts are basically three parts: metal case, plastic core with PCB and battery well, and the plastic core cover which has the buzzer.  [Photo](https://github.com/foldedtoad/trackr/blob/master/images/trackr_all_parts.jpg) 

The PCB board can be pryed from the plastic core shell.  The various parts on the PCB are shown [here](https://github.com/foldedtoad/trackr/blob/master/images/trackr_board_front.jpg)

The back side of the PCB has several "pogo-pin" pads, which are shown [here](https://github.com/foldedtoad/trackr/blob/master/images/trackr_board_back.jpg)
Note that the SWD lines are available, and will be used to reprogram the TrackR with new Eddystone firmware.

To connect the SWD wires, a custom cable was constructed which connects the pogo-pin pads to a suitable SWD connector.
See this [photo](https://github.com/foldedtoad/trackr/blob/master/images/TrackR_SWD_connection_overview.JPG) for overview, and this [photo](https://github.com/foldedtoad/trackr/blob/master/images/TrackR_SWD_connection_close_up.JPG) for details.

If you wish to also establish a serial port connection, for debug logging, you can connect the two buzzer line and reperpose then for UART usage. See this [photo](https://github.com/foldedtoad/trackr/blob/master/images/trackr_with_console_uart.JPG) for details.
