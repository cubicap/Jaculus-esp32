declare module "pulseCounter" {
    type LevelAction = number;
    type EdgeAction = number;

    const LevelAction: {
        Keep: LevelAction;
        Inverse: LevelAction;
        Hold: LevelAction;
    };

    const EdgeAction: {
        Hold: EdgeAction;
        Increase: EdgeAction;
        Decrease: EdgeAction;
    };

    type LevelMode = {
        low: LevelAction,
        high: LevelAction,
    }

    type EdgeMode = {
        pos: EdgeAction,
        neg: EdgeAction,
    }

    class PulseCounter {
        /**
         * Create a new pulse counter instance.
         * @param options The options for the pulse counter.
         */
        constructor(options: {
            pinLevel: number,
            pinEdge: number,
            levelMode: LevelMode,
            edgeMode: EdgeMode,
        })

        /**
         * Read the current value of the pulse counter.
         */
        read(): number;

        /**
         * Reset the pulse counter to 0.
         */
        clear(): void;

        /**
         * Start the pulse counter.
         */
        start(): void;

        /**
         * Stop the pulse counter.
         */
        stop(): void;

        /**
         * Close the pulse counter.
         */
        close(): void;
    }
}
