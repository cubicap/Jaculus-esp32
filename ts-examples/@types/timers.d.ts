declare function sleep(ms: number): Promise<void>;

declare function setInterval(callback: () => void, ms: number): number;
declare function setTimeout(callback: () => void, ms: number): number;
declare function clearInterval(id: number): void;
declare function clearTimeout(id: number): void;
