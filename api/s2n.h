#pragma once

#ifndef S2N_API
# define S2N_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#ifndef _WIN32
#include <sys/uio.h>
#else
struct iovec {
  void *iov_base;
  size_t iov_len;
}
#endif

#define S2N_SUCCESS 0
#define S2N_FAILURE -1
#define S2N_CALLBACK_BLOCKED -2
#define S2N_MINIMUM_SUPPORTED_TLS_RECORD_MAJOR_VERSION 2
#define S2N_MAXIMUN_SUPPORTED_TLS_RECORD_MAJOR_VERSION 3
#define S2N_SSLv2 20
#define S2N_SSLv3 30
#define S2N_TLS10 31
#define S2N_TLS11 32
#define S2N_TLS12 33
#define S2N_TLS13 34
#define S2N_UNKNOWN_PROTOCOL_VERSION 0

S2N_API extern __thread int s2n_errno;

S2N_API extern int *s2n_errno_location(void);

typedef enum {
  S2N_ERR_T_OK = 0,
  S2N_ERR_T_IO,
  S2N_ERR_T_CLOSED,
  S2N_ERR_T_BLOCKED,
  S2N_ERR_T_ALERT,
  S2N_ERR_T_PROTO,
  S2N_ERR_T_INTERNAL,
  S2N_ERR_T_USAGE
} s2n_error_type;

S2N_API extern int s2n_error_get_type(int error);

struct s2n_config;

struct s2n_connection;

S2N_API extern int s2n_crypto_disable_init(void);

S2N_API extern int s2n_disable_atexit(void);

S2N_API extern unsigned long s2n_get_openssl_version(void);

S2N_API extern int s2n_init(void);

S2N_API extern int s2n_cleanup(void);

S2N_API extern int s2n_cleanup_final(void);

typedef enum {
  S2N_FIPS_MODE_DISABLED = 0,
  S2N_FIPS_MODE_ENABLED,
} s2n_fips_mode;

S2N_API extern int s2n_get_fips_mode(s2n_fips_mode *fips_mode);

S2N_API extern struct s2n_config *s2n_config_new(void);

S2N_API extern struct s2n_config *s2n_config_new_minimal(void);

S2N_API extern int s2n_config_free(struct s2n_config *config);

S2N_API extern int s2n_config_free_dhparams(struct s2n_config *config);

S2N_API extern int s2n_config_free_cert_chain_and_key(struct s2n_config *config);

typedef int (*s2n_clock_time_nanoseconds)(void *, uint64_t *);

/**
 * Cache callback function that allows the caller to retrieve SSL session data
 * from a cache.
 */

typedef int (*s2n_cache_retrieve_callback)(struct s2n_connection *conn, void *, const void *key, uint64_t key_size, void *value, uint64_t *value_size);

/**
 * Cache callback function that allows the caller to store SSL session data in a cache
 */
typedef int (*s2n_cache_store_callback)(struct s2n_connection *conn, void *, uint64_t ttl_in_seconds, const void *key, uint64_t key_size, void *value, uint64_t *value_size);

/**
 * Cache callback function that allows the caller to set a callback function
 * that will be used to delete SSL session data from a cache.
 */
typedef int (*s2n_cache_delete_callback)(struct s2n_connection *conn, void *, const void* key, uint64_t key_size);

/**
 * Allows the caller to set a callback function that will be used to get the 
 * system time.
 */
S2N_API extern int s2n_config_set_wall_clock(struct s2n_config *config, s2n_clock_time_nanoseconds clock_fn, void *ctx);

/**
 * Allows the caller to set a callback function that will be used to get
 * monotonic time.
 */
S2N_API extern int s2n_config_set_monotonic_clock(struct s2n_config *config, s2n_clock_time_nanoseconds clock_fn, void *ctx);

/**
 * Translates an s2n_error code to a human readable string explaining the error.
 */
S2N_API extern const char *s2n_strerror(int error, const char *lang);

/**
 * Translates an s2n_error code to a human readable string containing internal debug
 * information, including file name and line number. This function is useful when
 * reporting issues to the s2n_tls development team.
 */
S2N_API extern const char *s2n_strerror_debug(int error, const char *lang);

/**
 * Translates an s2n_error code to a human readable string.
 */
S2N_API extern const char *s2n_strerror_name(int error);

/**
 * Translates an s2n_error code to a filename and line number.
 */
S2N_API extern const char *s2n_strerror_source(int error);

struct s2n_stacktrace;

/**
 * Checks if s2n stack trace captures are enabled.
 */
S2N_API extern bool s2n_stack_traces_enabled(void);

S2N_API extern int s2n_stack_traces_enabled_set(bool newval);

S2N_API extern int s2n_calculate_stacktrace(void);

S2N_API extern int s2n_print_stacktrace(FILE *fptr);

S2N_API extern int s2n_free_stacktrace(void);

S2N_API extern int s2n_get_stacktrace(struct s2n_stacktrace *trace);

S2N_API extern int s2n_config_set_cache_store_callback(struct s2n_config *config, s2n_cache_store_callback cache_store_callback, void *data);

