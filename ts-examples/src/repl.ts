import { readline } from "./readline.js";
import { stderr, stdout } from "stdio";

/**
 * An incomplete REPL implementation.
 * - does not support control characters (e.g., backspace, arrow keys)
 * - stops on exceptions thrown by scheduled events (limitation of the runtime)
 */

let rl = new readline(true);

function prompt() {
    stdout.write("> ");
    rl.read().then((line) => {
        let res = eval(line);
        stdout.write(res + "\n");
        prompt();
    })
    .catch((reason) => {
        stderr.write("Uncaught " + reason + "\n");
        prompt();
    });
}

prompt();
