declare module "wifi" {
    /**
    * Return current IPv4 of the device, or null if WiFi is disabled or not connected.
    */
    function currentIp(): string | null;
}
