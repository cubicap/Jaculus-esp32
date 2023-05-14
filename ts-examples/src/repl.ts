import { readline } from "./readline.js";
import { stderr, stdout } from "stdio";

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
