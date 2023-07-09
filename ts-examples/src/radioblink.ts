import * as simpleradio from "simpleradio";
import * as gpio from "gpio";

import { stdout, stderr } from "stdio";


/**
 * Example showing the use of simple radio to control LEDs.
 */


simpleradio.begin(1);


const BUTTON_A = 18;
const BUTTON_B = 16;
const BUTTON_C = 42;

const LED_GREEN = 17;
const LED_YELLOW = 15;
const LED_RED = 45;


gpio.pinMode(BUTTON_A, gpio.PinMode.INPUT);
gpio.pinMode(BUTTON_B, gpio.PinMode.INPUT);
gpio.pinMode(BUTTON_C, gpio.PinMode.INPUT);

gpio.pinMode(LED_GREEN, gpio.PinMode.OUTPUT);
gpio.pinMode(LED_YELLOW, gpio.PinMode.OUTPUT);
gpio.pinMode(LED_RED, gpio.PinMode.OUTPUT);


gpio.on("falling", BUTTON_A, () => {
    simpleradio.sendKeyValue("green", 1);
});
gpio.on("rising", BUTTON_A, () => {
    simpleradio.sendKeyValue("green", 0);
});

gpio.on("falling", BUTTON_B, () => {
    simpleradio.sendKeyValue("yellow", 1);
});
gpio.on("rising", BUTTON_B, () => {
    simpleradio.sendKeyValue("yellow", 0);
});

gpio.on("falling", BUTTON_C, () => {
    simpleradio.sendKeyValue("red", 1);
});
gpio.on("rising", BUTTON_C, () => {
    simpleradio.sendKeyValue("red", 0);
});


simpleradio.on("keyvalue", (key, value) => {
    stdout.write("Received key-value: " + key + "=" + value + "\n");
    switch (key) {
        case "green":
            gpio.write(LED_GREEN, value);
            break;
        case "yellow":
            gpio.write(LED_YELLOW, value);
            break;
        case "red":
            gpio.write(LED_RED, value);
            break;
    }
});
