import { stdout } from "stdio";
import { readline } from "./readline.js";

/**
 * Example showing the use of standard input and output through the readline script.
 */

stdout.write("What's your name? ");

let rl = new readline(false);
rl.read().then((line) => {
    stdout.write("Hi " + line + "!\n");
    rl.close();
    exit(0);
})
.catch((reason) => {
    stdout.write("Error: " + reason + "\n");
});
