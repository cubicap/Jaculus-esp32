import * as httpClient from 'httpClient';
import { stdout } from 'stdio'; // For logging

/**
 * Example demonstrating the httpClient module.
 *
 * NOTE: Ensure your device is connected to WiFi with internet access
 * before running these examples.
 */

const TEST_API_URL = "https://jsonplaceholder.typicode.com"; // A public test API

async function runHttpExamples() {
    stdout.write("Running httpClient examples...\n");

    while (true) {
        const response = httpClient.post("http://httpbin.org/post");
        console.log("Status:" + response.status);
        console.log("Body:" + response.body);

        await sleep(1000);
    }

    // // Example 1: GET request
    // try {
    //     stdout.write(`\n--- GET ${TEST_API_URL}/todos/1 ---\n`);
    //     const getResponse = await httpClient.get(`${TEST_API_URL}/todos/1`);
    //     stdout.write(`Status: ${getResponse.status}\n`);
    //     stdout.write(`Headers: ${JSON.stringify(getResponse.headers)}\n`);
    //     stdout.write(`Body: ${getResponse.body}\n`);
    // } catch (error) {
    //     stdout.write(`GET Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 2: POST request with JSON body
    // try {
    //     stdout.write(`\n--- POST ${TEST_API_URL}/posts ---\n`);
    //     const postData = {
    //         title: 'foo',
    //         body: 'bar',
    //         userId: 1,
    //     };
    //     const postResponse = await httpClient.post(`${TEST_API_URL}/posts`, postData, {
    //         headers: { 'Content-type': 'application/json; charset=UTF-8' }
    //     });
    //     stdout.write(`Status: ${postResponse.status}\n`);
    //     stdout.write(`Body: ${postResponse.body}\n`); // API should echo back the data with an ID
    // } catch (error) {
    //     stdout.write(`POST Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 3: PUT request
    // try {
    //     stdout.write(`\n--- PUT ${TEST_API_URL}/posts/1 ---\n`);
    //     const putData = {
    //         id: 1,
    //         title: 'updated foo',
    //         body: 'updated bar',
    //         userId: 1,
    //     };
    //     const putResponse = await httpClient.put(`${TEST_API_URL}/posts/1`, putData, {
    //         headers: { 'Content-type': 'application/json; charset=UTF-8' }
    //     });
    //     stdout.write(`Status: ${putResponse.status}\n`);
    //     stdout.write(`Body: ${putResponse.body}\n`);
    // } catch (error) {
    //     stdout.write(`PUT Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 4: DELETE request
    // try {
    //     stdout.write(`\n--- DELETE ${TEST_API_URL}/posts/1 ---\n`);
    //     const deleteResponse = await httpClient.del(`${TEST_API_URL}/posts/1`);
    //     stdout.write(`Status: ${deleteResponse.status}\n`); // Should be 200 or 204
    //     stdout.write(`Body: ${deleteResponse.body}\n`); // Body is often empty for DELETE
    // } catch (error) {
    //     stdout.write(`DELETE Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 5: HEAD request
    // try {
    //     stdout.write(`\n--- HEAD ${TEST_API_URL}/todos/1 ---\n`);
    //     const headResponse = await httpClient.head(`${TEST_API_URL}/todos/1`);
    //     stdout.write(`Status: ${headResponse.status}\n`);
    //     stdout.write(`Headers: ${JSON.stringify(headResponse.headers)}\n`);
    //     // Body for HEAD should be empty
    //     stdout.write(`Body length: ${headResponse.body.length}\n`);
    // } catch (error) {
    //     stdout.write(`HEAD Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 6: Generic 'request' function for a GET
    // try {
    //     stdout.write(`\n--- Generic GET ${TEST_API_URL}/users/1 ---\n`);
    //     const genericGetResponse = await httpClient.request("GET", `${TEST_API_URL}/users/1`);
    //     stdout.write(`Status: ${genericGetResponse.status}\n`);
    //     stdout.write(`Body: ${genericGetResponse.body}\n`);
    // } catch (error) {
    //     stdout.write(`Generic GET Error: ${error.name} - ${error.message}\n`);
    // }

    // await sleep(1000);

    // // Example 7: Request with timeout (expected to fail if timeout is too short)
    // try {
    //     stdout.write(`\n--- GET ${TEST_API_URL}/todos/2 with 1ms timeout (expected to fail) ---\n`);
    //     const timeoutResponse = await httpClient.get(`${TEST_API_URL}/todos/2`, { timeout: 1 });
    //     stdout.write(`Status: ${timeoutResponse.status}\n`);
    // } catch (error) {
    //     stdout.write(`Timeout GET Error: ${error.name} - ${error.message}\n`); // Expecting HttpError or similar
    // }

    // stdout.write("\nFinished httpClient examples.\n");
    // exit(0);
}

// Run the examples
runHttpExamples().catch(e => {
    stdout.write(`Unhandled global error in examples: ${e.message}\n`);
    exit(1);
});
