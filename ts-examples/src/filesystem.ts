import * as fs from "fs";

/**
 * Example of using the filesystem module.
 * Note that on the ESP32, functions involving directories may break the runtime.
 */

console.log("start");

let file = fs.open("test.txt", "w");
file.write("Hello, World\n");
file.close();

console.log("file written")

console.log("file exists: " + fs.exists("test.txt"));

file = fs.open("test.txt", "r");
console.log("\"" + file.read(64) + "\"");
file.close();


console.log("done");