S2N_API extern int s2n_config_set_cache_retrieve_callback(struct s2n_config *config, s2n_cache_retrieve_callback callback, void *data);

S2N_API extern int s2n_config_set_cache_delete_callback(struct s2n_config *config, s2n_cache_delete_callback callback, void* data);

/**
 * Called when s2n_init is executed
 */
typedef int (*s2n_mem_init_callback)(void);

/**
 * will be called when s2n_cleanup is executed.
 */
typedef int (*s2n_mem_cleanup_callback)(void);

typedef int (*s2n_mem_malloc_callback)(void **ptr, uint32_t requested, uint32_t *allocated);

typedef int (*s2n_mem_free_callback)(void *ptr, uint32_t size);

S2N_API extern int s2n_mem_set_callback(s2n_mem_init_callback mem_init_callback, s2n_mem_cleanup_callback mem_cleanup_callback, s2n_mem_malloc_callback mem_malloc_callback, s2n_mem_free_callback mem_free_callback);

typedef int (*s2n_rand_init_callback)(void);

typedef int (*s2n_rand_cleanup_callback)(void);

typedef int (*s2n_rand_seed_callback)(void *data, uint32_t size);

typedef int (*s2n_rand_mix_callback)(void *data, uint32_t size);

S2N_API extern int s2n_rand_set_callbacks(s2n_rand_init_callback rand_init_callback, s2n_rand_cleanup_callback rand_cleanup_callback, s2n_rand_seed_callback rand_seed_callback, s2n_rand_mix_callback rand_mix_callback);

typedef enum {
  S2N_EXTENSION_SERVER_NAME = 0,
  S2N_EXTENSION_MAX_FLAG_LEN = 1,
  S2N_EXTENSION_OCSP_STAPLING = 5,
  S2N_EXTENSION_SUPPORTED_GROUPS = 10,
  S2N_EXTENSION_EC_POINT_FORMATS = 11,
  S2N_EXTENSION_SIGNATURE_ALGORITHMS = 13,
  S2N_EXTENSION_ALPN = 16,
  S2N_EXTENSION_CERTIFICATE_TRANSPARENCY = 18,
  S2N_EXTENSION_SUPPORTED_VERSIONS = 43,
  S2N_EXTENSION_RENEGOTIATION_INFO = 65281,
} s2n_tls_extension_type;

typedef enum {
  S2N_TLS_MAX_FRAG_LEN_512 = 1,
  S2N_TLS_MAX_FRAG_LEN_1024 = 2,
  S2N_TLS_MAX_FRAG_LEN_2048 = 3,
  S2N_TLS_MAX_FRAG_LEN_4096 = 4,
} s2n_max_frag_len;

struct s2n_cert;

struct s2n_cert_chain_and_key;

struct s2n_pkey;

typedef struct s2n_pkey s2n_cert_public_key;

typedef struct s2n_pkey s2n_cert_private_key;

S2N_API extern struct s2n_cert_chain_and_key *s2n_cert_chain_and_key_new(void);

S2N_API extern int s2n_cert_chain_and_key_load_pem(struct s2n_cert_chain_and_key *chain_and_key, const char *chain_pem, const char *private_key_pem);

S2N_API extern int s2n_cert_chain_and_key_load_pem_bytes(struct s2n_cert_chain_and_key *chain_and_key, uint8_t *chain_pem, uint32_t chain_pem_len, uint8_t *private_key_pem, uint32_t private_key_pem_len);

S2N_API extern int s2n_cert_chain_and_key_load_public_pem_bytes(struct s2n_cert_chain_and_key *chain_and_key, uint8_t *chain_pem, uint32_t chain_pem_len);

S2N_API extern int s2n_cert_chain_and_key_free(struct s2n_cert_chain_and_key *cert_and_key);

S2N_API extern int s2n_cert_chain_and_key_set_ctx(struct s2n_cert_chain_and_key *cert_and_key, void *ctx);

S2N_API extern void *s2n_cert_chain_and_key_get_ctx(struct s2n_cert_chain_and_key *cert_and_key);

S2N_API extern s2n_cert_private_key *s2n_cert_chain_and_key_get_private_key(struct s2n_cert_chain_and_key *cert_and_key);

S2N_API extern int s2n_cert_chain_and_key_set_oscp_data(struct s2n_cert_chain_and_key *chain_and_key, const uint8_t *data, uint32_t length);

S2N_API extern int s2n_cert_chain_and_key_set_sct_list(struct s2n_cert_chain_and_key *chain_and_key, const uint8_t *data, uint32_t length);

typedef struct s2n_cert_chain_and_key *(*s2n_cert_tiebreak_callback)(struct s2n_cert_chain_and_key *cert1, struct s2n_cert_chain_and_key *cert2, uint8_t *name, uint32_t name_len);

S2N_API extern int s2n_config_set_cert_tiebreak_callback(struct s2n_config *config, s2n_cert_tiebreak_callback cert_tiebreak_cb);

S2N_API extern int s2n_config_add_cert_chain_and_key(struct s2n_config *config, const char *cert_chain_pem, const char *private_key_pem);

S2N_API extern int s2n_config_add_cert_chain_and_key_to_store(struct s2n_config *config, struct s2n_cert_chain_and_key *cert_key_pair);

