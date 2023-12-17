import { Analog } from "embedded:io/analog";
import { PWM } from "embedded:io/pwm";

/**
 * Example showing how to use the ADC to control the brightness of an LED.
 */

const INPUT_PIN = 1;
const LED_PIN = 45;

const ledPWM = new PWM({
    pin: LED_PIN,
    hz: 1000,
    resolution: 10
});

const adc = new Analog({
    pin: INPUT_PIN,
    resolution: 10
});

let power = 3;

setInterval(() => {
    const value = adc.read();
    ledPWM.write(Math.pow(value, power) / Math.pow(1023, power - 1));
}, 10);
