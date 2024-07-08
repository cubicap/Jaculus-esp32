import { Motor } from "motor";
import * as ledc from "ledc";

ledc.configureTimer(0, 64000, 10);

let left = new Motor({
    pins: {
        motA: 11,
        motB: 12,
        encA: 39,
        encB: 40
    },
    ledc: {
        timer: 0,
        channelA: 0,
        channelB: 1
    },
    encTicks: 406,  // ticks per revolution
    diameter: 34  // mm
});

let right = new Motor({
    pins: {
        motA: 45,
        motB: 13,
        encA: 42,
        encB: 41
    },
    ledc: {
        timer: 0,
        channelA: 2,
        channelB: 3
    },
    encTicks: 406,  // ticks per revolution
    diameter: 34  // mm
});

setInterval(() => {
    console.log(`left: ${left.getPosition()} right: ${right.getPosition()}`);
}, 1000);


left.setSpeed(100);  // 100 mm/s
right.setSpeed(100);

async function moveDistance() {
    let l = left.move({ distance: 200 });  // 200 mm
    let r = right.move({ distance: 200 });
    await Promise.all([l, r]);
}

async function moveTime() {
    left.setSpeed(-200);  // 200 mm/s backwards (for timed and infinite movement)
    let l = left.move({ time: 2000 });  // 2000 ms
    let r = right.move({ time: 2000 });
    await Promise.all([l, r]);
}

async function moveInfinite() {
    left.setSpeed(200);  // 200 mm/s
    right.setSpeed(-200);
    let l = left.move();
    let r = right.move();

    setTimeout(() => {
        left.stop(true);
        right.stop(true);
    }, 1500);

    await Promise.all([l, r]);  // resolves when stop is called
}

async function main() {
    await moveDistance();
    console.log("Distance done");

    await moveTime();
    console.log("Time done");

    await moveInfinite();
    console.log("Infinite done");
}
main();
