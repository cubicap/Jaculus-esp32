declare module "path" {
    function normalize(path: string): string;
    function dirname(path: string): string;
    function basename(path: string): string;
    function join(...paths: string[]): string;
    function isAbsolute(path: string): boolean;
}
