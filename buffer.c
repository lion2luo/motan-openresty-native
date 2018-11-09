//
// Created by minggang on 2018/11/9.
//
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "motan.h"
#include "buffer.h"

motan_bytes_buffer_t *motan_new_bytes_buffer(size_t capacity, byte_order_t order) {
    motan_bytes_buffer_t *mb = (motan_bytes_buffer_t *) malloc(sizeof(motan_bytes_buffer_t));
    if (mb == NULL) {
        return NULL;
    }
    mb->buffer = (uint8_t *) malloc(capacity * sizeof(uint8_t));
    if (mb->buffer == NULL) {
        free(mb);
        return NULL;
    }
    mb->order = order;
    mb->read_pos = 0;
    mb->write_pos = 0;
    mb->capacity = capacity;
    return mb;
}

void motan_free_bytes_buffer(motan_bytes_buffer_t *mb) {
    if (mb == NULL) {
        return;
    }
    if (mb->buffer != NULL) {
        free(mb->buffer);
        mb->buffer = NULL;
    }
    free(mb);
}

static int mb_grow_buffer(motan_bytes_buffer_t *mb, size_t n) {
    assert(mb != NULL);
    assert(mb->buffer != NULL);
    size_t new_cap = 2 * mb->capacity + n;
    uint8_t *new_buffer = (uint8_t *) malloc(new_cap);
    if (new_buffer == NULL) {
        return E_MOTAN_MEMORY_NOT_ENOUGH;
    }
    memcpy(new_buffer, mb->buffer, mb->capacity);
    free(mb->buffer);
    mb->buffer = new_buffer;
    mb->capacity = new_cap;
    return MOTAN_OK;
}

