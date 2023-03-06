import { pinMode, write, OUTPUT } from 'gpio';

const LED_PIN = 2;

pinMode(LED_PIN, OUTPUT);

let ledState = false;

createTimer(500, () => {
    write(LED_PIN, ledState ? 1 : 0);
    ledState = !ledState;
}, true);
