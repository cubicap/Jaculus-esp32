import * as gpio from "gpio";

const LED_PIN = 45;

gpio.pinMode(LED_PIN, gpio.PinMode.OUTPUT);

let state = false;

setInterval(() => {
    gpio.write(LED_PIN, state ? 1 : 0);
    state = !state;
}, 1000);
