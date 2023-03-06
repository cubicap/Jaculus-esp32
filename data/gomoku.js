import { Neopixel } from 'neopixel'
import { pinMode, pullMode, read, write, OUTPUT, INPUT, PULL_UP } from 'gpio';

const POWER_PIN = 16;

pinMode(POWER_PIN, OUTPUT);
write(POWER_PIN, 1);

const UP = 14;
const DOWN = 26;
const LEFT = 32;
const RIGHT = 13;
const MIDDLE = 17;

for (let pin of [UP, DOWN, LEFT, RIGHT, MIDDLE]) {
    pinMode(pin, INPUT);
    pullMode(pin, PULL_UP);
}

let strip = new Neopixel(23, 100);

let pos = { x: 0, y: 0 };

let matrix = [];
for (let i = 0; i < 10; i++) {
    matrix[i] = [];
    for (let j = 0; j < 10; j++) {
        matrix[i][j] = 0;
    }
}

function set(x, y, color, brightness = 0.1) {
    strip.set(x + y * 10, { r: color.r * brightness, g: color.g * brightness, b: color.b * brightness });
}

let colors = [
    { r: 0, g: 0, b: 0 },
    { r: 255, g: 0, b: 0 },
    { r: 0, g: 0, b: 255 }
];

set(0, 0, { r: 0, g: 255, b: 0 });
strip.show();

let turnRed = true;
let ended = false;

function gameEnd(pos) {
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

createTimer(150, () => {
    if (ended) {
        return;
    }
    let newPos = { x: pos.x, y: pos.y };
    if (read(UP) === 0) {
        newPos.y = Math.max(0, newPos.y - 1);
    }
    if (read(DOWN) === 0) {
        newPos.y = Math.min(9, newPos.y + 1);
    }
    if (read(LEFT) === 0) {
        newPos.x = Math.max(0, newPos.x - 1);
    }
    if (read(RIGHT) === 0) {
        newPos.x = Math.min(9, newPos.x + 1);
    }

    if (newPos.x !== pos.x || newPos.y !== pos.y) {
        set(pos.x, pos.y, colors[matrix[pos.x][pos.y]]);
        pos = newPos;
        set(pos.x, pos.y, { r: 0, g: 255, b: 0 });
        strip.show();
    }

    if (read(MIDDLE) === 0 && matrix[pos.x][pos.y] === 0) {
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
            ended = true;
        }
        turnRed = !turnRed;
        strip.show();
    }
}, true);
