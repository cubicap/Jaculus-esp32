import { Analog } from "embedded:io/analog";
import { PWM } from "embedded:io/pwm";

/**
 * Example showing how to control servos using the LEDC peripheral.
 */

const INPUT_PIN = 1;
const SERVO_PIN = 35;

const servoPWM = new PWM({
    pin: SERVO_PIN,
    hz: 50,
    resolution: 10
});

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

    servoPWM.write(duty);
}, 5);
