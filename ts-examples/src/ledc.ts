import * as ledc from "ledc";

/**
 * Example showing how to use the LEDC to control the brightness of an LED.
 */

const LED_PIN = 45;

ledc.configureTimer(0, 1000);
ledc.configureChannel(0, LED_PIN, 0, 1023);

let duty = 0;
let step = 10;

let power = 3;

setInterval(() => {
    duty += step;
    if (duty >= 1023) {
        duty = 1023;
        step *= -1;
    }
    if (duty <= 0) {
        duty = 0;
        step *= -1;
    }
    ledc.setDuty(0, Math.pow(duty, power) / Math.pow(1023, power - 1));
}, 10);
