declare module "embedded:io/i2c" {
    class I2C {
        constructor(options: {
            data: number,
            clock: number,
            hz: number,
            address: number,
            port?: number
        });

        /**
         * Reads data from the I2C bus.
         * @param buffer The buffer to read data into.
         * @param stop Whether to send a stop bit after reading, default: true.
         */
        read(buffer: ArrayBuffer, stop?: boolean): number;

        /**
         * Reads data from the I2C bus.
         * @param byteLength The number of bytes to read.
         * @param stop Whether to send a stop bit after reading, default: true.
         * @returns The data read from the bus.
         */
        read(byteLength: number, stop?: boolean): ArrayBuffer;

        /**
         * Writes data to the I2C bus.
         * @param buffer The buffer to write data from.
         * @param stop Whether to send a stop bit after writing, default: true.
         */
        write(buffer: ArrayBuffer, stop?: boolean): void;
    }
}
