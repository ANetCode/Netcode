#include "common.h"
#include <iomanip>

buffer::buffer() {
    init();
}
buffer::buffer(size_t size) {
    init();
    alloc(size);
}

buffer::buffer(const buffer &obj) {
    init();
    copy(obj);
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

void buffer::copy(const buffer& other) {
    copy(other.m_data, other.m_size);
}

void buffer::set_size(size_t size) {
    m_size = size;
}

void buffer::dump() const{
    std::stringstream ss;
    
    ss << "\t0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F\n";
    int line = 0;
    
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < m_size; i++) {
        if (i % 15 == 0) {
            ss << "\n" << std::setw(3) << line++ << "\t";
        }
        uint8_t v = m_data[i];
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)v << " ";
    }
    logd("\ndump buffer, size:%zu:\n%s", m_size, ss.str().c_str());
}
void buffer::pop_front(size_t sz) {
    if (sz <= size()) {
        memcpy(m_data, m_data + sz, m_size - sz);
        m_size -= sz;
    } else {
        m_size = 0;
    }
}