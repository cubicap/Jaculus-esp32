declare module "keyvalue" {
    class KeyValueNamespace {
        /**
         * Get saved value.
         * @param key key, max 15 characters.
         * @return saved value or null if not present
         */
        get(key: string): string | number | null;

        /**
         * Get string saved value.
         * @param key key, max 15 characters.
         * @return saved value or null if not present or if not string
         */
        getString(key: string): string | null;

        /**
         * Get number saved value.
         * @param key key, max 15 characters.
         * @return saved value or null if not present or if not number
         */
        getNumber(key: string): number | null;

        /**
         * Set value in KeyValue namespace, overwriting any previous value.
         * Call commit() after setting everything, otherwise changes will be lost!
         * @param key key, max 15 characters.
         * @param value value
         */
        set(key: string, value: string | number): void;

        /**
         * Erase value from KeyValue namespace.
         * Call commit() after setting everything, otherwise changes will be lost!
         * @param key key, max 15 characters.
         */
        erase(key: string): void;

        /**
         * Check existance of a key.
         * @param key key, max 15 characters.
         * @returns true if exists, false otherwise
         */
        exists(key: string): boolean;

        /**
         * List all keys in the namespace.
         * @returns array of keys
         */
        keys(): string[];

        /**
         * Save modifications to persistent storage.
         */
        commit(): void;
    }

    /**
     * Open a KeyValue namespace for use. KeyValue is a persitent storage for small
     * pieces of data that saves values across restarts.
     *
     * The namespaces are isolated from each other.
     *
     * @param namespace Namespace name, max 15 characters.
     */
    function open(namespace: string): KeyValueNamespace;
}
