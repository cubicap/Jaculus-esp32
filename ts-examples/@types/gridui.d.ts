declare module "gridui" {
    /**
     * Initialize GridUI.
     * @param ownerName name of the owner, must match the name entered in RBController app.
     * @param deviceName name of this device, visible in the RBController app.
     */
    function begin(ownerName: string, deviceName: string): void;

    /**
     * Stop GridUI.
     */
    function end(): void;
}
