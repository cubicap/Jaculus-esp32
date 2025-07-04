declare module "wifi" {
    /**
    * Return current IPv4 of the device, or null if WiFi is disabled or not connected.
    */
    function currentIp(): string | null;

    /**
     * Waits for the device to obtain an IP address.
     * @param timeoutMs Optional timeout in milliseconds. If not provided, waits indefinitely.
     * @returns A promise that resolves to true if an IP address is obtained, or false if the timeout is reached.
     */
    function waitForIp(timeoutMs?: number): boolean;
}
