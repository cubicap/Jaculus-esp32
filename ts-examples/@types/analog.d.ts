declare module "embedded:io/analog" {
    class Analog {
        constructor(options: {
            pin: number,
            resolution?: number
        });

        /**
         * Reads the value of the pin.
         * @returns The value of the pin.
         */
        read(): number;

        close(): void;
    }
}
