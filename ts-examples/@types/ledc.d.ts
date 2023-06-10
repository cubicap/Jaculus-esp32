declare module "ledc" {
    /**
     * Configure the given timer.
     * @param timer The timer to configure.
     * @param frequency The frequency to configure the timer to.
     * @param resolution The resolution to configure the timer to (default 10 bits, changes frequency range)
     */
    function configureTimer(timer: number, frequency: number, resolution?: number): void;

    /**
     * Configure the given LEDC channel.
     * @param channel The channel to configure.
     * @param pin The pin to configure the channel to.
     * @param timer The timer to configure the channel to.
     * @param duty The duty to configure the channel to (0-1023).
     */
    function configureChannel(channel: number, pin: number, timer: number, duty: number): void;

    /**
     * Set the frequency of the given timer.
     * @param timer The timer to set the frequency of.
     * @param frequency The frequency to set the timer to.
     */
    function setFrequency(timer: number, frequency: number): void;

    /**
     * Set the duty of the given channel.
     * @param channel The channel to set the duty of.
     * @param duty The duty to set the channel to (0-1023).
     */
    function setDuty(channel: number, duty: number): void;
}
