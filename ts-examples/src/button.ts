import * as gpio from 'gpio';


const BUTTON_PIN = 12;

gpio.pinMode(BUTTON_PIN, gpio.PinMode.INPUT_PULLUP);

gpio.on("falling", BUTTON_PIN, ({ timestamp }) => {
    console.log(`Button pressed at ${timestamp.millis()}.${timestamp.micros()}`);
});
