import { LED_WS2812B, SmartLed } from "smartled";

/**
 * This example blinks using a smart LED on pin 48.
 */

const LED_PIN = 48;

let strip = new SmartLed({
    pin: LED_PIN,
    count: 1,
    type: LED_WS2812B
});

let buffer = new ArrayBuffer(4);
let view = new Uint32Array(buffer);

let state = false;

setInterval(() => {
    if (state) {
        view[0] = 0x000000;
    }
    else {
        // BRG
        view[0] = 0x002000;
    }
    strip.send(buffer);

    state = !state;
}, 1000);
