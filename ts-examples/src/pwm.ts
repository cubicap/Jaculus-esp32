import { PWM } from "embedded:io/pwm";

/**
 * Example showing how to use the LEDC to control the brightness of an LED.
 */

const LED_PIN = 45;

const ledPWM = new PWM({
    pin: LED_PIN,
    hz: 1000,
    resolution: 10
});

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
    ledPWM.write(Math.pow(duty, power) / Math.pow(1023, power - 1));
}, 10);
