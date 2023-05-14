import { stdout } from "stdio";
import { readline } from "./readline.js";

stdout.write("Input: ");

let rl = new readline(true);
rl.read().then((line) => {
    stdout.write("You typed: " + line + "\n");
    rl.close();
    exit(0);
})
.catch((reason) => {
    stdout.write("Error: " + reason + "\n");
});
