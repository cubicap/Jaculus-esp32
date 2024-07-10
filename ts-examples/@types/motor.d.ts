declare module "motor" {

    type MoveDuration = { distance: number; } | { time: number; } | { };

    type MotorPins = {
        motA: number;
        motB: number;
        encA: number;
        encB: number;
    };

    type LedcConfig = {
        timer: number;
        channelA: number;
        channelB: number;
    };

    class Motor {
        /**
         * Construc a new Motor instance
         * @param options Motor configuration
         * @note Units used in the circumference parameter determines the units used in the other methods
         */
        constructor(options: { pins: MotorPins, ledc: LedcConfig, encTicks: number, circumference: number });

        /**
         * Set the speed of the motor
         * @param speed Speed in units per second
         * @note The units are the same as used in the circumference parameter
         */
        setSpeed(speed: number): void;
        // setRamp(ramp: number): void;

        /**
         * Move the motor
         * @param duration Duration of the movement
         * @note The units are the same as used in the circumference parameter
         * @note If the duration is not provided, the motor will move indefinitely
         */
        move(duration?: MoveDuration): Promise<void>;

        /**
         * Stop the motor
         * @param brake If true, the motor will brake, otherwise it will coast to a stop
         */
        stop(brake?: boolean): Promise<void>;

        /**
         * Get the position of the motor
         * @note The units are the same as used in the circumference parameter
         * @returns The position of the motor
         */
        getPosition(): number;

        /**
         * Close the motor
         */
        close(): void;
    }

    /* class Wheels {
        constructor(options: {
            left: MotorPins,
            right: MotorPins,
            encTicks: number,
            diameter: number,
            spacing: number
        });
        setSpeed(speed: number): void;
        // setRamp(ramp: number): void;

        move(curve: number, duration?: MoveDuration): Promise<void>;
        // rotate(angle: number): Promise<void>;
        stop(brake?: boolean): Promise<void>;
        getPosition(): { left: number, right: number };

        close(): void;
    } */
}
