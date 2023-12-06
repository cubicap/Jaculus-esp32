import { SmartLed, Rgb, LED_WS2812 } from "smartled";
import { Digital } from "embedded:io/digital";

/**
 * A simple Snake game.
 * Implemented for the Logic 1.1 with ESP32, and the Logic 2.0 with ESP32-S3.
 * (https://github.com/RoboticsBrno/RB3205-Logic)
 */

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

const FOOD_COLOR = { r: 255, g: 0, b: 0 };
const SNAKE_COLOR = { r: 0, g: 255, b: 0 };

let power = new Digital({ pin: POWER_PIN, mode: Digital.Output });
power.write(1);

let strip = new SmartLed(LED_PIN, 100, LED_WS2812);
function set(x: number, y: number, color: Rgb, brightness: number = 0.2) {
    strip.set(x + y * 10, { r: color.r * brightness, g: color.g * brightness, b: color.b * brightness });
}

let snake = [
    { x: 4, y: 4 },
    { x: 3, y: 4 },
    { x: 2, y: 4 },
    { x: 1, y: 4 },
    { x: 0, y: 4 }
];
for (let pos of snake) {
    set(pos.x, pos.y, SNAKE_COLOR);
}
let direction = { x: 1, y: 0 };

function newFood() {
    let x = Math.floor(Math.random() * 10);
    let y = Math.floor(Math.random() * 10);
    if (snake.find((pos) => pos.x == x && pos.y == y)) {
        return newFood();
    }
    set(x, y, FOOD_COLOR);
    return { x, y };
}
let food = newFood();
strip.show();

let score = 0;

let up = new Digital({
    pin: UP,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        direction = { x: 0, y: -1 };
    }
});

let down = new Digital({
    pin: DOWN,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        direction = { x: 0, y: 1 };
    }
});

let left = new Digital({
    pin: LEFT,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        direction = { x: -1, y: 0 };
    }
});

let right = new Digital({
    pin: RIGHT,
    mode: Digital.InputPullUp,
    edge: Digital.Falling,
    onReadable: () => {
        direction = { x: 1, y: 0 };
    }
});

let blinkState = true;
function blinkSnake() {
    for (let pos of snake) {
        set(pos.x, pos.y, blinkState ? SNAKE_COLOR : { r: 0, g: 0, b: 0 });
    }
    strip.show();
    blinkState = !blinkState;
}

function step() {
    let next = { x: snake[0].x + direction.x, y: snake[0].y + direction.y };
    if (next.x < 0) {
        next.x = 9;
    }
    else if (next.x > 9) {
        next.x = 0;
    }
    if (next.y < 0) {
        next.y = 9;
    }
    else if (next.y > 9) {
        next.y = 0;
    }

    if (snake.find((pos) => pos.x == next.x && pos.y == next.y)) {
        clearInterval(timer);
        blinkSnake();
        setInterval(blinkSnake, 500);
        return;
    }

    snake.unshift(next);
    if (next.x == food.x && next.y == food.y) {
        score++;
        food = newFood();
        console.log("Score: " + score);
    }
    else {
        let last = snake.pop();
        set(last.x, last.y, { r: 0, g: 0, b: 0 });
    }
    set(next.x, next.y, SNAKE_COLOR);
    strip.show();
}

var timer = setInterval(step, 300);


// hack to prevent the object from being garbage collected
// TODO: remove when fixed
setInterval(() => {
    up;
    down;
    left;
    right;
    power;
    strip;
}, 100000);
