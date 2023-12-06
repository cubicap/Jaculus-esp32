import * as simpleradio from "simpleradio";
import { Digital } from "embedded:io/digital";

import { stdout } from "stdio";


/**
 * Example showing the use of simple radio to control LEDs.
 */


simpleradio.begin(1);


const BUTTON_A = 18;
const BUTTON_B = 16;
const BUTTON_C = 42;

const LED_GREEN = 17;
const LED_YELLOW = 15;
const LED_RED = 45;

let ledGreen = new Digital({
    pin: LED_GREEN,
    mode: Digital.Output,
});

let ledYellow = new Digital({
    pin: LED_YELLOW,
    mode: Digital.Output,
});

let ledRed = new Digital({
    pin: LED_RED,
    mode: Digital.Output,
});


let buttonA = new Digital({
    pin: BUTTON_A,
    mode: Digital.InputPullUp,
    edge: Digital.Falling + Digital.Rising,
    onReadable: ( args ) => {
        simpleradio.sendKeyValue("green", args.edge === Digital.Falling ? 1 : 0);
    }
});

let buttonB = new Digital({
    pin: BUTTON_B,
    mode: Digital.InputPullUp,
    edge: Digital.Falling + Digital.Rising,
    onReadable: ( args ) => {
        simpleradio.sendKeyValue("yellow", args.edge === Digital.Falling ? 1 : 0);
    }
});

let buttonC = new Digital({
    pin: BUTTON_C,
    mode: Digital.InputPullUp,
    edge: Digital.Falling + Digital.Rising,
    onReadable: ( args ) => {
        simpleradio.sendKeyValue("red", args.edge === Digital.Falling ? 1 : 0);
    }
});


simpleradio.on("keyvalue", (key, value) => {
    stdout.write("Received key-value: " + key + "=" + value + "\n");
    switch (key) {
        case "green":
            ledGreen.write(value);
            break;
        case "yellow":
            ledYellow.write(value);
            break;
        case "red":
            ledRed.write(value);
            break;
    }
});


// hack to prevent the object from being garbage collected
// TODO: remove when fixed
setInterval(() => {
    buttonA;
    buttonB;
    buttonC;
}, 100000);
