import * as fs from "fs";

console.log("start");

let file = fs.open("test.txt", "w");
file.write("Hello, World\n");
file.close();

console.log("file written")

file = fs.open("test.txt", "r");
console.log("\"" + file.read(64) + "\"");
file.close();

console.log(fs.exists("test.txt"));

console.log("done");
