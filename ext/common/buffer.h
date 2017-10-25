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
    void copy(const buffer& other);
    void append(uint8_t data);
    void pop_front(size_t size);
    const uint8_t* r_data() const { return m_data; };
    uint8_t*       w_data() { return m_data + m_size; }
    void           set_size (size_t size);
    size_t         size() const { return m_size; }
    size_t         remains() const { return m_capacity - m_size; }
    void dump() const;
private:
    void init();
private:
    uint8_t * m_data;
    size_t m_size;
    size_t m_capacity;
};

#endif // BUFFER_H