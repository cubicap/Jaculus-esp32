import * as simpleradio from "simpleradio";

import { stdout, stderr } from "stdio";
import { readline } from "./readline.js";

/**
 * Example showing the use of simple radio to send and receive strings.
 */

simpleradio.begin(1);

console.log(`Current group: ${simpleradio.group()}, address: ${simpleradio.address()}`)

stdout.write("Input: ");

let rl = new readline(true);
function prompt() {
    stdout.write("> ");
    rl.read().then((line) => {
        simpleradio.sendString(line);
        prompt();
    })
    .catch((reason) => {
        stderr.write("Uncaught " + reason + "\n");
        prompt();
    });
}

prompt();

simpleradio.on("number", (num) => {
    stdout.write("Received number: " + num + "\n");
});

simpleradio.on("string", (str) => {
    stdout.write("Received string: " + str + "\n");
});

simpleradio.on("keyvalue", (key, value) => {
    stdout.write("Received key-value: " + key + "=" + value + "\n");
});
