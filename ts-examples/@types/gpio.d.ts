declare module "gpio" {
    const PinMode: {
        readonly DISABLE: number,
        readonly OUTPUT: number,
        readonly INPUT: number,
        readonly INPUT_PULLUP: number,
        readonly INPUT_PULLDOWN: number,
    };

    interface EventInfo {
        timestamp: Timestamp;
    }

    /**
     * Configure the given pin.
     * @param pin The pin to configure.
     * @param mode The mode to configure the pin in.
     */
    function pinMode(pin: number, mode: number): void;

    /**
     * Write digital value to the given pin.
     * @param pin The pin to write to.
     * @param value The value to write.
     */
    function write(pin: number, value: number): void;

    /**
     * Read digital value from the given pin.
     * @param pin The pin to read from.
     * @returns The value of the pin (0 or 1).
     */
    function read(pin: number): number;

    /**
     * Set event handler for the given pin.
     * @param event The event to handle.
     * @param pin The pin to handle the event for.
     * @param callback The callback to call when the event occurs.
     */
    function on(event: "rising" | "falling" | "change", pin: number, callback: (info: EventInfo) => void): void;

    /**
     * Remove event handler for the given pin.
     * @param event The event to remove.
     * @param pin The pin to remove the event handler for.
     */
    function off(event: "rising" | "falling" | "change", pin: number): void;
}
