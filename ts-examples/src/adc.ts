import * as adc from "adc";
import * as ledc from "ledc";

const INPUT_PIN_R = 1;
const INPUT_PIN_L = 2;

const LED_PIN_R = 45;
const LED_PIN_B = 47;

ledc.configureTimer(0, 1000);
ledc.configureChannel(0, LED_PIN_R, 0, 1023);

ledc.configureChannel(2, LED_PIN_B, 0, 1023);

const PIEZO_PIN = 18;

ledc.configureTimer(1, 1000);
ledc.configureChannel(1, PIEZO_PIN, 1, 512);

let power = 3;

setInterval(() => {
    const valueR = adc.read(INPUT_PIN_R);
    ledc.setDuty(0, Math.pow(1023 - valueR, power) / Math.pow(1023, power - 1));

    const valueL = adc.read(INPUT_PIN_L);
    ledc.setDuty(2, Math.pow(1023 - valueL, power) / Math.pow(1023, power - 1));

    console.log((1023 - valueR) * 5);
    try {
        ledc.setFrequency(1, (1023 - valueR) * 5);
    } catch (e) {
        console.log(e);
    }
}, 10);