int mb_set_write_pos(motan_bytes_buffer_t *mb, uint32_t pos) {
    if (mb->capacity < pos) {
        int err = mb_grow_buffer(mb, pos - mb->capacity);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    mb->write_pos = pos;
    return MOTAN_OK;
}

int mb_set_read_pos(motan_bytes_buffer_t *mb, uint32_t pos) {
    mb->read_pos = pos;
    return MOTAN_OK;
}

inline void mb_reset(motan_bytes_buffer_t *mb) {
    mb->read_pos = 0;
    mb->write_pos = 0;
}


inline int mb_remain(motan_bytes_buffer_t *mb) {
    return mb->write_pos - mb->read_pos;
}

int mb_write_bytes(motan_bytes_buffer_t *mb, const uint8_t *bytes, int len) {
    if (mb->capacity < mb->write_pos + len) {
        int err = mb_grow_buffer(mb, len);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    memcpy((void *) (mb->buffer + mb->write_pos), (void *) bytes, len);
    mb->write_pos += len;
    return MOTAN_OK;
}

int mb_write_byte(motan_bytes_buffer_t *mb, uint8_t u) {
    if (mb->capacity < mb->write_pos + 1) {
        int err = mb_grow_buffer(mb, 1);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    mb->buffer[mb->write_pos] = u;
    mb->write_pos++;
    return MOTAN_OK;
}

int mb_write_uint16(motan_bytes_buffer_t *mb, uint16_t u) {
    if (mb->capacity < mb->write_pos + 2) {
        int err = mb_grow_buffer(mb, 2);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    if (mb->order == M_BIG_ENDIAN) {
        big_endian_write_uint16(mb->buffer, u);
    } else {
        little_endian_write_uint16(mb->buffer, u);
    }
    mb->write_pos += 2;
    return MOTAN_OK;
}

int mb_write_uint32(motan_bytes_buffer_t *mb, uint32_t u) {
    if (mb->capacity < mb->write_pos + 4) {
        int err = mb_grow_buffer(mb, 4);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    if (mb->order == M_BIG_ENDIAN) {
        big_endian_write_uint32(mb->buffer, u);
    } else {
        little_endian_write_uint32(mb->buffer, u);
    }
    mb->write_pos += 4;
    return MOTAN_OK;
}

int mb_write_uint64(motan_bytes_buffer_t *mb, uint64_t u) {
    if (mb->capacity < mb->write_pos + 8) {
        int err = mb_grow_buffer(mb, 8);
        if (err != MOTAN_OK) {
            return err;
        }
    }
    if (mb->order == M_BIG_ENDIAN) {
        big_endian_write_uint64(mb->buffer, u);
    } else {
        little_endian_write_uint64(mb->buffer, u);
    }
    mb->write_pos += 8;
    return MOTAN_OK;
}

int mb_write_varint(motan_bytes_buffer_t *mb, uint64_t u, int *len) {
    int l = 0;
    for (; u >= 1 << 7;) {
        int err = mb_write_byte(mb, (uint8_t) ((u & 0x7f) | 0x80));
        if (err != MOTAN_OK) {
            return err;
        }
        u >>= 7;
        l++;
    }
    int err = mb_write_byte(mb, (uint8_t) u);
    if (err != MOTAN_OK) {
        return err;
    }
    *len = l + 1;
    return MOTAN_OK;
}

int mb_read_bytes(motan_bytes_buffer_t *mb, uint8_t *bs, int len) {
    assert(len > 0);
    if (mb_remain(mb) < len) {
        return E_MOTAN_BUFFER_NOT_ENOUGH;
    }
    memcpy((void *) bs, (void *) (mb->buffer + mb->read_pos), len);
    mb->read_pos += len;
    return MOTAN_OK;
}


int mb_read_byte(motan_bytes_buffer_t *mb, uint8_t *u) {
    if (mb_remain(mb) < 1) {
        return E_MOTAN_BUFFER_NOT_ENOUGH;
    }
    *u = mb->buffer[mb->read_pos];
    mb->read_pos++;
    return MOTAN_OK;
}

int mb_read_uint16(motan_bytes_buffer_t *mb, uint16_t *u) {
    if (mb_remain(mb) < 2) {
        return E_MOTAN_BUFFER_NOT_ENOUGH;
    }
    if (mb->order == M_BIG_ENDIAN) {
        *u = big_endian_read_uint16(mb->buffer);
    } else {
        *u = little_endian_read_uint16(mb->buffer);
    }
    mb->read_pos += 2;
    return MOTAN_OK;
}

int mb_read_uint32(motan_bytes_buffer_t *mb, uint32_t *u) {
    if (mb_remain(mb) < 4) {
        return E_MOTAN_BUFFER_NOT_ENOUGH;
    }
    if (mb->order == M_BIG_ENDIAN) {
        *u = big_endian_read_uint32(mb->buffer);
    } else {
        *u = little_endian_read_uint32(mb->buffer);
    }
    mb->read_pos += 4;
    return MOTAN_OK;
}

int mb_read_uint64(motan_bytes_buffer_t *mb, uint64_t *u) {
    if (mb_remain(mb) < 8) {
        return E_MOTAN_BUFFER_NOT_ENOUGH;
    }
    if (mb->order == M_BIG_ENDIAN) {
        *u = big_endian_read_uint64(mb->buffer);
    } else {
        *u = little_endian_read_uint64(mb->buffer);
    }
    mb->read_pos += 8;
    return MOTAN_OK;
}

int mb_read_varint(motan_bytes_buffer_t *mb, uint64_t *u) {
    uint64_t r = 0;
    for (int shift = 0; shift < 64; shift += 7) {
        uint8_t b;
        int err = mb_read_byte(mb, &b);
        if (err != MOTAN_OK) {
            return err;
        }
        if ((b & 0x80) != 0x80) {
            r |= (uint64_t) b << shift;
            *u = r;
            return MOTAN_OK;
        }
        r |= (uint64_t) (b & 0x7f) << shift;
    }
    return E_MOTAN_OVERFLOW;
}

