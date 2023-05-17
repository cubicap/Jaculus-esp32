import * as ledc from "ledc";

/**
 * Example showing how to use the LEDC to control the frequency of a piezo.
 */

const PIEZO_PIN = 18;

ledc.configureTimer(0, 1000);
ledc.configureChannel(0, PIEZO_PIN, 0, 512);

let frequency = 1000;
let step = 10;

setInterval(() => {
    frequency += step;
    if (frequency >= 2000) {
        frequency = 2000;
        step *= -1;
    }
    if (frequency <= 1000) {
        frequency = 1000;
        step *= -1;
    }
    ledc.setFrequency(0, frequency);
}, 10);
