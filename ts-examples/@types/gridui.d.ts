declare module "gridui" {
    namespace widget {
        interface Base {
          readonly uuid: number
          widgetX: number
          widgetY: number
          widgetW: number
          widgetH: number
          widgetTab: number
          css(key: string): string
          setCss(key: string, value: string): void
        }
      
        interface Arm extends Base {
            readonly x: number
            readonly y: number
        }

        interface Bar extends Base {
            color: string
            fontSize: number
            min: number
            max: number
            value: number
            showValue: boolean
        }

        interface Button extends Base {
            text: string
            fontSize: number
            color: string
            background: string
            align: string
            valign: string
            disabled: boolean
            readonly pressed: boolean
        }

        interface Camera extends Base {
            rotation: number
            clip: boolean
        }

        interface Checkbox extends Base {
            fontSize: number
            checked: boolean
            color: string
            text: string
        }

        interface Circle extends Base {
            color: string
            fontSize: number
            min: number
            max: number
            lineWidth: number
            valueStart: number
            value: number
            showValue: boolean
        }

        interface Input extends Base {
            text: string
            color: string
            type: string
            disabled: boolean
        }

        interface Joystick extends Base {
            color: string
            keys: string
            text: string
            readonly x: number
            readonly y: number
        }

        interface Led extends Base {
            color: string
            on: boolean
        }

        interface Orientation extends Base {
            color: string
            readonly yaw: number
            readonly pitch: number
            readonly roll: number
            readonly joystickX: number
            readonly joystickY: number
        }

        interface Select extends Base {
            color: string
            background: string
            disabled: boolean
            options: string
            selectedIndex: number
        }

        interface Slider extends Base {
            color: string
            fontSize: number
            min: number
            max: number
            value: number
            precision: number
            showValue: boolean
        }

        interface SpinEdit extends Base {
            fontSize: number
            color: string
            value: number
            step: number
            precision: number
        }

        interface Switcher extends Base {
            fontSize: number
            color: string
            value: number
            min: number
            max: number
        }

        interface Text extends Base {
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
        interface Base {
            css(key: string, value: string): this
        }

        interface Arm extends Base {
            info(info: Record<string, any>): Arm

            onGrab(callback: (arm: widget.Arm) => void): Arm
            onPositionChanged(callback: (arm: widget.Arm) => void): Arm

            finish(): widget.Arm
        }

        interface Bar extends Base {
            color(color: string): Bar
            fontSize(fontSize: number): Bar
            min(min: number): Bar
            max(max: number): Bar
            value(value: number): Bar
            showValue(showValue: boolean): Bar

            finish(): widget.Bar
        }

        interface Button extends Base {
            text(text: string): Button
            fontSize(fontSize: number): Button
            color(color: string): Button
            background(background: string): Button
            align(align: string): Button
            valign(valign: string): Button
            disabled(disabled: boolean): Button

            onPress(callback: (button: widget.Button) => void): Button
            onRelease(callback: (button: widget.Button) => void): Button

            finish(): widget.Button
        }

        interface Camera extends Base {
            rotation(rotation: number): Camera
            clip(clip: boolean): Camera
            tags(tags: any /* TODO: fix type */): Camera

            finish(): widget.Camera
        }

        interface Checkbox extends Base {
            fontSize(fontSize: number): Checkbox
            checked(checked: boolean): Checkbox
            color(color: string): Checkbox
            text(text: string): Checkbox

            onChanged(callback: (checkbox: widget.Checkbox) => void): Checkbox

            finish(): widget.Checkbox
        }

        interface Circle extends Base {
            color(color: string): Circle
            fontSize(fontSize: number): Circle
            min(min: number): Circle
            max(max: number): Circle
            lineWidth(lineWidth: number): Circle
            valueStart(valueStart: number): Circle
            value(value: number): Circle
            showValue(showValue: boolean): Circle

            finish(): widget.Circle
        }

        interface Input extends Base {
            text(text: string): Input
            color(color: string): Input
            type(type: string): Input
            disabled(disabled: boolean): Input

            onChanged(callback: (input: widget.Input) => void): Input

            finish(): widget.Input
        }

        interface Joystick extends Base {
            color(color: string): Joystick
            keys(keys: string): Joystick
            text(text: string): Joystick

            onClick(callback: (joystick: widget.Joystick) => void): Joystick
            onPositionChanged(callback: (joystick: widget.Joystick) => void): Joystick

            finish(): widget.Joystick
        }

        interface Led extends Base {
            color(color: string): Led
            on(on: boolean): Led

            finish(): widget.Led
        }

        interface Orientation extends Base {
            color(color: string): Orientation

            onPositionChanged(callback: (orientation: widget.Orientation) => void): Orientation

            finish(): widget.Orientation
        }

        interface Select extends Base {
            color(color: string): Select
            background(background: string): Select
            disabled(disabled: boolean): Select
            options(options: string): Select
            selectedIndex(selectedIndex: number): Select

            onChanged(callback: (select: widget.Select) => void): Select

            finish(): widget.Select
        }

        interface Slider extends Base {
            color(color: string): Slider
            fontSize(fontSize: number): Slider
            min(min: number): Slider
            max(max: number): Slider
            value(value: number): Slider
            precision(precision: number): Slider
            showValue(showValue: boolean): Slider

            onChanged(callback: (slider: widget.Slider) => void): Slider

            finish(): widget.Slider
        }

        interface SpinEdit extends Base {
            fontSize(fontSize: number): SpinEdit
            color(color: string): SpinEdit
            value(value: number): SpinEdit
            step(step: number): SpinEdit
            precision(precision: number): SpinEdit

            onChanged(callback: (spinEdit: widget.SpinEdit) => void): SpinEdit

            finish(): widget.SpinEdit
        }

        interface Switcher extends Base {
            fontSize(fontSize: number): Switcher
            color(color: string): Switcher
            value(value: number): Switcher
            min(min: number): Switcher
            max(max: number): Switcher

            onChanged(callback: (switcher: widget.Switcher) => void): Switcher

            finish(): widget.Switcher
        }

        interface Text extends Base {
            text(text: string): Text
            fontSize(fontSize: number): Text
            color(color: string): Text
            background(background: string): Text
            align(align: string): Text
            valign(valign: string): Text
            prefix(prefix: string): Text
            suffix(suffix: string): Text

            finish(): widget.Text
        }
    }

    class Builder {
        arm(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Arm
        bar(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Bar
        button(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Button
        camera(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Camera
        checkbox(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Checkbox
        circle(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Circle
        input(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Input
        joystick(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Joystick
        led(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Led
        orientation(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Orientation
        select(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Select
        slider(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Slider
        spinEdit(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.SpinEdit
        switcher(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Switcher
        text(x: number, y: number, w: number, h: number, uuid?: number, tab?: number): builder.Text
    }

    /**
     * Initialize GridUI.
     * @param ownerName name of the owner, must match the name entered in RBController app.
     * @param deviceName name of this device, visible in the RBController app.
     * @param builderCallback callback, which receives the builder instance that can be used to create widgets.
     */
    function begin(ownerName: string, deviceName: string, builderCallback: (builder: Builder) => void): void

    /**
     * Stop GridUI.
     */
    function end(): void

    /**
     * Set current tab index
     */
    function changeTab(index: number): void

    /**
     * Send a message to the integrated terminal at the top of the UI.
     */
    function log(message: string): void

    /**
     * Returns included GridUI version as number, to be compared with hex representation of the version.
     * 
     * For example, for version 5.1.0, do: `gridui.version() >= 0x050100`
     */
    function version(): number
}
