import * as adc from "adc";
import * as ledc from "ledc";

/**
 * Example showing how to use the ADC to control the brightness of an LED.
 */

const INPUT_PIN = 1;
const LED_PIN = 45;

ledc.configureTimer(0, 1000);
ledc.configureChannel(0, LED_PIN, 0, 1023);

adc.configure(INPUT_PIN);

let power = 3;

setInterval(() => {
    const value = adc.read(INPUT_PIN);
    ledc.setDuty(0, Math.pow(value, power) / Math.pow(1023, power - 1));
}, 10);
