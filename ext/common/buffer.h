#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

class buffer {
public:
    buffer();
    buffer(size_t size);
    buffer(const buffer &obj);
    ~buffer();

    void alloc(size_t size);
    void release();
    void reset();

    void copy(const uint8_t *data, size_t size);
    void copy(const buffer* other);
    const uint8_t* r_data() { return m_data; }
    uint8_t*       w_data() { return m_data + m_size; }
    size_t         size() { return m_size; }
    size_t         remains() { return m_capacity - m_size; }
private:
    void init();
private:
    uint8_t * m_data;
    size_t m_size;
    size_t m_capacity;
};

#endif // BUFFER_H