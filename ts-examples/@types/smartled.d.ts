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
        constructor(options: {
            pin: number,
            count: number,
            type?: LedType
        });

        begin(options: {
            x: number,
            y: number,
            width: number,
            height: number
        });

        send(data: ArrayBuffer): void;

        close(): void;
    }

    const LED_WS2812: LedType;
    const LED_WS2812B: LedType;
    const LED_WS2812B_2020: LedType;
    const LED_SK6812: LedType;
    const LED_WS2813: LedType;
}
