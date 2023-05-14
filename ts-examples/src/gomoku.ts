import { Neopixel, Rgb } from "neopixel";
import * as gpio from "gpio";


console.log("Platform: " + PlatformInfo.name);

if (PlatformInfo.name == "ESP32") {
    var LED_PIN = 23;
    var POWER_PIN = 16;

    var UP = 14;
    var DOWN = 26;
    var LEFT = 32;
    var RIGHT = 13;
    var MIDDLE = 17;
}
else if (PlatformInfo.name == "ESP32-S3") {
    var LED_PIN = 45;
    var POWER_PIN = 0;

    var UP = 8;
    var DOWN = 14;
    var LEFT = 10;
    var RIGHT = 4;
    var MIDDLE = 9;
}

gpio.pinMode(POWER_PIN, gpio.PinMode.OUTPUT);
gpio.write(POWER_PIN, 1);

for (let pin of [UP, DOWN, LEFT, RIGHT, MIDDLE]) {
    gpio.pinMode(pin, gpio.PinMode.INPUT_PULLUP);
}

let strip = new Neopixel(LED_PIN, 100);

interface Pos {
    x: number;
    y: number;
}

let pos: Pos = { x: 0, y: 0 };

let matrix: number[][] = [];
for (let i = 0; i < 10; i++) {
    matrix[i] = [];
    for (let j = 0; j < 10; j++) {
        matrix[i][j] = 0;
    }
}

function set(x: number, y: number, color: Rgb, brightness: number = 0.1) {
    brightness /= 4;
    strip.set(x + y * 10, { r: color.r * brightness, g: color.g * brightness, b: color.b * brightness });
}

let colors: Rgb[] = [
    { r: 0, g: 0, b: 0 },
    { r: 255, g: 0, b: 0 },
    { r: 0, g: 0, b: 255 }
];

set(0, 0, { r: 0, g: 255, b: 0 });
strip.show();

let turnRed = true;

function gameEnd(pos: Pos): { color: number, direction: number[], posCount: number, negCount: number } | null {
    let color = matrix[pos.x][pos.y];

    for (let direction of [[1, 0], [0, 1], [1, 1], [-1, 1]]) {
        let posCount = 0;
        let negCount = 0;
        let newPos = { x: pos.x, y: pos.y };
        let reverse = false;
        while (true) {
            newPos.x += direction[0];
            newPos.y += direction[1];
            if (newPos.x < 0 || newPos.x >= 10 || newPos.y < 0 || newPos.y >= 10
                || matrix[newPos.x][newPos.y] !== color) {
                if (reverse) {
                    break;
                }
                reverse = true;
                newPos = { x: pos.x, y: pos.y };
                direction[0] *= -1;
                direction[1] *= -1;
                continue;
            }
            if (reverse) {
                negCount++;
            }
            else {
                posCount++;
            }
        }
        if (posCount + negCount == 4) {
            return {
                color: color,
                direction: [direction[0] * -1, direction[1] * -1],
                posCount: posCount,
                negCount: negCount
            }
        }
    }
    return null;
}

function update(oldPos: Pos, newPos: Pos) {
    set(oldPos.x, oldPos.y, colors[matrix[oldPos.x][oldPos.y]]);
    set(newPos.x, newPos.y, { r: 0, g: 255, b: 0 });
    strip.show();
    pos = newPos;
}

gpio.on("falling", UP, () => {
    update(pos, { x: pos.x, y: Math.max(0, pos.y - 1) });
});

gpio.on("falling", DOWN, () => {
    update(pos, { x: pos.x, y: Math.min(9, pos.y + 1) });
});

gpio.on("falling", LEFT, () => {
    update(pos, { x: Math.max(0, pos.x - 1), y: pos.y });
});

gpio.on("falling", RIGHT, () => {
    update(pos, { x: Math.min(9, pos.x + 1), y: pos.y });
});

gpio.on("falling", MIDDLE, () => {
    if (matrix[pos.x][pos.y] === 0) {
        matrix[pos.x][pos.y] = turnRed ? 1 : 2;
        set(pos.x, pos.y, colors[matrix[pos.x][pos.y]]);

        let end = gameEnd(pos);
        if (end) {
            for (let i = 1; i < end.posCount + 1; i++) {
                set(pos.x + i * end.direction[0], pos.y + i * end.direction[1], colors[end.color], 0.5);
            }
            for (let i = 1; i < end.negCount + 1; i++) {
                set(pos.x - i * end.direction[0], pos.y - i * end.direction[1], colors[end.color], 0.5);
            }
            set(pos.x, pos.y, colors[end.color], 0.5);
            gpio.off("falling", MIDDLE);
            gpio.off("falling", UP);
            gpio.off("falling", DOWN);
            gpio.off("falling", LEFT);
            gpio.off("falling", RIGHT);
        }
        turnRed = !turnRed;
        strip.show();
    }
});
