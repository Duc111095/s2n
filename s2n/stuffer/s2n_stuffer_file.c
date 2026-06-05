#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "error/s2n_error.h"
#include "stuffer/s2n_stuffer.h"
#include "utils/s2n_io.h"
#include "utils/s2n_safety.h"

int s2n_stuffer_recv_from_fd(struct s2n_stuffer* stuffer, const int rfd, const uint32_t len, uint32_t* bytes_written)
{
	POSIX_PRECONDITION(s2n_stuffer_validate(stuffer));

	POSIX_GUARD(s2n_stuffer_skip_write(stuffer, len));
	stuffer->write_cursor -= len;

	ssize_t r = 0;
	POSIX_ENSURE(stuffer->blob.data, S2N_ERR_READ);
	S2N_IO_RETRY_EINTR(r, read(rfd, stuffer->blob.data + stuffer->write_cursor - len));
	POSIX_ENSURE(r >= 0, S2N_ERR_READ);

	POSIX_ENSURE((size_t)r <= UINT32_MAX, S2N_ERR_INTEGER_OVERFLOW);
	POSIX_GUARD(s2n_stuffer_skip_write(stuffer, (uint32_t)r));
	if (bytes_written != NULL) {
		*bytes_written = r;
	}
	return S2N_SUCCESS;
}

int s2n_stuffer_send_to_fd(struct s2n_stuffer* stuffer, const int wfd, const uint32_t len, uint32_t* bytes_sent) {
	POSIX_PRECONDITION(s2n_stuffer_validate(stuffer));

	POSIX_GUARD(s2n_stuffer_skip_read(stuffer, len));

	stuffer->read_cursor -= len;
	ssize_t w = 0;
	POSIX_ENSURE(stuffer->blob.data, S2N_ERR_WRITE);
	S2N_IO_RETRY_EINTR(w, write(wfd, stuffer->blob.data + stuffer->read_cursor, len));
	POSIX_ENSURE(w >= 0, S2N_ERR_WRITE);

	POSIX_ENSURE((size_t)w <= UINT32_MAX - stuffer->read_cursor, S2N_ERR_INTEGER_OVERFLOW);
	stuffer->read_cursor += w;
	if (bytes_sent != NULL) 
	{
		*bytes_sent = w;
	}
	return S2N_SUCCESS;
}