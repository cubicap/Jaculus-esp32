declare module "simpleradio" {
    type PacketDataType = "number" | "string" | "keyvalue";
    interface PacketInfo {
        group: number;
        address: string;
        rssi: number;
    }

    function begin(group: number): void;
    function setGroup(group: number): void;
    function sendString(str: string): void;
    function sendNumber(num: number): void;
    function sendKeyValue(key: string, value: number): void;
    function on(type: "number", callback: (num: number, info: PacketInfo) => void): void;
    function on(type: "string", callback: (str: string, info: PacketInfo) => void): void;
    function on(type: "keyvalue", callback: (key: string, value: number, info: PacketInfo) => void): void;
}