S2N_API extern int s2n_config_set_cert_chain_and_key_defaults(struct s2n_config *config, 
    struct s2n_cert_chain_and_key **cert_key_pairs, uint32_t num_cert_key_pairs);

S2N_API extern int s2n_config_set_verification_ca_location(struct s2n_config *config, const char* ca_pem_filename, const char *ca_dir);

S2N_API extern int s2n_config_add_pem_to_trust_store(struct s2n_config *config, const char *pem);

S2N_API extern int s2n_config_wipe_trust_store(struct s2n_config *config);

S2N_API extern int s2n_config_load_system_certs(struct s2n_config *config);

typedef enum {
  S2N_VERIFY_AFTER_SIGN_DISABLED,
  S2N_VERIFY_AFTER_SIGN_ENABLED
} s2n_verify_after_sign;

S2N_API extern int s2n_config_set_verify_after_sign(struct s2n_config *config, s2n_verify_after_sign mode);

S2N_API extern int s2n_config_set_send_buffer_size(struct s2n_config *config, uint32_t size);

S2N_API extern int s2n_config_set_recv_multi_record(struct s2n_config *config, bool enabled);

typedef uint8_t (*s2n_verify_host_fn)(const char *host_name, size_t host_name_len, void *data);

S2N_API extern int s2n_config_set_verify_host_callback(struct s2n_config *config, s2n_verify_host_fn, void *data);

S2N_API extern int s2n_config_set_check_stapled_ocsp_response(struct s2n_config *config, uint8_t check_oscp);

S2N_API extern int s2n_config_disable_x509_time_verification(struct s2n_config *config);

S2N_API extern int s2n_config_disable_x509_intent_verification(struct s2n_config *config);

S2N_API extern int s2n_config_disable_x509_verification(struct s2n_config *config);

S2N_API extern int s2n_config_set_max_cert_chain_depth(struct s2n_config *config, uint16_t max_depth);

S2N_API extern int s2n_config_add_dhparams(struct s2n_config *config, const char *dhparams_pem);

S2N_API extern int s2n_config_set_cipher_preferences(struct s2n_config *config, const char *version);

S2N_API extern int s2n_config_append_protocol_preference(struct s2n_config *config, const uint8_t *protocol, uint8_t protocol_len);

S2N_API extern int s2n_config_set_protocol_preferences(struct s2n_config *config, const char *const *protocols, int protocol_count);

typedef enum {
  S2N_STATUS_REQUEST_NONE = 0,
  S2N_STATUS_REQUEST_OCSP = 1
} s2n_status_request_type;

S2N_API extern int s2n_config_set_status_request_type(struct s2n_config *config, s2n_status_request_type type);

typedef enum {
  S2N_CT_SUPPORT_NONE = 0,
  S2N_CT_SUPPORT_REQUEST = 1
} s2n_ct_support_level;

S2N_API extern int s2n_config_set_ct_support_level(struct s2n_config *config, s2n_ct_support_level level);

typedef enum {
  S2N_ALERT_FAIL_ON_WARNINGS = 0,
  S2N_ALERT_IGNORE_WARNINGS = 1
} s2n_alert_behavior;

S2N_API extern int s2n_config_set_alert_behavior(struct s2n_config *config, s2n_alert_behavior alert_behavior);

S2N_API extern int s2n_config_set_extension_data(struct s2n_config *config, s2n_tls_extension_type type, const uint8_t *data, uint32_t length);

S2N_API extern int s2n_config_send_max_fragment_length(struct s2n_config *config, s2n_max_frag_len mfl_code);

S2N_API extern int s2n_config_accept_max_fragment_length(struct s2n_config *config);

S2N_API extern int s2n_config_set_session_state_lifetime(struct s2n_config *config, uint64_t lifetime_in_secs);

S2N_API extern int s2n_config_set_session_tickets_onoff(struct s2n_config *config, uint8_t enabled);

S2N_API extern int s2n_config_set_session_cache_onoff(struct s2n_config *config, uint8_t enabled);

S2N_API extern int s2n_config_set_ticket_encrypt_decrypt_key_lifetime(struct s2n_config *config, uint64_t lifetime_in_secs);

S2N_API extern int s2n_config_set_ticket_decrypt_key_lifetime(struct s2n_config *config, uint64_t lifetime_in_secs);

S2N_API extern int s2n_config_add_ticket_crypto_key(struct s2n_config *config, const uint8_t *name, uint32_t name_len, uint8_t *key, uint32_t key_len, uint64_t intro_time_in_seconds_from_epoch);

S2N_API extern int s2n_config_require_ticket_forward_secrecy(struct s2n_config *config, bool enabled);

S2N_API extern int s2n_config_set_ctx(struct s2n_config *config, void *ctx);

S2N_API extern int s2n_config_get_ctx(struct s2n_config *config, void **ctx);

typedef enum {
  S2N_SERVER,
  S2N_CLIENT
} s2n_mode;

S2N_API extern struct s2n_connection *s2n_connection_new(s2n_mode mode);

