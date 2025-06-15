import * as httpClient from 'httpClient';
import { stdout } from 'stdio'; // For logging
import { waitForIp, currentIp } from 'wifi'; // For logging


async function runHttpExamples() {
    console.error("Waiting for IP address before exiting...");
    waitForIp();
    console.error("Connected IP:" + currentIp());

    stdout.write("Running httpClient examples...\n");

    while (true) {
        const response_hdl = httpClient.get("http://httpbin.org/get");

        for (let i = 0; i < 3; i++) {  // Reduced from 10 to 3
            stdout.write(`Waiting for response... (${i + 1}/3)\n`);
            await sleep(500);  // Increased from 100ms to 500ms
        }

        const response = await response_hdl;

        console.log("Status:" + response.status);
        console.log("Body length:" + response.body.length + " chars");  // Just length instead of full body
        // console.log("Body:" + response.body);  // Comment out large output

        await sleep(3000);  // Increased from 1000ms to 3000ms
    }
}

// Run the examples
runHttpExamples().catch(e => {
    stdout.write(`Unhandled global error in examples: ${e.message}\n`);
    exit(1);
});
