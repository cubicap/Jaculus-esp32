declare module "httpClient" {
    /**
     * HTTP Response object returned by all HTTP methods
     */
    interface HttpResponse {
        status: number;  // HTTP status code (200, 404, etc.)
        body: string;    // Response body as string
    }

    /**
     * Performs an HTTP GET request.
     * @param url The URL to request.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     * @throws Exception if no IP address (WiFi not connected) or task creation fails.
     */
    function get(url: string): Promise<HttpResponse>;

    /**
     * Performs an HTTP POST request.
     * @param url The URL to request.
     * @param data Optional request body data (default: "").
     * @param contentType Optional content type header (default: "application/json").
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     * @throws Exception if no IP address (WiFi not connected) or task creation fails.
     */
    function post(url: string, data?: string, contentType?: string): Promise<HttpResponse>;

    /**
     * Performs an HTTP PUT request.
     * @param url The URL to request.
     * @param data Optional request body data (default: "").
     * @param contentType Optional content type header (default: "application/json").
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     * @throws Exception if no IP address (WiFi not connected) or task creation fails.
     */
    function put(url: string, data?: string, contentType?: string): Promise<HttpResponse>;

    /**
     * Performs an HTTP DELETE request.
     * @param url The URL to request.
     * @returns A promise that resolves with the HTTP response or rejects with an error.
     * @throws Exception if no IP address (WiFi not connected) or task creation fails.
     * @note 'delete' is a TypeScript keyword, so the function is named 'del'.
     */
    function del(url: string): Promise<HttpResponse>;
}
