declare interface Timestamp {
    /**
     * Convert the timestamp to milliseconds.
     */
    millis(): number;

    /**
     * Get the lower 10^3 part of the timestamp in microseconds.
     */
    micros(): number;
}
