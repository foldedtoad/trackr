# Eddystone nRF5x Beacon 

This is a Eddystone implementation for the Nordic SemiConductor's nRF5x chipset. This is not intended as a production ready, nor the most performant implementation. It's meant as an example for other people quickly wanting to play with an Eddystone beacon.

## Eddystone, what is it?

Eddystone is a protocol specification that defines a Bluetooth low energy (BLE) message format for proximity beacon messages. It describes several different frame types that may be used individually or in combinations to create beacons that can be used for a variety of applications.

Find out more at [https://github.com/google/eddystone]

## Implementation

This implementation currently only works with the TrackR beacon with the S110 softdriver. The make file is for use with the gcc compiler.

This example build against version 7.2.0 of the nRF SDK. With the SDK, gcc and segger's JLink installed (I'll refer you to [https://www.nordicsemi.com/] for tutorials how to get started with the nRF SDK) this work to get a working beacon:

This beacon uses a fixed UID, but the URL is updatable via, for example, the Nordic's Android Master Control Panel app. 

The next section assmumes you have an Android system and have the Nordic Master Control Panel (MCP) installed on it.  

When the TrackR device is powered on, ie. inserting battery, there is a 20 second window during which the device advertises as "connectable".  This allows you to connect to the TrackR with the MCP.  

Once the service and characteristics have resolved, you will see an "Unknown Characteristic" which is the Trackr Service's URL characteristic. Click on the up-arrow (on the right) to pop up the data entry box.  Select the "Text" Type and entery a new shortened URL. You must use a URL shortener utility, such as "Google URL Shortener" (https://goo.gl), to generate a compact form of you URL.  This is due to the limited space (17 bytes) in the Eddystone URL advertise format.

