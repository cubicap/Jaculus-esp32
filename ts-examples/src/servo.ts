import { Analog } from "embedded:io/analog";
import * as ledc from "ledc";

/**
 * Example showing how to control servos using the LEDC peripheral.
 */

const INPUT_PIN = 1;
const SERVO_PIN = 17;

ledc.configureTimer(0, 50, 12);
ledc.configureChannel(0, SERVO_PIN, 0, 1023);

const adc = new Analog({
    pin: INPUT_PIN,
    resolution: 10
});

setInterval(() => {
    const value = adc.read();

    // map the value from 0-1023 to 1-2ms
    const ms = (value / 1023) + 1;

    // convert to a duty cycle
    const duty = (ms / 20) * 1023;

    ledc.setDuty(0, duty);
}, 20);
