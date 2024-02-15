import { SmartLed, Rgb, LED_WS2812 } from "smartled";
import { Digital } from "embedded:io/digital";

/**
 * A simple Gomoku game.
 * Implemented for the Logic 1.1 with ESP32, and the Logic 2.0 with ESP32-S3.
 * (https://github.com/RoboticsBrno/RB3205-Logic)
 */

console.log("Platform: " + PlatformInfo.name);

if (PlatformInfo.name == "ESP32") {
    var LED_PIN = 23;
    var POWER_PIN = 16;

    var UP_PIN = 14;
    var DOWN_PIN = 26;
    var LEFT_PIN = 32;
    var RIGHT_PIN = 13;
    var MIDDLE_PIN = 17;
}
else if (PlatformInfo.name == "ESP32-S3") {
    var LED_PIN = 45;
    var POWER_PIN = 0;

    var UP_PIN = 8;
    var DOWN_PIN = 14;
    var LEFT_PIN = 10;
    var RIGHT_PIN = 4;
    var MIDDLE_PIN = 9;
}

let strip = new SmartLed({
    pin: LED_PIN,
    count: 100,
    type: LED_WS2812
});

let buffer = new ArrayBuffer(400);
let view = new Uint32Array(buffer);

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

function set(x: number, y: number, color: Rgb, brightness: number = 0.2) {
    brightness /= 4;
    view[x + y * 10] =
        (Math.floor(color.b * brightness) << 16) |
        (Math.floor(color.r * brightness) << 8) |
        (Math.floor(color.g * brightness));
}

let colors: Rgb[] = [
    { r: 0, g: 0, b: 0 },
    { r: 255, g: 0, b: 0 },
    { r: 0, g: 0, b: 255 }
];

set(0, 0, { r: 0, g: 255, b: 0 });
strip.send(buffer);

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
    strip.send(buffer);
    pos = newPos;
}


let power = new Digital({ pin: POWER_PIN, mode: Digital.Output });
power.write(1);

let up = new Digital({
    pin: UP_PIN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        update(pos, { x: pos.x, y: Math.max(0, pos.y - 1) });
    }
});

let down = new Digital({
    pin: DOWN_PIN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        update(pos, { x: pos.x, y: Math.min(9, pos.y + 1) });
    }
});

let left = new Digital({
    pin: LEFT_PIN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        update(pos, { x: Math.max(0, pos.x - 1), y: pos.y });
    }
});

let right = new Digital({
    pin: RIGHT_PIN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        update(pos, { x: Math.min(9, pos.x + 1), y: pos.y });
    }
});

let middle = new Digital({
    pin: MIDDLE_PIN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
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
                middle.close();
                up.close();
                down.close();
                left.close();
                right.close();
            }
            turnRed = !turnRed;
            strip.send(buffer);
        }
    }
});
