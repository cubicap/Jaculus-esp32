declare module "embedded:io/digital" {
    type DigitalMode = number;
    type DigitalEdge = number;

    class Digital {
        constructor(options: {
            pin: number,
            mode: DigitalMode,
            edge?: DigitalEdge,
            debounce?: number,
            onReadable?: ({ edge: DigitalEdge, timestamp: number }) => void },
        );

        /**
         * Reads the value of the pin.
         * @returns The value of the pin.
         */
        read(): number;

        /**
         * Writes the value of the pin.
         * @param value The value to write.
         */
        write(value: number): void;

        close(): void;

        static Input: DigitalMode;
        static InputPullUp: DigitalMode;
        static InputPullDown: DigitalMode;
        static InputPullUpDown: DigitalMode;
        static Output: DigitalMode;
        static OutputOpenDrain: DigitalMode;

        static None: DigitalEdge;
        static Rising: DigitalEdge;
        static Falling: DigitalEdge;
    }
}
