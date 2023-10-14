import { SmartLed } from "smartled";

/**
 * This example blinks using a smart LED on pin 48.
 */

const LED_PIN = 48;

let strip = new SmartLed(LED_PIN, 1);

let state = false;

setInterval(() => {
    if (state) {
        strip.set(0, { r: 0, g: 0, b: 0 });
    }
    else {
        strip.set(0, { r: 20, g: 0, b: 0 });
    }
    strip.show();

    state = !state;
}, 1000);
