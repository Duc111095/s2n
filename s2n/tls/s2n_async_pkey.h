#pragma once

#include "crypto/s2n_signature.h"
#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

struct s2n_connection;

typedef int (*s2n_async_pkey_sign_complete)(struct s2n_connection *conn, struct s2n_blob *signature);
typedef int (*s2n_async_pkey_decrypt_complete)(struct s2n_connection *conn, bool rsa_failed, struct s2n_blob *decrypted);

struct s2n_async_pkey_op;

#define S2N_ASYNC_PKEY_GUARD(conn)  \
  do {  \
    __typeof(conn) __tmp_conn = (conn); \
    POSIX_GUARD_PTR(__tmp_conn);  \
    switch (conn->handshake.async_state){  \
      case S2N_ASYNC_NOT_INVOKED: \
          break;  \
      case S2N_ASYNC_INVOKED: \
          POSIX_BAIL(S2N_ERR_ASYNC_BLOCKED);  \
      case S2N_ASYNC_COMPLETE:  \
          _tmp_conn->handshake.async_state = S2N_ASYNC_NOT_INVOKED; \
          return S2N_SUCCESS; \
    } \
  } while (0)

#define S2N_ASYNC_PKEY_DECRYPT(conn, encrypted, init_decrypted, on_complete) \
  return s2n_result_is_ok(s2n_async_pkey_decrypt(conn, sig_alg, digest, on_complete)) ? S2N_SUCCESS : S2N_FAILURE;

#define S2N_ASYNC_PKEY_SIGN(conn, sig_alg, digest, on_complete) \
  return s2n_result_is_ok(s2n_async_pkey_sign(conn, sig_alg, digest, on_complete)) ? S2N_SUCCESS : S2N_FAILURE;

int s2n_async_pkey_op_perform(struct s2n_async_pkey_op *op, s2n_cert_private_key *key);
int s2n_async_pkey_op_apply(struct s2n_async_pkey_op *op, struct s2n_connection *conn);
int s2n_async_pkey_op_free(struct s2n_async_pkey_op *op);

int s2n_async_pkey_op_get_op_type(struct s2n_async_pkey_op *op, s2n_async_pkey_op_type *type);
int s2n_async_pkey_op_get_input_size(struct s2n_async_pkey_op *op, uint32_t *data_len);
int s2n_async_pkey_op_get_input(struct s2n_async_pkey_op *op, uint8_t *data, uint32_t data_len);
int s2n_async_pkey_op_set_output(struct s2n_async_pkey_op *op, const uint8_t *data, uint32_t data_len);
int s2n_async_pkey_op_set_validation_mode(struct s2n_async_pkey_op *op, s2n_async_pkey_validation_mode mode);

S2N_RESULT s2n_async_pkey_verify_signature(struct s2n_connection *conn, s2n_signature_algorithm sig_alg, struct s2n_hash_state *digest, struct s2n_blob *signature);
S2N_RESULT s2n_async_pkey_decrypt(struct s2n_connection *conn, struct s2n_blob *encrypted, struct s2n_blob *init_decrypted, s2n_async_pkey_decrypt_complete on_complete);
S2N_RESULT s2n_async_pkey_sign(struct s2n_connection *conn, s2n_signature_algorithm sig_alg, struct s2n_hash_state *digest, s2n_async_pkey_sign_complete on_complete);

struct s2n_async_pkey_verify_data {
  struct s2n_hash_state digest;
  s2n_signature_algorithm sig_alg;
  struct s2n_blob signature;
};

int s2n_async_pkey_verify(struct s2n_connection *conn, s2n_signature_algorithm sig_alg,
    struct s2n_hash_state *digest, struct s2n_blob *signature);
