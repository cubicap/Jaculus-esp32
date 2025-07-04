declare module "wifi" {
    /**
    * Return current IPv4 of the device, or null if WiFi is disabled or not connected.
    */
    function currentIp(): string | null;

    /**
    * List all saved WiFi networks.
    */
    function listNetworks(): string[];
}