S2N_API extern int s2n_connection_set_config(struct s2n_connection *conn, struct s2n_config *config);

S2N_API extern int s2n_connection_set_ctx(struct s2n_connection *conn, void *ctx);

S2N_API extern void *s2n_connection_get_ctx(struct s2n_connection *conn);

typedef int s2n_client_hello_fn(struct s2n_connection *conn, void *ctx);

typedef enum {
  S2N_CLIENT_HELLO_CB_BLOCKING,
  S2N_CLIENT_HELLO_CB_NONBLOCKING
} s2n_client_hello_cb_mode;

S2N_API extern int s2n_config_set_client_hello_cb(struct s2n_config *config, s2n_client_hello_fn client_hello_callback, void *ctx);

S2N_API extern int s2n_config_set_client_hello_cb_mode(struct s2n_config *config, s2n_client_hello_cb_mode cb_mode);

S2N_API extern int s2n_client_hello_cb_done(struct s2n_connection *conn);

S2N_API extern int s2n_connection_server_name_extension_used(struct s2n_connection *conn);

struct s2n_client_hello;

S2N_API extern struct s2n_client_hello *s2n_connection_get_client_hello(struct s2n_connection *conn);

S2N_API extern struct s2n_client_hello *s2n_client_hello_parse_message(const uint8_t *bytes, uint32_t size);

S2N_API extern int s2n_client_hello_free(struct s2n_client_hello **ch);

S2N_API extern ssize_t s2n_client_hello_get_raw_message_length(struct s2n_client_hello *ch);

