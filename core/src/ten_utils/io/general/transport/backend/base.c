//
// Copyright © 2025 Agora
// This file is part of TEN Framework, an open source project.
// Licensed under the Apache License, Version 2.0, with certain conditions.
// Refer to the "LICENSE" file in the root directory for more information.
//
#include "ten_utils/io/general/transport/backend/base.h"

#include "include_internal/ten_utils/io/runloop.h"
#include "include_internal/ten_utils/lib/safe_cast.h"
#include "ten_utils/io/stream.h"
#include "ten_utils/io/transport.h"
#include "ten_utils/macro/check.h"
#include "ten_utils/macro/memory.h"

void ten_transportbackend_init(ten_transportbackend_t *self,
                               ten_transport_t *transport,
                               const ten_string_t *name) {
  TEN_ASSERT(self, "Invalid argument.");

  ten_atomic_store(&self->is_close, false);
  self->transport = transport;

  int int_name_len = 0;
  bool rc = safe_cast_size_t_to_int(name->buf_size, &int_name_len);
  TEN_ASSERT(rc, "Name length overflow detected.");

  self->name = ten_string_create_formatted("%.*s", int_name_len, name->buf);

  self->impl = TEN_STRDUP(transport->loop->impl);
}

void ten_transportbackend_deinit(ten_transportbackend_t *self) {
  TEN_ASSERT(self, "Invalid argument.");
  if (self->name) {
    ten_string_destroy(self->name);
  }

  if (self->impl) {
    TEN_FREE(self->impl);
  }
}

void ten_streambackend_init(const char *impl, ten_streambackend_t *backend,
                            ten_stream_t *stream) {
  TEN_ASSERT(backend && stream, "Invalid argument.");

  ten_atomic_store(&backend->is_close, false);
  backend->stream = stream;

  stream->backend = backend;
  backend->impl = TEN_STRDUP(impl);
}

void ten_streambackend_deinit(ten_streambackend_t *backend) {
  TEN_ASSERT(backend, "Invalid argument.");
  if (backend->impl) {
    TEN_FREE(backend->impl);
  }
}
