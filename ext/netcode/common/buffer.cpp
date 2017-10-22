#include "common.h"

buffer::buffer() {
    init();
}
buffer::buffer(size_t size) {
    init();
    alloc(size);
}

buffer::buffer(const buffer &obj) {
    init();
    copy(&obj);
}

buffer::~buffer() {
    release();
}

void buffer::init() {
    m_data     = nullptr;
    m_capacity = 0;
    m_size     = 0;
}

void buffer::release() {
    free(m_data);
    m_data     = nullptr;
    m_capacity = 0;
    m_size     = 0;
} 

void buffer::alloc(size_t size) {
    void* ptr = realloc(m_data, size);
    if (ptr != NULL) {
        m_data = (uint8_t*)ptr;
        m_size = 0;
        m_capacity = size;
    }
}

void buffer::reset() {
    memset(m_data, 0, m_capacity);
    m_size = 0;
}

void buffer::copy(const uint8_t *data, size_t size) {
    if (m_capacity - m_size < size) {
        alloc(size);
    }
    if (m_capacity - m_size >= size) {
        memcpy(m_data + m_size, data, size);
        m_size += size;
    }
}

void buffer::copy(const buffer* other) {
    copy(other->m_data, other->m_size);
}