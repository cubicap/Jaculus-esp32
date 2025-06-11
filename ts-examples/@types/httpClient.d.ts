declare module "httpClient" {
    type HttpMethod =
        | "GET"
        | "POST"
        | "PUT"
        | "DELETE"
        | "PATCH"
        | "HEAD"
        | "OPTIONS";

    interface HttpRequestOptions {
        headers?: Record<string, string>;
        timeout?: number; // Timeout in milliseconds
    }

    interface HttpResponse {
        status: number;
        body: string;
        // headers: Record<string, string>;
    }

    /**
     * Performs an HTTP GET request.
     * @param url The URL to request.
     * @param options Optional request options (e.g., headers, timeout).
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    // function get(url: string, options?: HttpRequestOptions): Promise<HttpResponse>;

    function get(url: string): HttpResponse;

    /**
     * Performs an HTTP POST request.
     * @param url The URL to request.
     * @param body The body of the request (string or object to be JSON stringified).
     * @param options Optional request options (e.g., headers, timeout).
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    function post(url: string, body: string | object, options?: HttpRequestOptions): Promise<HttpResponse>;

    /**
     * Performs an HTTP PUT request.
     * @param url The URL to request.
     * @param body The body of the request (string or object to be JSON stringified).
     * @param options Optional request options.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    function put(url: string, body: string | object, options?: HttpRequestOptions): Promise<HttpResponse>;

    /**
     * Performs an HTTP DELETE request.
     * @param url The URL to request.
     * @param options Optional request options.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    function del(url: string, options?: HttpRequestOptions): Promise<HttpResponse>; // 'delete' is a keyword

    /**
     * Performs an HTTP PATCH request.
     * @param url The URL to request.
     * @param body The body of the request (string or object to be JSON stringified).
     * @param options Optional request options.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    function patch(url: string, body: string | object, options?: HttpRequestOptions): Promise<HttpResponse>;

    /**
     * Performs an HTTP HEAD request.
     * @param url The URL to request.
     * @param options Optional request options.
     * @returns A promise that resolves with the HTTP response (typically no body) or rejects with an error.
     */
    function head(url: string, options?: HttpRequestOptions): Promise<HttpResponse>;

    /**
     * Performs a generic HTTP request.
     * @param method The HTTP method (GET, POST, etc.).
     * @param url The URL to request.
     * @param body The body of the request (string or object for methods that support it).
     * @param options Optional request options.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     */
    function request(method: HttpMethod, url: string, body?: string | object, options?: HttpRequestOptions): Promise<HttpResponse>;


    function read(pin: number): number;
}