S2N_API extern ssize_t s2n_client_hello_get_raw_message(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

S2N_API extern ssize_t s2n_client_hello_get_cipher_suites_length(struct s2n_client_hello *ch);

S2N_API extern ssize_t s2n_client_hello_get_cipher_suites(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

S2N_API extern ssize_t s2n_client_hello_get_extensions_length(struct s2n_client_hello *ch);

S2N_API extern ssize_t s2n_client_hello_get_extensions(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

S2N_API extern ssize_t s2n_client_hello_get_extension_length(struct s2n_client_hello *ch, s2n_tls_extension_type extension_type);

S2N_API extern ssize_t s2n_client_hello_get_extension_by_id(struct s2n_client_hello *ch, s2n_tls_extension_type extension_type, uint8_t *out, uint32_t max_length);

S2N_API extern int s2n_client_hello_has_extension(struct s2n_client_hello *ch, uint16_t extension_iana, bool *exists);

S2N_API extern int s2n_client_hello_get_session_id_length(struct s2n_client_hello *ch, uint32_t *out_length);

S2N_API extern int s2n_client_hello_get_session_id(struct s2n_client_hello *ch, uint8_t *out, uint32_t *out_length, uint32_t max_length);

S2N_API extern int s2n_client_hello_get_compression_methods_length(struct s2n_client_hello *ch, uint32_t *out_length);

S2N_API extern int s2n_client_hello_get_compression_methods(struct s2n_client_hello *ch, uint8_t *list, uint32_t list_length, uint32_t *out_length);

S2N_API extern int s2n_client_hello_get_legacy_protocol_version(struct s2n_client_hello *ch, uint8_t *out);

S2N_API extern int s2n_client_hello_get_random(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

S2N_API extern int s2n_client_hello_get_supported_groups(struct s2n_client_hello *ch, uint16_t *groups, uint16_t groups_count_max, uint16_t *group_count);

S2N_API extern int s2n_client_hello_get_server_name_length(struct s2n_client_hello *ch, uint16_t *length);

S2N_API extern int s2n_client_hello_get_server_name(struct s2n_client_hello *ch, uint8_t *server_name, uint16_t length);

S2N_API extern int s2n_connection_set_fd(struct s2n_connection *conn, int fd);

S2N_API extern int s2n_connection_set_read_fd(struct s2n_connection *conn, int readfd);

S2N_API extern int s2n_connection_set_write_fd(struct s2n_connection *conn, int writefd);

S2N_API extern int s2n_connection_get_read_fd(struct s2n_connection *conn, int *readfd);

S2N_API extern int s2n_connection_get_write_fd(struct s2n_connection *conn, int *writefd);

S2N_API extern int s2n_connection_use_corked_io(struct s2n_connection *conn);

typedef int s2n_recv_fn(void *io_context, uint8_t *buf, uint32_t len);

typedef int s2n_send_fn(void *io_context, const uint8_t *buf, uint32_t len);

S2N_API extern int s2n_connection_set_recv_ctx(struct s2n_connection *conn, void *ctx);

S2N_API extern int s2n_connection_set_send_ctx(struct s2n_connection *conn, void *ctx);

S2N_API extern int s2n_connection_set_recv_cb(struct s2n_connection *conn, s2n_recv_fn recv);

S2N_API extern int s2n_connection_set_send_cb(struct s2n_connection *conn, s2n_send_fn send);

S2N_API extern int s2n_connection_prefer_throughput(struct s2n_connection *conn);

S2N_API extern int s2n_connection_prefer_low_latency(struct s2n_connection *conn);

S2N_API extern int s2n_connection_set_recv_buffering(struct s2n_connection *conn, bool enabled);

S2N_API extern uint32_t s2n_peek_buffered(struct s2n_connection *conn);

S2N_API extern int s2n_connection_set_dynamic_buffers(struct s2n_connection *conn, bool enabled);

S2N_API extern int s2n_connection_set_dynamic_record_threshold(struct s2n_connection *conn, uint32_t resize_threshold, uint16_t timeout_threshold);

S2N_API extern int s2n_connection_set_verify_host_callback(struct s2n_connection *conn, s2n_verify_host_fn host_fn, void *data);

typedef enum {
  S2N_BUILT_IN_BLINDING,
  S2N_SELF_SERVICE_BLINDING
} s2n_blinding;

S2N_API extern int s2n_connection_set_blinding(struct s2n_connection *conn, s2n_blinding blinding);

S2N_API extern uint64_t s2n_connection_get_delay(struct s2n_connection *conn);

S2N_API extern int s2n_config_set_max_blinding_delay(struct s2n_config *config, uint32_t seconds);

S2N_API extern int s2n_connection_set_cipher_preferences(struct s2n_connection *conn, const char *version);

typedef enum {
  S2N_KEY_UPDATE_NOT_REQUESTED = 0,
  S2N_KEY_UPDATE_REQUESTED
} s2n_peer_key_update;

S2N_API extern int s2n_connection_request_key_update(struct s2n_connection *conn, s2n_peer_key_update peer_request);

S2N_API extern int s2n_connection_append_protocol_preference(struct s2n_connection *conn, const uint8_t *protocol, uint8_t protocol_len);

S2N_API extern int s2n_connection_set_protocol_preferences(struct s2n_connection *conn, const char *const *protocols, int protocol_count);

S2N_API extern int s2n_set_server_name(struct s2n_connection *conn, const char *server_name);

S2N_API extern const char *s2n_get_server_name(struct s2n_connection *conn);

S2N_API extern const char *s2n_get_application_protocol(struct s2n_connection *conn);

S2N_API extern const uint8_t *s2n_connection_get_ocsp_response(struct s2n_connection *conn, uint32_t *length);

S2N_API extern const uint8_t *s2n_connection_get_sct_list(struct s2n_connection *conn, uint32_t *length);

typedef enum {
  S2N_NOT_BLOCKED = 0,
  S2N_BLOCKED_ON_READ,
  S2N_BLOCKED_ON_WRITE,
  S2N_BLOCKED_ON_APPLICATION_INPUT,
  S2N_BLOCKED_ON_EARLY_DATA,
} s2n_blocked_status;

S2N_API extern int s2n_negotiate(struct s2n_connection *conn, s2n_blocked_status *blocked);

S2N_API extern ssize_t s2n_send(struct s2n_connection *conn, const void *buf, ssize_t size, s2n_blocked_status *blocked);

#ifndef _WIN32
S2N_API extern ssize_t s2n_sendv(struct s2n_connection *conn, const struct iovec *bufs, ssize_t count, s2n_blocked_status *blocked);

S2N_API extern ssize_t s2n_sendv_with_offset(struct s2n_connection *con, const struct iovec *bufs, ssize_t count, ssize_t offs, s2n_blocked_status *blocked);

#endif

S2N_API extern ssize_t s2n_recv(struct s2n_connection *conn, void *buf, ssize_t size, s2n_blocked_status *blocked);

S2N_API extern uint32_t s2n_peek(struct s2n_connection *conn);

S2N_API extern int s2n_connection_free_handshake(struct s2n_connection *conn);

S2N_API extern int s2n_connection_release_buffers(struct s2n_connection *conn);

S2N_API extern int s2n_connection_wipe(struct s2n_connection *conn);

S2N_API extern int s2n_connection_free(struct s2n_connection *conn);

S2N_API extern int s2n_shutdown(struct s2n_connection *conn, s2n_blocked_status *blocked);

S2N_API extern int s2n_shutdown_send(struct s2n_connection *conn, s2n_blocked_status *blocked);

typedef enum {
  S2N_CERT_AUTH_NONE,
  S2N_CERT_AUTH_REQUIRED,
  S2N_CERT_AUTH_OPTIONAL
} s2n_cert_auth_type;

S2N_API extern int s2n_config_get_client_auth_type(struct s2n_config *config, s2n_cert_auth_type *client_auth_type);

S2N_API extern int s2n_config_set_client_auth_type(struct s2n_config *config, s2n_cert_auth_type client_auth_type);

S2N_API extern int s2n_connection_get_client_auth_type(struct s2n_connection *conn, s2n_cert_auth_type *client_auth_type);

S2N_API extern int s2n_connection_set_client_auth_type(struct s2n_connection *conn, s2n_cert_auth_type client_auth_type);

S2N_API extern int s2n_connection_get_client_cert_chain(struct s2n_connection *conn, uint8_t **der_cert_chain_out, uint32_t *cert_chain_len);

S2N_API extern int s2n_config_set_initial_ticket_count(struct s2n_config *config, uint8_t num);

S2N_API extern int s2n_connection_add_new_tickets_to_send(struct s2n_connection *conn, uint8_t num);

S2N_API extern int s2n_connection_get_tickets_sent(struct s2n_connection *conn, uint16_t *num);

S2N_API extern int s2n_connection_set_server_keying_material_lifetime(struct s2n_connection *conn, uint32_t lifetime_in_secs);

struct s2n_session_ticket;

typedef int (*s2n_session_ticket_fn)(struct s2n_connection *conn, void *ctx, struct s2n_session_ticket *ticket);

S2N_API extern int s2n_config_set_session_ticket_cb(struct s2n_config *config, s2n_session_ticket_fn callback, void *ctx);

S2N_API extern int s2n_session_ticket_get_data_len(struct s2n_session_ticket *ticket, size_t *data_len);

S2N_API extern int s2n_session_ticket_get_data(struct s2n_session_ticket *ticket, size_t max_data_len, uint8_t *data);

S2N_API extern int s2n_session_ticket_get_lifetime(struct s2n_session_ticket *ticket, uint32_t *session_lifetime);

S2N_API extern int s2n_connection_set_session(struct s2n_connection *conn, const uint8_t *session, size_t length);

S2N_API extern int s2n_connection_get_session(struct s2n_connection *conn, uint8_t *session, size_t max_length);

S2N_API extern int s2n_connection_get_session_ticket_lifetime_hint(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_session_length(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_session_id_length(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_session_id(struct s2n_connection *conn, uint8_t *session_id, size_t max_length);

S2N_API extern int s2n_connection_is_session_resumed(struct s2n_connection *conn);

S2N_API extern int s2n_connection_is_oscp_stapled(struct s2n_connection *conn);

typedef enum {
  S2N_TLS_SIGNATURE_ANONYMOUS = 0,
  S2N_TLS_SIGNATURE_RSA = 1,
  S2N_TLS_SIGNATURE_ECDSA = 3,
  S2N_TLS_SIGNATURE_MLDSA = 9,

  S2N_TLS_SIGNATURE_RSA_PSS_RSAE = 224,
  S2N_TLS_SIGNATURE_RSA_PSS_PSS
} s2n_tls_signature_algorithm;

typedef enum {
  S2N_TLS_HASH_NONE = 0,
  S2N_TLS_HASH_MD5 = 1,
  S2N_TLS_HASH_SHA1 = 2,
  S2N_TLS_HASH_SHA224 = 3,
  S2N_TLS_HASH_SHA256 = 4,
  S2N_TLS_HASH_SHA384 = 5,
  S2N_TLS_HASH_SHA512 = 6,

  S2N_TLS_HASH_MD5_SHA1 = 224
} s2n_tls_hash_algorithm;

S2N_API extern int s2n_connection_get_selected_signature_algorithm(struct s2n_connection *conn,
    s2n_tls_signature_algorithm *chosen_alg);

S2N_API extern int s2n_connection_get_selected_digest_algorithm(struct s2n_connection *conn, s2n_tls_hash_algorithm *chosen_alg);

S2N_API extern int s2n_connection_get_selected_client_cert_signature_algorithm(struct s2n_connection *conn, s2n_tls_signature_algorithm *chosen_alg);

S2N_API extern int s2n_connection_get_selected_client_cert_digest_algorithm(struct s2n_connection *conn, s2n_tls_hash_algorithm *chosen_alg);

S2N_API extern int s2n_connection_get_signature_scheme(struct s2n_connection *conn, const char **scheme_name);

S2N_API extern struct s2n_cert_chain_and_key *s2n_connection_get_selected_cert(struct s2n_connection *conn);

S2N_API extern int s2n_cert_chain_get_length(const struct s2n_cert_chain_and_key *chain_and_key, uint32_t *cert_length);

S2N_API extern int s2n_cert_chain_get_cert(const struct s2n_cert_chain_and_key *chain_and_key, struct s2n_cert **out_cert, const uint32_t cert_idx);

S2N_API extern int s2n_cert_get_der(const struct s2n_cert *cert, const uint8_t **out_cert_der, uint32_t *cert_length);

S2N_API extern int s2n_connection_get_peer_cert_chain(const struct sn2_connection *conn, struct s2n_cert_chain_and_key *cert_chain);

S2N_API extern int s2n_cert_get_x509_extension_value_length(struct s2n_cert *cert, const uint8_t *oid, uint32_t *ext_value_len);

S2N_API extern int s2n_cert_get_x509_extension_value(struct s2n_cert *cert, const uint8_t *oid, uint8_t *ext_value, uint32_t *ext_value_len, bool *critical);

S2N_API extern int s2n_cert_get_utf8_string_from_extension_data_length(const uint8_t *extension_data, uint32_t extension_len, uint32_t *utf8_str_len);

S2N_API extern int s2n_cert_get_utf8_string_from_extension_data(const uint8_t *extension_data, uint32_t extension_len, uint8_t *out_data, uint32_t *out_len);

typedef enum {
  S2N_PSK_HMAC_SHA256,
  S2N_PSK_HMAC_SHA384,
} s2n_psk_hmac;

struct s2n_psk;

S2N_API struct s2n_psk *s2n_external_psk_new(void);

S2N_API int s2n_psk_free(struct s2n_psk **psk);

S2N_API int s2n_psk_set_identity(struct s2n_psk *psk, const uint8_t *identity, uint16_t identity_size);

S2N_API int s2n_psk_set_secret(struct s2n_psk *psk, const uint8_t *secret, uint16_t secret_size);

S2N_API int s2n_psk_set_hmac(struct s2n_psk *psk, s2n_psk_hmac hmac);

S2N_API int s2n_connection_append_psk(struct s2n_connection *conn, struct s2n_psk *psk);

typedef enum {
  S2N_PSK_MODE_RESUMPTION,
  S2N_PSK_MODE_EXTERNAL
} s2n_psk_mode;

S2N_API int s2n_config_set_psk_mode(struct s2n_config *config, s2n_psk_mode mode);

S2N_API int s2n_connection_set_psk_mode(struct s2n_connection *conn, s2n_psk_mode mode);

S2N_API int s2n_connection_get_negotiated_psk_identity_length(struct s2n_connection *conn, uint16_t *identity_length);

S2N_API int s2n_connection_get_negotiated_psk_identity(struct s2n_connection *conn, uint8_t *identity, uint16_t max_identity_length);

struct s2n_offered_psk;

S2N_API struct s2n_offered_psk *s2n_offered_psk_new(void);

S2N_API int s2n_offered_psk_free(struct s2n_offered_psk **psk);

S2N_API int s2n_offered_psk_get_identity(struct s2n_offered_psk *psk, uint8_t **identity, uint16_t *size);

struct s2n_offered_psk_list;

S2N_API bool s2n_offered_psk_list_has_next(struct s2n_offered_psk_list *psk_list);

S2N_API int s2n_offered_psk_list_next(struct s2n_offered_psk_list *psk_list, struct s2n_offered_psk *psk);

S2N_API int s2n_offered_psk_list_reread(struct s2n_offered_psk_list *psk_list);

S2N_API int s2n_offered_psk_list_choose_psk(struct s2n_offered_psk_list *psk_list, struct s2n_offered_psk *psk);

typedef int (*s2n_psk_selection_callback)(struct s2n_connection *conn, void *context, 
    struct s2n_offered_psk_list *psk_list);

S2N_API int s2n_config_set_psk_selection_callback(struct s2n_config *config, s2n_psk_selection_callback cb, void *context);

S2N_API extern uint64_t s2n_connection_get_wire_bytes_in(struct s2n_connection *conn);

S2N_API extern uint64_t s2n_connection_get_wire_bytes_out(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_client_protocol_version(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_server_protocol_version(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_actual_protocol_version(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_client_hello_version(struct s2n_connection *conn);

S2N_API extern int s2n_client_hello_get_legacy_record_version(struct s2n_client_hello *ch, uint8_t *out);

S2N_API extern int s2n_connection_client_cert_used(struct s2n_connection *conn);

S2N_API extern const char *s2n_connection_get_cipher(struct s2n_connection *conn);

typedef enum {
  S2N_SNI_NONE = 1,
  S2N_SNI_EXACT_MATCH,
  S2N_SNI_WILDCARD_MATCH,
  S2N_SNI_NO_MATCH,
} s2n_cert_sni_match;

S2N_API extern int s2n_connection_get_certificate_match(struct s2n_connection *conn, s2n_cert_sni_match *match_status);

S2N_API extern int s2n_connection_get_master_secret(const struct s2n_connection *conn,
    uint8_t *secret_bytes, size_t max_size);

S2N_API extern int s2n_connection_tls_exporter(struct s2n_connection *conn,
    const uint8_t *label, uint32_t label_length, const uint8_t *context, uint32_t context_length,    uint8_t *output, uint32_t output_length);

S2N_API extern int s2n_connection_get_cipher_iana_value(struct s2n_connection *conn,
    uint8_t *first, uint8_t *second);

S2N_API extern int s2n_connection_is_valid_for_cipher_preferences(struct s2n_connection *conn, const char *version);

S2N_API extern const char *s2n_connection_get_curve(struct s2n_connection *conn);

S2N_API extern const char *s2n_connection_get_kem_name(struct s2n_connection *conn);

S2N_API extern const char *s2n_connection_get_kem_group_name(struct s2n_connection *conn);

S2N_API extern int s2n_connection_get_key_exchange_group(struct s2n_connection *conn, const char **group_name);

S2N_API extern int s2n_connection_get_alert(struct s2n_connection *conn);

S2N_API extern const char *s2n_connection_get_handshake_type_name(struct s2n_connection *conn);

S2N_API extern const char *s2n_connection_get_last_message_name(struct s2n_connection *conn);

struct s2n_async_pkey_op;

typedef enum {
  S2N_ASYNC_PKEY_VALIDATION_FAST,
  S2N_ASYNC_PKEY_VALIDATION_STRICT
} s2n_async_pkey_validation_mode;

typedef enum {
  S2N_ASYNC_DECRYPT,
  S2N_ASYNC_SIGN
} s2n_async_pkey_op_type;

typedef int (*s2n_async_pkey_fn)(struct s2n_connection *conn, struct s2n_async_pkey_op *op);

S2N_API extern int s2n_config_set_async_pkey_callback(struct s2n_config *config, s2n_async_pkey_fn fn);

S2N_API extern int s2n_async_pkey_op_perform(struct s2n_async_pkey_op *op, s2n_cert_private_key *key);

S2N_API extern int s2n_async_pkey_op_apply(struct s2n_async_pkey_op *op, struct s2n_connection *conn);

S2N_API extern int s2n_async_pkey_op_free(struct s2n_async_pkey_op *op);

S2N_API extern int s2n_config_set_async_pkey_validation_mode(struct s2n_config *config, s2n_async_pkey_validation_mode mode);

S2N_API extern int s2n_async_pkey_op_get_op_type(struct s2n_async_pkey_op *op, s2n_async_pkey_op_type *type);

S2N_API extern int s2n_async_pkey_op_get_input_size(struct s2n_async_pkey_op *op, uint32_t *data_len);

S2N_API extern int s2n_async_pkey_op_get_input(struct s2n_async_pkey_op *op, uint8_t *data, uint32_t data_len);

S2N_API extern int s2n_async_pkey_op_set_output(struct s2n_async_pkey_op *op, const uint8_t *data, uint32_t data_len);

typedef int (*s2n_key_log_fn)(void *ctx, struct s2n_connection *conn, uint8_t *logline, size_t len);

S2N_API extern int s2n_config_set_key_log_cb(struct s2n_config *config, s2n_key_log_fn callback, void *ctx);

S2N_API extern int s2n_config_enable_cert_req_dss_legacy_compat(struct s2n_config *config);

S2N_API int s2n_config_set_server_max_early_data_size(struct s2n_config *config, uint32_t max_early_data_size);

S2N_API int s2n_connection_set_server_max_early_data_size(struct s2n_connection *conn, uint32_t max_early_data_size);

S2N_API int s2n_connection_set_server_early_data_context(struct s2n_connection *conn, const uint8_t *context, uint16_t context_size);

S2N_API int s2n_psk_configure_early_data(struct s2n_psk *psk, uint32_t max_early_data_size,
    uint8_t cipher_suite_first_byte, uint8_t cipher_suite_second_byte);

S2N_API int s2n_psk_set_application_protocol(struct s2n_psk *psk, const uint8_t *application_protocol, uint8_t size);

S2N_API int s2n_psk_set_early_data_context(struct s2n_psk *psk, const uint8_t *context, uint16_t size);

typedef enum {
  S2N_EARLY_DATA_STATUS_OK,
  S2N_EARLY_DATA_STATUS_NOT_REQUESTED,
  S2N_EARLY_DATA_STATUS_REJECTED,
  S2N_EARLY_DATA_STATUS_END,
} s2n_early_data_status_t;

S2N_API int s2n_connection_get_early_data_status(struct s2n_connection *conn, s2n_early_data_status_t *status);

S2N_API int s2n_connection_get_remaining_early_data_size(struct s2n_connection *conn, uint32_t *allowed_early_data_size);

S2N_API int s2n_connection_get_max_early_data_size(struct s2n_connection *conn, uint32_t *max_early_data_size);

S2N_API int s2n_send_early_data(struct s2n_connection *conn, const uint8_t *data, ssize_t data_len, ssize_t *data_sent, s2n_blocked_status *blocked);

S2N_API int s2n_recv_early_data(struct s2n_connection *conn, uint8_t *data, ssize_t max_data_len,
 ssize_t *data_received, s2n_blocked_status *blocked);

struct s2n_offered_early_data;

typedef int (*s2n_early_data_cb)(struct s2n_connection *conn, struct s2n_offered_early_data *early_data);

S2N_API int s2n_config_set_early_data_cb(struct s2n_config *config, s2n_early_data_cb cb);

S2N_API int s2n_offered_early_data_get_context_length(struct s2n_offered_early_data *early_data,
    uint16_t *context_len);

S2N_API int s2n_offered_early_data_get_context(struct s2n_offered_early_data *early_data, uint8_t *context, uint16_t max_len);

S2N_API int s2n_offered_early_data_reject(struct s2n_offered_early_data *early_data);

S2N_API int s2n_offered_early_data_accept(struct s2n_offered_early_data *early_data);

S2N_API int s2n_config_get_supported_groups(struct s2n_config *config, uint16_t *groups, uint16_t groups_count_max, uint16_t *groups_count);

typedef enum {
  S2N_SERIALIZED_CONN_NONE = 0,
  S2N_SERIALIZED_CONN_V1 = 1
} s2n_serialization_version;

S2N_API int s2n_config_set_serialization_version(struct s2n_config *config, s2n_serialization_version version);

S2N_API int s2n_connection_serialization_length(struct s2n_connection *conn, uint32_t *length);

S2N_API int s2n_connection_serialize(struct s2n_connection *conn, uint8_t *buffer, uint32_t buffer_length);

S2N_API int s2n_connection_deserialize(struct s2n_connection *conn, uint8_t *buffer, uint32_t buffer_length);

S2N_API int s2n_config_set_cert_authorities_from_trust_store(struct s2n_config *config);

#ifdef __cplusplus 
}
#endif

