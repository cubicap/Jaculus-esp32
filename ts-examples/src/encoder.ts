import { PulseCounter, LevelAction, EdgeAction } from "pulseCounter";
import * as gpio from "gpio";


const LEVEL_PIN = 12;
const EDGE_PIN = 13;
const BUTTON_PIN = 7;


const encoder = new PulseCounter({
    pinLevel: LEVEL_PIN,
    pinEdge: EDGE_PIN,
    levelMode: {
        low: LevelAction.Inverse,
        high: LevelAction.Keep,
    },
    edgeMode: {
        pos: EdgeAction.Increase,
        neg: EdgeAction.Decrease,
    },
});


setInterval(() => {
    try {
        console.log("pos: " + encoder.read());
    } catch (e) {
        console.error(e);
    }
}, 1);

gpio.pinMode(BUTTON_PIN, gpio.PinMode.INPUT_PULLUP);
gpio.on("falling", BUTTON_PIN, () => {
    console.log("reset");
    encoder.clear();
});
