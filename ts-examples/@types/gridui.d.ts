declare module "gridui" {
    namespace widget {
        interface Arm {
        }

        interface Bar {
            color: string
            fontSize: number
            min: number
            max: number
            value: number
            showValue: boolean
        }

        interface Button {
            text: string
            fontSize: number
            color: string
            background: string
            align: string
            valign: string
            disabled: boolean
        }

        interface Camera {
            rotation: number
            clip: boolean
        }

        interface Checkbox {
            fontSize: number
            checked: boolean
            color: string
            text: string
        }

        interface Circle {
            color: string
            fontSize: number
            min: number
            max: number
            lineWidth: number
            valueStart: number
            value: number
            showValue: boolean
        }

        interface Input {
            text: string
            color: string
            type: string
            disabled: boolean
        }

        interface Joystick {
            color: string
            keys: string
            text: string
        }

        interface Led {
            color: string
            on: boolean
        }

        interface Orientation {
            color: string
        }

        interface Select {
            color: string
            background: string
            disabled: boolean
            options: string
            selectedIndex: number
        }

        interface Slider {
            color: string
            fontSize: number
            min: number
            max: number
            value: number
            precision: number
            showValue: boolean
        }

        interface SpinEdit {
            fontSize: number
            color: string
            value: number
            step: number
            precision: number
        }

        interface Switcher {
            fontSize: number
            color: string
            value: number
            min: number
            max: number
        }

        interface Text {
            text: string
            fontSize: number
            color: string
            background: string
            align: string
            valign: string
            prefix: string
            suffix: string
        }
    }

    namespace builder {
        interface Arm {
            info(infoJson: string): Arm;
        }

        interface Bar {
            color(color: string): Bar;
            fontSize(fontSize: number): Bar;
            min(min: number): Bar;
            max(max: number): Bar;
            value(value: number): Bar;
            showValue(showValue: boolean): Bar;
        }

        interface Button {
            text(text: string): Button;
            fontSize(fontSize: number): Button;
            color(color: string): Button;
            background(background: string): Button;
            align(align: string): Button;
            valign(valign: string): Button;
            disabled(disabled: boolean): Button;
        }

        interface Camera {
            rotation(rotation: number): Camera;
            clip(clip: boolean): Camera;
            tags(tags: any /* TODO: fix type */): Camera;
        }

        interface Checkbox {
            fontSize(fontSize: number): Checkbox;
            checked(checked: boolean): Checkbox;
            color(color: string): Checkbox;
            text(text: string): Checkbox;
        }

        interface Circle {
            color(color: string): Circle;
            fontSize(fontSize: number): Circle;
            min(min: number): Circle;
            max(max: number): Circle;
            lineWidth(lineWidth: number): Circle;
            valueStart(valueStart: number): Circle;
            value(value: number): Circle;
            showValue(showValue: boolean): Circle;
        }

        interface Input {
            text(text: string): Input;
            color(color: string): Input;
            type(type: string): Input;
            disabled(disabled: boolean): Input;
        }

        interface Joystick {
            color(color: string): Joystick;
            keys(keys: string): Joystick;
            text(text: string): Joystick;
        }

        interface Led {
            color(color: string): Led;
            on(on: boolean): Led;
        }

        interface Orientation {
            color(color: string): Orientation;
        }

        interface Select {
            color(color: string): Select;
            background(background: string): Select;
            disabled(disabled: boolean): Select;
            options(options: string): Select;
            selectedIndex(selectedIndex: number): Select;
        }

        interface Slider {
            color(color: string): Slider;
            fontSize(fontSize: number): Slider;
            min(min: number): Slider;
            max(max: number): Slider;
            value(value: number): Slider;
            precision(precision: number): Slider;
            showValue(showValue: boolean): Slider;
        }

        interface SpinEdit {
            fontSize(fontSize: number): SpinEdit;
            color(color: string): SpinEdit;
            value(value: number): SpinEdit;
            step(step: number): SpinEdit;
            precision(precision: number): SpinEdit;
        }

        interface Switcher {
            fontSize(fontSize: number): Switcher;
            color(color: string): Switcher;
            value(value: number): Switcher;
            min(min: number): Switcher;
            max(max: number): Switcher;
        }

        interface Text {
            text(text: string): Text;
            fontSize(fontSize: number): Text;
            color(color: string): Text;
            background(background: string): Text;
            align(align: string): Text;
            valign(valign: string): Text;
            prefix(prefix: string): Text;
            suffix(suffix: string): Text;
        }
    }

    class Builder {
        arm(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Arm;
        bar(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Bar;
        button(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Button;
        camera(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Camera;
        checkbox(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Checkbox;
        circle(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Circle;
        input(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Input;
        joystick(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Joystick;
        led(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Led;
        orientation(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Orientation;
        select(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Select;
        slider(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Slider;
        spinEdit(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.SpinEdit;
        switcher(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Switcher;
        text(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Text;
    }

    /**
     * Initialize GridUI.
     * @param ownerName name of the owner, must match the name entered in RBController app.
     * @param deviceName name of this device, visible in the RBController app.
     * @param builderCallback callback, which receives the builder instance that can be used to create widgets.
     */
    function begin(ownerName: string, deviceName: string, builderCallback: (builder: Builder) => void): void;

    /**
     * Stop GridUI.
     */
    function end(): void;
}
