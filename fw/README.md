# Eddystone Beacon Firmware on the TrackR Hardware 

This is a Eddystone implementation for the Nordic SemiConductor's nRF5x chipset. This is not intended as a production ready, nor the most performant implementation. It "reperposes" the TrackR device's hardware to turn it into a Eddystone beacon device.

## Eddystone, what is it?

Eddystone is a protocol specification that defines a Bluetooth low energy (BLE) message format for proximity beacon messages. It describes several different frame types that may be used individually or in combinations to create beacons that can be used for a variety of applications.

Find out more at [https://github.com/google/eddystone]

## Implementation

This implementation currently only works with the TrackR beacon with the S110 softdriver. The make file is for use with the gcc compiler.

This example build against version 7.2.0 of the nRF SDK. With the SDK, gcc and segger's JLink installed (I'll refer you to [https://www.nordicsemi.com/] for tutorials how to get started with the nRF SDK) this work to get a working beacon:

This beacon uses a fixed UID, but the URL is updatable via. For example, the Nordic's Android Master Control Panel app can be use to update the URLs. 

Master Control Panel app: https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en

The next section assumes you have an Android system and have the Nordic Master Control Panel (MCP) installed on it.  

When the TrackR device is powered on, ie. inserting battery, there is a 20 second window during which the device advertises as connectable, but no name.  During this time the blue LED will be slowly flashing. This window allows you to connect to the TrackR with the MCP. When you successfully connect, the LED will turn solid blue.  NOTE: try to minimize you time connected, as leaving the LED on for extended periods of time will quickly deplete the battery.

Once connected and the service and characteristics have resolved, you will notice an "Unknown Characteristic": this is the Trackr Service's URL characteristic. Click on this line to expand the characteristic. Next click on the up-arrow (on the right) to pop up the data entry box.  Select the "Text" Type and entery a new shortened URL. You must use a URL shortener utility, such as "Google URL Shortener" (https://goo.gl), to generate a compact form of you URL.  This is due to the limited space (17 bytes) in the Eddystone URL advertise format.

If you do not attempt to connect to the TrackR device, and allow the 20 second timeout to accure, the the device will begin Eddystone beaconing.  In this case, the last update URL (or if never update, the built-in URL) will be broadcast. 

From an Android system, you can use the Google "Physical Web" app to listen for Eddystone beacons. After as few seconds, the Physical Web app should display an entry for your URL.  Click on it will take you to that URL's site.

Physical Web app:  https://play.google.com/store/apps/details?id=physical_web.org.physicalweb&hl=en





