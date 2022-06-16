#include <initializer_list>
#include <stdint.h>
#include <GL/glew.h>
#include <iostream>
#include "../SmartVector.hpp"
#include "../Utility.hpp"

template<typename T>
struct VertexElement;

template<typename T>
class InstancedVertexBuffer {
    uint32_t VBO;
    uint32_t draw_type;
public:
    using type = T;
    SmartVector<T, true> data;

    InstancedVertexBuffer() {
        glGenBuffers(1, &VBO);
        data.reserve(1024);
    }

    ~InstancedVertexBuffer() {
        glDeleteBuffers(1, &VBO);
        unbind();
    }

    void create() {
        
    }

    void update() {
        bind();
        glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_DYNAMIC_DRAW);
        unbind();
    }

    void updateIndex(uint32_t index) {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(T), sizeof(T), data.data()+index);
        unbind();
    }

    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

template<typename T>
struct VertexElement {
    InstancedVertexBuffer<T>* buffer;
    uint32_t index;

    VertexElement(InstancedVertexBuffer<T>& buffer_) : buffer(&buffer_) {
        index = buffer->data.size();
        buffer->data.emplace_back();
    }

    ~VertexElement() {
        if (index < buffer->data.size()) { // safe remove
            buffer->data.erase(buffer->data.begin()+index);
        }
        
        //buffer->last_element->back->next = nullptr;
        //buffer->last_element = buffer->last_element->back;
    }

    [[nodiscard]] T& getValue() const {
        return buffer->data[index];
    }

    void setValue(const T& v) {
        buffer->data[index] = v;
        buffer->updateIndex(index);
    }

        //it_value = buffer->data.end()-1;
    /*VertexElement& operator=(const VertexElement& e) {
        return *this;
    }*/

        /*buffer->data[index] = buffer->data[buffer->data.size()-1];
        buffer->elements[index] = buffer->elements[buffer->data.size()-1];
        buffer->elements[index]->index = index;
        buffer->data.pop_back();
        buffer->elements.pop_back();*/
};