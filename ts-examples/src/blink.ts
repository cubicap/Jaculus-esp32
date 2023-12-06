import { Digital } from "embedded:io/digital";

/**
 * This example blinks an LED on pin 45.
 */

const LED_PIN = 45;

let led = new Digital({
    pin: LED_PIN,
    mode: Digital.Output,
});

let state = false;

setInterval(() => {
    led.write(state ? 1 : 0);
    state = !state;
}, 1000);
