declare module "smartled" {
    interface Rgb {
        r: number;
        g: number;
        b: number;
    }

    interface LedType {
        T0H: number;
        T1H: number;
        T0L: number;
        T1L: number;
        TRS: number;
    }

    class SmartLed {
        /**
         * Create a new Smart LED strip.
         * @param pin The pin the strip is connected to.
         * @param count The number of LEDs in the strip.
         * @param type The type of LED strip.
         */
        constructor(pin: number, count: number, type?: LedType);

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

        /**
         * Clear the buffer.
         */
        public clear(): void;
    }

    const LED_WS2812: LedType;
    const LED_WS2812B: LedType;
    const LED_WS2812B_2020: LedType;
    const LED_SK6812: LedType;
    const LED_WS2813: LedType;
}
