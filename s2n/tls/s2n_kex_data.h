#pragma once

#include "utils/s2n_blob.h"

struct s2n_ecdhe_raw_server_params {
  struct s2n_blob point_blob;
  struct s2n_blob curve_blob;
};

struct s2n_dhe_raw_server_points {
  struct s2n_blob p;
  struct s2n_blob g;
  struct s2n_blob Ys;
};

struct s2n_kem_raw_server_params {
  struct s2n_blob kem_name;
  struct s2n_blob raw_public_key;
};

struct s2n_kex_raw_server_data {
  struct s2n_ecdhe_raw_server_params ecdhe_data;
  struct s2n_dhe_raw_server_points dhe_data;
  struct s2n_kem_raw_server_params kem_data;
};
