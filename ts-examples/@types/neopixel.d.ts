declare module "neopixel" {
    interface Rgb {
        r: number;
        g: number;
        b: number;
    }

    class Neopixel {
        /**
         * Create a new Neopixel strip.
         * @param pin The pin the strip is connected to.
         * @param count The number of LEDs in the strip.
         */
        constructor(pin: number, count: number);

        /**
         * Show the current buffer on the strip.
         */
        public show(): void;

        /**
         * Set the color of the given LED.
         * @param index The index of the LED to set.
         * @param rgb The color to set the LED to.
         */
        public set(index: number, rgb: Rgb): void;

        /**
         * Get the color of the given LED.
         * @param index The index of the LED to get.
         * @returns The color of the LED.
         */
        public get(index: number): Rgb;
    }
}
