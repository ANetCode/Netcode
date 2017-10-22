#include "message.h"

message::message(lua_State* L) {
    this->L = L;
    m_udata = nullptr;
    reset();
}

bool message::parse(buffer& buffer) {
    if (m_ready == true) 
        return true;
    
    if(m_message_start == false) {
        m_message_start = true;
        const uint8_t* raw_data = buffer.r_data();
        m_length = *((int32_t*)raw_data);
        buffer.pop_front(4);
    }

    // 完成此消息还需要的长度
    size_t nbyte_left = m_length - m_buffer.size();
    if(nbyte_left >= buffer.size()) { // buffer 数据不够, 全部吃完
        m_buffer.copy(buffer);
        buffer.reset();
    } else {
        // 吃掉一部分buffer, 余下部分是数据其他消息
        m_buffer.copy(buffer.r_data(), nbyte_left);
        buffer.pop_front(nbyte_left);
    }

    // is is ready
    if (m_length == m_buffer.size()) {
        m_content = std::string((char*) m_buffer.r_data());
        m_ready = true;
        m_message_start = false;
        m_buffer.reset();
    }

    return true;
}

bool message::is_ready() { 
    return m_ready; 
}

std::string message::content() {
    return m_content;
}

void message::reset() {
    m_message_start = false;
    m_ready = false;
    m_content = "";
}