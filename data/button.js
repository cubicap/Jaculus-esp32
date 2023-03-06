import { print } from 'stdio'
import { pinMode, pullMode, read, write, OUTPUT, INPUT, PULL_UP } from 'gpio';

const LED_PIN = 2;
const BUTTON_PIN = 17;

pinMode(LED_PIN, OUTPUT);
pinMode(BUTTON_PIN, INPUT);
pullMode(BUTTON_PIN, PULL_UP);

createTimer(100, () => {
    write(LED_PIN, read(BUTTON_PIN));
    print(read(BUTTON_PIN));
}, true);
