import { stdout, stdin } from "stdio";

/**
 * A class for reading standard input line by line.
 */

export class readline {
    private buffer: string = "";
    private promise: Promise<string> | null = null;
    private resolve: ((value: string) => void) | null = null;
    private reject: ((reason: any) => void) | null = null;
    private closed: boolean = false;
    private echo: boolean;

    private onGet(str: string) {
        if (this.echo) {
            stdout.write(str);
        }

        if (str == "\n") {
            if (this.resolve) {
                this.resolve(this.buffer);
            }

            this.buffer = "";
            this.promise = null;
            this.resolve = null;
            this.reject = null;

            return;
        }

        this.buffer += str;

        if (!this.closed) {
            stdin.get()
            .then((data) => this.onGet(data))
            .catch((reason) => {
                if (this.reject) {
                    this.reject(reason);
                }
            });
        }

    }

    constructor(echo: boolean = false) {
        this.echo = echo;
    }

    public read(): Promise<string> {
        if (this.promise != null) {
            return Promise.reject("Already reading");
        }

        this.promise = new Promise((resolve, reject) => {
            this.resolve = resolve;
            this.reject = reject;
        });

        stdin.get()
        .then((data) => this.onGet(data))
        .catch((reason) => {
            if (this.reject) {
                this.reject(reason);
            }
        });

        return this.promise;
    }

    public close() {
        this.closed = true;

        if (this.reject) {
            this.reject("Stopped");
        }

        this.buffer = "";
        this.promise = null;
        this.resolve = null;
        this.reject = null;
    }
}
