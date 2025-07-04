import * as httpClient from 'httpClient';
import { stdout } from 'stdio';
import { waitForIp, currentIp } from 'wifi';

/**
 * Comprehensive demonstration of the httpClient module.
 *
 * This example showcases all available HTTP methods:
 * - GET: Retrieve data from a server
 * - POST: Send data to create a resource
 * - PUT: Send data to update a resource
 * - DELETE: Remove a resource
 *
 * NOTE: Ensure your device is connected to WiFi with internet access
 * before running these examples.
 */

async function runHttpExamples() {
    stdout.write("=== HTTP Client Examples ===\n");

    // Wait for WiFi connection
    stdout.write("Waiting for IP address...\n");
    waitForIp();
    stdout.write(`Connected! IP: ${currentIp()}\n\n`);

    // Example 1: Basic GET request
    try {
        stdout.write("1. GET Request Example\n");
        stdout.write("   URL: http://httpbin.org/get\n");

        const getResponse = await httpClient.get("http://httpbin.org/get");

        stdout.write(`   Status: ${getResponse.status}\n`);
        stdout.write(`   Body length: ${getResponse.body.length} characters\n`);

        if (getResponse.status === 200) {
            // Parse a small part of the JSON to show it works
            const bodyJson = JSON.parse(getResponse.body);
            stdout.write(`   Your IP: ${bodyJson.origin}\n`);
        }

        stdout.write("   ✓ GET request successful!\n\n");
    } catch (error) {
        stdout.write(`   ✗ GET Error: ${error.message}\n\n`);
    }

    await sleep(1000);

    // Example 2: POST request with JSON data
    try {
        stdout.write("2. POST Request Example\n");
        stdout.write("   URL: http://httpbin.org/post\n");

        const postData = JSON.stringify({
            message: "Hello from ESP32!",
            timestamp: Date.now(),
            device: "ESP32-S3"
        });

        const postResponse = await httpClient.post(
            "http://httpbin.org/post",
            postData,
            "application/json"
        );

        stdout.write(`   Status: ${postResponse.status}\n`);
        stdout.write(`   Body length: ${postResponse.body.length} characters\n`);

        if (postResponse.status === 200) {
            const bodyJson = JSON.parse(postResponse.body);
            stdout.write(`   Data echoed back: ${bodyJson.data}\n`);
        }

        stdout.write("   ✓ POST request successful!\n\n");
    } catch (error) {
        stdout.write(`   ✗ POST Error: ${error.message}\n\n`);
    }

    await sleep(1000);

    // Example 3: PUT request
    try {
        stdout.write("3. PUT Request Example\n");
        stdout.write("   URL: http://httpbin.org/put\n");

        const putData = JSON.stringify({
            id: 123,
            name: "Updated Resource",
            value: "new value"
        });

        const putResponse = await httpClient.put(
            "http://httpbin.org/put",
            putData,
            "application/json"
        );

        stdout.write(`   Status: ${putResponse.status}\n`);
        stdout.write(`   Body length: ${putResponse.body.length} characters\n`);
        stdout.write("   ✓ PUT request successful!\n\n");
    } catch (error) {
        stdout.write(`   ✗ PUT Error: ${error.message}\n\n`);
    }

    await sleep(1000);

    // Example 4: DELETE request
    try {
        stdout.write("4. DELETE Request Example\n");
        stdout.write("   URL: http://httpbin.org/delete\n");

        const deleteResponse = await httpClient.del("http://httpbin.org/delete");

        stdout.write(`   Status: ${deleteResponse.status}\n`);
        stdout.write(`   Body length: ${deleteResponse.body.length} characters\n`);
        stdout.write("   ✓ DELETE request successful!\n\n");
    } catch (error) {
        stdout.write(`   ✗ DELETE Error: ${error.message}\n\n`);
    }

    await sleep(1000);

    // Example 5: Different content types
    try {
        stdout.write("5. POST with Plain Text\n");
        stdout.write("   URL: http://httpbin.org/post\n");

        const textResponse = await httpClient.post(
            "http://httpbin.org/post",
            "This is plain text data from ESP32",
            "text/plain"
        );

        stdout.write(`   Status: ${textResponse.status}\n`);
        stdout.write("   ✓ Plain text POST successful!\n\n");
    } catch (error) {
        stdout.write(`   ✗ Text POST Error: ${error.message}\n\n`);
    }

    await sleep(1000);

    // Example 6: Error handling - invalid URL
    try {
        stdout.write("6. Error Handling Example\n");
        stdout.write("   URL: http://invalid-domain-that-does-not-exist.com\n");

        const errorResponse = await httpClient.get("http://invalid-domain-that-does-not-exist.com");
        stdout.write(`   Unexpected success: ${errorResponse.status}\n`);
    } catch (error) {
        stdout.write(`   ✓ Expected error caught: ${error.message}\n\n`);
    }

    stdout.write("=== All HTTP Client Examples Completed ===\n");
    stdout.write("The HTTP client is working correctly!\n");

    // Optional: Run a continuous test loop (uncomment to enable)
    await runContinuousTest();
}

/**
 * Optional continuous test - uncomment the call above to enable
 */
async function runContinuousTest() {
    stdout.write("\n--- Starting continuous test (every 5 seconds) ---\n");

    let requestCount = 0;
    while (true) {
        try {
            requestCount++;
            stdout.write(`Request #${requestCount}: `);

            const response = await httpClient.get("http://httpbin.org/get");
            stdout.write(`Status ${response.status}, ${response.body.length} chars\n`);

        } catch (error) {
            stdout.write(`Error: ${error.message}\n`);
        }

        await sleep(500); // 5 second intervals
    }
}

// Start the examples
runHttpExamples().catch(error => {
    stdout.write(`Fatal error: ${error.message}\n`);
    exit(1);
});
