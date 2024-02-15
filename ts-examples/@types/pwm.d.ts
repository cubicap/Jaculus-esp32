declare module "embedded:io/pwm" {
    class PWM{
        constructor(options: {
            pin: number,
            hz: number,
            resolution: number
        });

        hz: number;

        /**
         * Returns number of the timer used by the PWM.
         */
        timer(): number;

        /**
         * Sets the PWM duty cycle.
         * @param duty The duty cycle to set.
         */
        write(duty: number): void;

        /**
         * Stops the PWM.
         */
        close(): void;
    }
}
